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
#include "LauncherAndroid.h"

#include "jni.h"
#include "android/asset_manager_jni.h"
#include <string>

using namespace std;
using namespace cross;

static Game* game;
static Graphics* graphics;
static LauncherAndroid* launcher;

void Init(int width, int heigth, string dataPath, AAssetManager* assetManager){
	LOGI("Init");
	launcher = new LauncherAndroid(width, heigth, dataPath, assetManager);
	Audio::Init(launcher);
	game = CrossMain(launcher);
	graphics = new Graphics(game);
	game->graphics = graphics;
	game->Start();
}

extern "C"{
	void Java_com_cross_Cross_Init(JNIEnv *env, jobject thiz, jint width, jint height, jstring dataPath, jobject assetManager){
		LOGI("Cross_Init");
		AAssetManager* mng = AAssetManager_fromJava(env, assetManager);
		if(!mng){
			LOGI("Error loading asset manager");
		}
		string stdDataPath = env->GetStringUTFChars(dataPath, NULL);
		Init((int)width, (int)height, stdDataPath, mng);
	}

	void Java_com_cross_Cross_Update(JNIEnv *env, jobject thiz){
		game->Update();
		game->input->key_state = false;
	}

	void Java_com_cross_Cross_Suspend(JNIEnv *env, jobject thiz){
		LOGI("Cross_Suspend");
		game->Suspend();
		Audio::SuspendSystem();
	}

	void Java_com_cross_Cross_Resume(JNIEnv *env, jobject thiz){
		LOGI("Cross_Resume");
		Audio::ResumeSystem();
	}

	void Java_com_cross_Cross_Release(JNIEnv *env, jobject thiz){
		LOGI("Cross_Release");
		delete game;
		Audio::Release();
		delete graphics;
		delete launcher;
	}


	void Java_com_cross_Cross_SetInputState(JNIEnv *env, jobject thiz, jboolean state){
		game->input->input_state = state;
	}
	void Java_com_cross_Cross_SetInputLoc(JNIEnv *env, jobject thiz, jfloat x, jfloat y){
		game->input->input_loc.x = x;
		game->input->input_loc.y = y;
	}
	void Java_com_cross_Cross_SetKeyState(JNIEnv *env, jobject thiz, jboolean state){
		game->input->key_state = state;
	}
	void Java_com_cross_Cross_SetKeyKey(JNIEnv *env, jobject thiz, jint key){
		game->input->key_key = (Key)key;
	}

	void Java_com_cross_Cross_InitialCommercial(JNIEnv *env, jobject thiz, jobject comm){
		launcher->InitializeCommercial(comm);
	}
}
