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

#include <jni.h>
#include <time.h>
#include "LauncherAndroid.h"
#include "Graphics.h"
#include "SnakyGame.h"
#include "Demo.h"
#include "Debuger.h"

Game* game;
Debuger* debuger;
clock_t render_time = 0;


static void Start(JNIEnv* env){
	LauncherAndroid* launcher = new LauncherAndroid(env);
    game = new Demo(launcher);
    Graphics* gfx = new Graphics(game);
    game->graphics = gfx;
    debuger = new Debuger(game);
    game->Start();
}

static void Update(){
	clock_t rend = clock() - render_time;
	render_time = clock();
	game->GetCurrentScreen()->Update(rend / (float)CLOCKS_PER_SEC);
	//game->GetCurrentScreen()->Update(0);
	debuger->Display(rend / (float)CLOCKS_PER_SEC);
	clock_t up = clock() - render_time;
	debuger->SetUpdateTime(up / (float)CLOCKS_PER_SEC);
}

static JNINativeMethod methods[] = {
		{ "Start", 	"()V", (void*)Start },
		{ "Update", "()V", (void*)Update }
};

jint JNI_OnLoad(JavaVM* vm, void* reserved)
{
    JNIEnv* env;
    if (vm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6) != JNI_OK) {
        return -1;
    }
    jclass clazz = env->FindClass("com/cross/android/IMP");
    if(clazz){
    	env->RegisterNatives(clazz, methods, sizeof(methods) / sizeof(JNINativeMethod));
    	env->DeleteLocalRef(clazz);
    }
    return JNI_VERSION_1_6;
}
