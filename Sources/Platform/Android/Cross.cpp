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
#include "LauncherAndroid.h"
#include "Screen.h"
#include "Audio.h"
#include "GraphicsGL.h"
#include "Graphics2D.h"

#include "jni.h"
#include "android/asset_manager_jni.h"

    using namespace cross;

    Game* cross::game = NULL;

extern "C"{
	void Java_com_cross_Cross_Init(JNIEnv *env, jobject thiz, jint width, jint height, jstring dataPath, jobject assetManager, jobject crossActivity){
		LOGI("Cross_Init");
        try {
            AAssetManager *mng = AAssetManager_fromJava(env, assetManager);
            if (!mng) {
                LOGI("Error loading asset manager");
            }
            string stdDataPath = env->GetStringUTFChars(dataPath, NULL);
            crossActivity = env->NewGlobalRef(crossActivity);
            launcher = new LauncherAndroid((int) width, (int) height, stdDataPath, mng,
                                           crossActivity, env);
            game = CrossMain(launcher);
            gfxGL = new GraphicsGL();
            gfx2D = new Graphics2D();
        } catch(Exception &exc) {
            string msg = string(exc.message) +
                         +"\nFile: " + string(exc.filename) +
                         +"\nLine: " + to_string(exc.line);
            LOGE("%s", msg.c_str());
        }
	}

	void Java_com_cross_Cross_Start(JNIEnv* env, jobject thiz){
		try{
            game->Start();
			game->Init();
		} catch(Exception &exc) {
            string msg = string(exc.message) +
                         +"\nFile: " + string(exc.filename) +
                         +"\nLine: " + to_string(exc.line);
            LOGE("%s", msg.c_str());
        }
	}

	void Java_com_cross_Cross_Update(JNIEnv *env, jobject thiz){
		try{
			game->Update();
		} catch(Exception &exc) {
            string msg = string(exc.message) +
                         +"\nFile: " + string(exc.filename) +
                         +"\nLine: " + to_string(exc.line);
            LOGE("%s", msg.c_str());
        }
	}

	void Java_com_cross_Cross_Suspend(JNIEnv *env, jobject thiz){
		LOGI("Cross_Suspend");
		try{
			game->Suspend();
			Audio::SuspendSystem();
		} catch(Exception &exc) {
            string msg = string(exc.message) +
                         +"\nFile: " + string(exc.filename) +
                         +"\nLine: " + to_string(exc.line);
            LOGE("%s", msg.c_str());
        }
	}

	void Java_com_cross_Cross_Resume(JNIEnv *env, jobject thiz){
		LOGI("Cross_Resume");
		try{
			game->Resume();
			Audio::ResumeSystem();
		} catch(Exception &exc) {
            string msg = string(exc.message) +
                         +"\nFile: " + string(exc.filename) +
                         +"\nLine: " + to_string(exc.line);
            LOGE("%s", msg.c_str());
        }
	}

	void Java_com_cross_Cross_Release(JNIEnv *env, jobject thiz){
		LOGI("Cross_Release");
		try{
            game->GetCurrentScreen()->Stop();
            game->Stop();
			delete game;
			Audio::Release();
			delete gfx2D;
            delete gfxGL;
			delete launcher;
		} catch(Exception &exc) {
            string msg = string(exc.message) +
                         +"\nFile: " + string(exc.filename) +
                         +"\nLine: " + to_string(exc.line);
            LOGE("%s", msg.c_str());
        }
	}

	void Java_com_cross_Cross_ActionDown(JNIEnv *env, jobject thiz, jfloat targetX, jfloat targetY){
        float x = targetX / game->GetScaleFactor();
        float y = game->GetHeight() - targetY / game->GetScaleFactor();
        TRIGGER_EVENT(input->ActionDown, Vector2D(x, y));
	}

	void Java_com_cross_Cross_ActionMove(JNIEnv *env, jobject thiz, jfloat targetX, jfloat targetY){
        float x = targetX / game->GetScaleFactor();
        float y = game->GetHeight() - targetY / game->GetScaleFactor();
        TRIGGER_EVENT(input->ActionMove, Vector2D(x, y));
	}

	void Java_com_cross_Cross_ActionUp(JNIEnv *env, jobject thiz, jfloat targetX, jfloat targetY){
        float x = targetX / game->GetScaleFactor();
        float y = game->GetHeight() - targetY / game->GetScaleFactor();
        TRIGGER_EVENT(input->ActionUp, Vector2D(x, y));
	}
	void Java_com_cross_Cross_PressKey(JNIEnv *env, jobject thiz, jint key){
		//input->TriggerKeyPressed((Key)key);
        TRIGGER_EVENT(input->KeyPressed, (Key)key);
	}
	void Java_com_cross_Cross_ReleaseKey(JNIEnv *env, jobject thiz, jint key){
		//input->TriggerKeyReleased((Key)key);
        TRIGGER_EVENT(input->KeyReleased, (Key)key);
	}

	void Java_com_cross_Cross_InitialCommercial(JNIEnv *env, jobject thiz, jobject comm){
		LOGI("Java_com_cross_Cross_InitialCommercial");
		comm = env->NewGlobalRef(comm);
		((LauncherAndroid*)launcher)->InitializeCommercial(env, comm);
	}

	void Java_com_cross_Cross_CommertialResult(JNIEnv* env, jobject thiz, jint event){
		launcher->GetCommercial()->CommercialResult((Commercial::Event)event);
	}
}
