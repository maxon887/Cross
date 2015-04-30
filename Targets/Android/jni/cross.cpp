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

#include <EGL/egl.h>
#include <jni.h>
#include <android/log.h>
#include <android/asset_manager.h>
#include <android_native_app_glue.h>

#include <unistd.h>

#include "Demo.h"
#include "LauncherAndroid.h"
#include "AudioAndroid.h"

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "Cross++", __VA_ARGS__))

android_app* app;

EGLDisplay display;
EGLSurface surface;
EGLContext context;

LauncherAndroid* launcher;
Game* game;

static void init_display(){
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

    display = eglGetDisplay(EGL_DEFAULT_DISPLAY);

    eglInitialize(display, 0, 0);

    eglChooseConfig(display, attribs, &config, 1, &numConfigs);

    eglGetConfigAttrib(display, config, EGL_NATIVE_VISUAL_ID, &format);

    ANativeWindow_setBuffersGeometry(app->window, 0, 0, format);

    surface = eglCreateWindowSurface(display, config, app->window, NULL);
    context = eglCreateContext(display, config, NULL, NULL);

    if (eglMakeCurrent(display, surface, surface, context) == EGL_FALSE) {
        LOGI("Unable to eglMakeCurrent");
        return;
    }

    eglQuerySurface(display, surface, EGL_WIDTH, &w);
    eglQuerySurface(display, surface, EGL_HEIGHT, &h);

    launcher = new LauncherAndroid(app, w, h);
}

static int32_t handle_input(android_app* appl, AInputEvent* event){
    int32_t eventType = AInputEvent_getType(event);
    switch(eventType){
        case AINPUT_EVENT_TYPE_MOTION:
            switch(AInputEvent_getSource(event)){
                case AINPUT_SOURCE_TOUCHSCREEN:
                    int action = AKeyEvent_getAction(event) & AMOTION_EVENT_ACTION_MASK;
                    switch(action){
                        case AMOTION_EVENT_ACTION_DOWN:
                        case AMOTION_EVENT_ACTION_MOVE:
                        	game->input->input_state = true;
                        	game->input->input_loc.x = AMotionEvent_getX(event, 0);
                        	game->input->input_loc.y = AMotionEvent_getY(event, 0);
                        break;
                        case AMOTION_EVENT_ACTION_UP:
                        	game->input->input_state = false;
                        break;
                    }
                break;
            } // end switch
        break;
        case AINPUT_EVENT_TYPE_KEY:
            // handle key input...
        break;
    } // end switch
	return 0;
}

static void handle_cmd(struct android_app* app, int32_t cmd) {
	switch(cmd){
	case APP_CMD_SAVE_STATE:
		break;
	case APP_CMD_INIT_WINDOW:
		if(app->window != NULL){
			init_display();
			game = new Demo(launcher);
			game->graphics = new Graphics(game);
			game->Start();
		}
		break;
	case APP_CMD_TERM_WINDOW:
		break;
	}
}

static void on_exit(){
	//AudioAndroid::Release();
}

void android_main(android_app* application){
	app = application;
	app_dummy();
	app->onAppCmd = handle_cmd;
	app->onInputEvent = handle_input;

	LOGI("android_main");
#ifdef CROSSDEBUG
	sleep(3);
#endif

	while(true){
		android_poll_source* source;
		int events;
		int eventID = ALooper_pollOnce(0, NULL, &events, (void**)&source);
		if(eventID >= 0){
			if(source != NULL)
				source->process(app, source);
			if(app->destroyRequested != 0){
				on_exit();
				return;
			}
		}
		if(game != NULL && launcher != NULL){
			game->Update();
		    eglSwapBuffers(display, surface);
		}
	}
}
