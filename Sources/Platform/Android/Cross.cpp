	/*	Copyright � 2015 Lukyanau Maksim

	This file is part of Cross++ Game Engine.

    Cross++ Game Engine is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Cross++ is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Cross++.  If not, see <http://www.gnu.org/licenses/>			*/
#include "Cross.h"
#include "Game.h"
#include "Input.h"
#include "Utils/Commercial.h"
#include "AndroidSystem.h"
#include "Screen.h"
#include "Audio.h"
#include "GraphicsGL.h"
#include "Graphics2D.h"
#include "Graphics3D.h"
#include "Config.h"
#include "System/Debugger.h"
#include "Platform/CrossEGL.h"

#include <android/native_window_jni.h>
#include <android/asset_manager_jni.h>

#include <pthread.h>
#include <mutex>

using namespace cross;

enum ApplicationState{
    APP_INIT,
    APP_START,
    APP_RUNNING,
    APP_PAUSED,
    APP_EXIT
};

enum WindowState{
    WND_NONE,
    WND_CREATE,
    WND_RELEASE,
    WND_ACTIVE
};

CrossEGL* crossEGL          = NULL;
ApplicationState app_state  = APP_INIT;
WindowState wnd_state       = WND_NONE;
pthread_t   threadID;
std::mutex  app_mutex;
std::mutex  pause_mutex;
int screen_width            = 0;
int screen_height           = 0;

void* Main(void* self){
    LOGI("Main()");
    try {
        while (app_state != APP_EXIT) {
            switch (app_state) {
                case APP_INIT: {
                    if (!crossEGL) {
                        crossEGL = new CrossEGL();
                        app_state = APP_START;
                    } else {
                        throw CrossException("Application try to initialize second time");
                    }
                    break;
                }
                case APP_START: {
                    if (wnd_state == WND_ACTIVE) {
                        game = CrossMain();
                        gfxGL = new GraphicsGL();
                        gfx2D = new Graphics2D();
                        gfx3D = new Graphics3D();
                        game->Start();
                        game->SetScreen(game->GetStartScreen());
                        app_state = APP_RUNNING;
                    }
                    break;
                }
                case APP_RUNNING:{
					pause_mutex.lock();
                    game->EngineUpdate();
					pause_mutex.unlock();
                    break;
                }
                case APP_PAUSED:{
                    pause_mutex.lock();
                    system->Sleep(16);
                    pause_mutex.unlock();
                    break;
                }
            }
            switch (wnd_state) {
                case WND_NONE:
                    break;
                case WND_CREATE:{
                    app_mutex.lock();
                    if(!crossEGL->IsContextCreated()) {
                        bool success = crossEGL->CreateContext(true);
                        if (success) {
                            LOGI("Window create success");
                            wnd_state = WND_ACTIVE;
                            system->SetWindowSize(screen_width, screen_height);
                        } else {
                            LOGI("Can not create native window");
                            app_state = APP_EXIT;
                        }
                    }else{
                        crossEGL->DestroyContext(false);
                        bool success = crossEGL->CreateContext(false);
                        if (success) {
                            LOGI("Window recreaded");
                            wnd_state = WND_ACTIVE;
                            app_state = APP_RUNNING;
                            system->SetWindowSize(screen_width, screen_height);
                        } else {
                            LOGI("Can not recread native window");
                            app_state = APP_EXIT;
                        }
                    }
                    app_mutex.unlock();
                    break;
                }
                case WND_RELEASE:
                    break;
                case WND_ACTIVE:
                    crossEGL->SwapBuffers();
                    break;
            }
        }
        //exit application
        game->GetCurrentScreen()->Stop();
        game->Stop();
        Debugger::Release();
        delete gfx3D;
        delete gfx2D;
        delete gfxGL;
        delete game;
        system->LogIt("Saved!");
        delete system;
    } catch (Exception& exc){
        string msg = string(exc.message) +
                     +"\nFile: " + string(exc.filename) +
                     +"\nLine: " + to_string(exc.line);
        LOGE("%s", msg.c_str());
        ((AndroidSystem *) system)->MessageBox(msg);
    }
    if(app_state == APP_EXIT){
        ((AndroidSystem *) system)->Exit();
    }else {
        ((AndroidSystem *) system)->DetachFromJVM();
		delete system;
    }
}

extern "C"{
	void Java_com_cross_Cross_OnCreate(JNIEnv *env, jobject thiz, jobject crossActivity, jobject assManager, jstring dataPath){
		LOGI("Cross_OnCreate");
        if(!system) {
            AAssetManager *mng = AAssetManager_fromJava(env, assManager);
            if (!mng) {
                LOGI("Error loading asset manager");
            }
            string stdDataPath = env->GetStringUTFChars(dataPath, NULL);
            crossActivity = env->NewGlobalRef(crossActivity);
            system = new AndroidSystem(env, crossActivity, mng, stdDataPath);
            audio = new Audio();
            pthread_create(&threadID, 0, Main, NULL);
        }else{
            LOGE("Attempt to initialize game second time");
        }
	}

    void Java_com_cross_Cross_SurfaceChanged(JNIEnv *env, jobject thiz, jobject surface, jint w, jint h){
        LOGI("Cross_SurfaceChanged w - %d, h - %d", w, h);
        ANativeWindow *nativeWindow = ANativeWindow_fromSurface(env, surface);
        app_mutex.lock();
        crossEGL->BindWindow(nativeWindow);
        screen_width = w;
        screen_height = h;
        wnd_state = WND_CREATE;
        app_mutex.unlock();
    }

    void Java_com_cross_Cross_SurfaceDestroyed(JNIEnv* env, jobject thiz){
        LOGI("Cross_SurfaceDestroyed");
        crossEGL->UnbindWindow();
        wnd_state = WND_RELEASE;
    }

    void Java_com_cross_Cross_OnResume(JNIEnv *env, jobject thiz){
        LOGI("Cross_OnResume");
        pause_mutex.unlock();
        if(game){
            game->Resume();
        }
    }

	void Java_com_cross_Cross_OnSuspend(JNIEnv *env, jobject thiz){
        LOGI("Cross_OnSuspend");
        pause_mutex.lock();
        if(game){
            game->Suspend();
        }
        if(config){
            config->SaveGameConfig();
            config->SaveUserConfig();
        }
	}

	void Java_com_cross_Cross_OnExit(JNIEnv *env, jobject thiz){
		LOGI("Cross_OnExit");
        app_state = APP_EXIT;
		delete audio;
	}

	void Java_com_cross_Cross_ActionDown(JNIEnv *env, jobject thiz, jfloat targetX, jfloat targetY, jint actionID){
        TRIGGER_EVENT(input->TargetActionDown, (float)targetX, (float)targetY, (int)actionID);
	}

	void Java_com_cross_Cross_ActionMove(JNIEnv *env, jobject thiz, jfloat targetX, jfloat targetY, jint actionID){
        TRIGGER_EVENT(input->TargetActionMove, (float)targetX, (float)targetY, (int)actionID);
	}

	void Java_com_cross_Cross_ActionUp(JNIEnv *env, jobject thiz, jfloat targetX, jfloat targetY, jint actionID){
        TRIGGER_EVENT(input->TargetActionUp, (float)targetX, (float)targetY, (int)actionID);
	}
	void Java_com_cross_Cross_PressKey(JNIEnv *env, jobject thiz, jint key){
        TRIGGER_EVENT(input->KeyPressed, (Key)key);
	}
	void Java_com_cross_Cross_ReleaseKey(JNIEnv *env, jobject thiz, jint key){
        TRIGGER_EVENT(input->KeyReleased, (Key)key);
	}

	void Java_com_cross_Cross_InitialCommercial(JNIEnv *env, jobject thiz, jobject comm){
		LOGI("Java_com_cross_Cross_InitialCommercial");
		comm = env->NewGlobalRef(comm);
		((AndroidSystem*)system)->InitializeCommercial(env, comm);
	}

	void Java_com_cross_Cross_CommertialResult(JNIEnv* env, jobject thiz, jint event){
        system->GetCommercial()->CommercialResult((Commercial::Event)event);
	}
}
