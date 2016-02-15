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
#include "Commercial.h"
#include "LauncherAndroid.h"
#include "Screen.h"
#include "Audio.h"
#include "Graphics2D.h"

#include "jni.h"
#include "android/asset_manager_jni.h"

using namespace cross;

    Game* cross::game = NULL;

extern "C"{
	void Java_com_cross_Cross_Init(JNIEnv *env, jobject thiz, jint width, jint height, jstring dataPath, jobject assetManager, jobject crossActivity){
		LOGI("Cross_Init");
		//global_mutex.lock();
		AAssetManager* mng = AAssetManager_fromJava(env, assetManager);
		if(!mng){
			LOGI("Error loading asset manager");
		}
		string stdDataPath = env->GetStringUTFChars(dataPath, NULL);
		crossActivity = env->NewGlobalRef(crossActivity);
		launcher = new LauncherAndroid((int)width, (int)height, stdDataPath, mng, crossActivity, env);
		Audio::Init();
		game = CrossMain(launcher);
		gfx2D = new Graphics2D;
		//gfx3D = NULL;
		//global_mutex.unlock();
	}

	void Java_com_cross_Cross_Start(JNIEnv* env, jobject thiz){
		//global_mutex.lock();
		game->Init();
		game->Start();
		//global_mutex.unlock();
	}

	void Java_com_cross_Cross_Update(JNIEnv *env, jobject thiz){
		//global_mutex.lock();
		game->Update();
		//global_mutex.unlock();
	}

	void Java_com_cross_Cross_Suspend(JNIEnv *env, jobject thiz){
		LOGI("Cross_Suspend");
		//global_mutex.lock();
		game->Suspend();
		Audio::SuspendSystem();
		//global_mutex.unlock();
	}

	void Java_com_cross_Cross_Resume(JNIEnv *env, jobject thiz){
		LOGI("Cross_Resume");
		//global_mutex.lock();
		game->Resume();
		Audio::ResumeSystem();
		//global_mutex.unlock();
	}

	void Java_com_cross_Cross_Release(JNIEnv *env, jobject thiz){
		LOGI("Cross_Release");
		//global_mutex.lock();
		game->Suspend();
		delete game;
		Audio::Release();
		delete gfx2D;
		//delete gfx3D;
		delete launcher;
		//global_mutex.unlock();
	}

	void Java_com_cross_Cross_ActionDown(JNIEnv *env, jobject thiz, jfloat targetX, jfloat targetY){
		//float x = targetX / game->GetScaleFactor();
		//float y = targetY / game->GetScaleFactor();
		//input->TriggerActionDown(Point(x, y));
        TRIGGER_EVENT(input->ActionDown, Vector2D(targetX, launcher->GetTargetHeight() - targetY));
	}

	void Java_com_cross_Cross_ActionMove(JNIEnv *env, jobject thiz, jfloat targetX, jfloat targetY){
		//float x = targetX / game->GetScaleFactor();
		//float y = targetY / game->GetScaleFactor();
		//input->TriggerActionMove(Point(x, y));
        TRIGGER_EVENT(input->ActionMove, Vector2D(targetX, launcher->GetTargetHeight() - targetY));
	}

	void Java_com_cross_Cross_ActionUp(JNIEnv *env, jobject thiz, jfloat targetX, jfloat targetY){
		//float x = targetX / game->GetScaleFactor();
		//float y = targetY / game->GetScaleFactor();
		//input->TriggerActionUp(Point(x, y));
        TRIGGER_EVENT(input->ActionUp, Vector2D(targetX, launcher->GetTargetHeight() - targetY));
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
