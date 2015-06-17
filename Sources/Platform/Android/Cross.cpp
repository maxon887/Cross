	/*	Copyright © 2015 Lukyanau Maksim

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
#include "Graphics.h"
#include "Audio.h"
#include "Input.h"

#include <EGL/egl.h>
#include <jni.h>
#include <android/log.h>
#include <android/asset_manager.h>
#include <android_native_app_glue.h>

#include <unistd.h>
using namespace cross;

android_app* app = NULL;

EGLDisplay display = EGL_NO_DISPLAY;
EGLSurface surface = EGL_NO_SURFACE;
EGLContext context = EGL_NO_CONTEXT;

//bool eglInitialized = false;
LauncherAndroid* launcher = NULL;
Game* mGame = NULL;

extern "C"{
	void Java_com_cross_CrossActivity_InitAudio(JNIEnv *env, jobject thiz)
	{
		Audio::Init(NULL);

	}
	void Java_com_cross_CrossActivity_ReleaseAudio(JNIEnv* env, jobject thiz){
		Audio::Release();
	}
}

static void init_display(){
	LOGI("init_display()");
    const EGLint attribs[] = {
            EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
            EGL_BLUE_SIZE, 8,
            EGL_GREEN_SIZE, 8,
            EGL_RED_SIZE, 8,
            EGL_NONE
    };
    EGLint w, h, dummy, format;
    EGLint numConfigs;
    EGLConfig config;
    EGLBoolean result;

    display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    if(!display){
    	LOGI("ERROR: eglGetDisplay");
    }

    result = eglInitialize(display, 0, 0);
    if(!result){
    	LOGI("ERROR: eglInitialize");
    }

    result = eglChooseConfig(display, attribs, &config, 1, &numConfigs);
    if(!result){
    	LOGI("ERROR: eglChooseConfig");
    }

    result = eglGetConfigAttrib(display, config, EGL_NATIVE_VISUAL_ID, &format);
    if(!result){
    	LOGI("ERROR: eglGetConfigAttrib");
    }

    ANativeWindow_setBuffersGeometry(app->window, 0, 0, format);

    surface = eglCreateWindowSurface(display, config, app->window, NULL);
    if(context == EGL_NO_CONTEXT)
    	context = eglCreateContext(display, config, NULL, NULL);

    result = eglMakeCurrent(display, surface, surface, context);
	if(!result){
		LOGI("ERROR: eglMakeCurrent");
    }

    eglQuerySurface(display, surface, EGL_WIDTH, &w);
    eglQuerySurface(display, surface, EGL_HEIGHT, &h);

    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
    glEnable(GL_CULL_FACE);
    glShadeModel(GL_SMOOTH);
    glDisable(GL_DEPTH_TEST);

    //eglInitialized = true;
    launcher = new LauncherAndroid(app, w, h);
}

static int32_t handle_input(android_app* appl, AInputEvent* event){
	LOGI("handle_input 0");
    int32_t eventType = AInputEvent_getType(event);
    switch(eventType){
        case AINPUT_EVENT_TYPE_MOTION:
            switch(AInputEvent_getSource(event)){
                case AINPUT_SOURCE_TOUCHSCREEN:
                    int action = AKeyEvent_getAction(event) & AMOTION_EVENT_ACTION_MASK;
                    switch(action){
                        case AMOTION_EVENT_ACTION_DOWN:
                        case AMOTION_EVENT_ACTION_MOVE:
                        	mGame->input->input_state = true;
                        	mGame->input->input_loc.x = AMotionEvent_getX(event, 0);
                        	mGame->input->input_loc.y = AMotionEvent_getY(event, 0);
                        return 1;
                        case AMOTION_EVENT_ACTION_UP:
                        	mGame->input->input_state = false;
                        return 1;
                    }
                break;
            } // end switch
        break;
        case AINPUT_EVENT_TYPE_KEY:
        	int action = AKeyEvent_getAction(event);
				if(action == AKEY_EVENT_ACTION_DOWN){
				mGame->input->key_state = true;
				switch(AKeyEvent_getKeyCode(event)){
				case AKEYCODE_BACK:
					mGame->input->key_key = Key::PAUSE;
					return 1;
				case AKEYCODE_MENU:
					mGame->input->key_key = Key::PAUSE;
					return 1;
				}
        	}
        break;
    } // end switch
	return 0;
}

static void on_exit(){
	LOGI("on_exit()");
    if (display != EGL_NO_DISPLAY) {
        eglMakeCurrent(display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
        //if (engine->context != EGL_NO_CONTEXT) {
       //     eglDestroyContext(engine->display, engine->context);
       // }
        if (surface != EGL_NO_SURFACE) {
            eglDestroySurface(display, surface);
        }
        //eglTerminate(engine->display);
    }
    display = EGL_NO_DISPLAY;
    //engine->context = EGL_NO_CONTEXT;
    surface = EGL_NO_SURFACE;

    //eglInitialized = false;
    delete launcher;
    launcher = NULL;
}

static void handle_cmd(struct android_app* app, int32_t cmd) {
	switch(cmd){
	case APP_CMD_SAVE_STATE:
		LOGI("APP_CMD_SAVE_STATE");
		break;
	case APP_CMD_INIT_WINDOW:
		LOGI("APP_CMD_INIT_WINDOW");
		if(app->window != NULL){
			init_display();
		}
		break;
	case APP_CMD_TERM_WINDOW:
		LOGI("APP_CMD_TERM_WINDOW");
		on_exit();
		break;
	case APP_CMD_START:
		LOGI("APP_CMD_START");
		break;
	case APP_CMD_RESUME:
		LOGI("APP_CMD_RESUME");
		break;
	case APP_CMD_PAUSE:
		LOGI("APP_CMD_PAUSE");
		break;
	case APP_CMD_STOP:
		LOGI("APP_CMD_STOP");
		break;
	case APP_CMD_GAINED_FOCUS:
		LOGI("APP_CMD_GAINED_FOCUS");
		break;
	case APP_CMD_LOST_FOCUS:
		LOGI("APP_CMD_LOST_FOCUS");
		break;
	}
}

LauncherAndroid* CrossInit(android_app* application){
	LOGI("CrossInit");
	app = application;
	app_dummy();
	app->onAppCmd = handle_cmd;
	app->onInputEvent = handle_input;
#ifdef CROSSDEBUG
	//sleep(5);
#endif
	while(!launcher){
		android_poll_source* source;
		int events;
		int eventID = ALooper_pollOnce(0, NULL, &events, (void**)&source);
		if(eventID >= 0){
			if(source != NULL)
				source->process(app, source);
		}
	}
	return launcher;
}

void CrossMain(Game* game){
	LOGI("CrossMain");
	if(!mGame){
		mGame = game;
		mGame->graphics = new Graphics(mGame);
		mGame->Start();
	}else{
		mGame = game;
		mGame->launcher = launcher;
	}
	while(launcher){
		android_poll_source* source;
		int events;
		int eventID = ALooper_pollOnce(0, NULL, &events, (void**)&source);
		if(eventID >= 0){
			if(source != NULL)
				source->process(app, source);
		}
        // Check if we are exiting.
        if (app->destroyRequested != 0) {
        	LOGI("destroyRequested");
            return;
        }
		if(launcher){
			mGame->Update();
		    eglSwapBuffers(display, surface);
		}
	}
	ANativeActivity_finish(app->activity);
}
