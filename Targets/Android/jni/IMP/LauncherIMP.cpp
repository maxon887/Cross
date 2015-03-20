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

#include "IMP/LauncherIMP.h"

LauncherIMP::LauncherIMP(JNIEnv* env){
	jclass clazz = env->FindClass("com/cross/android/Launcher");
	jmethodID constructor = env->GetMethodID(clazz, "<init>", "()V");
	jobject jLauncher = env->NewObject(clazz, constructor);

	jmethodID method = env->GetMethodID(clazz, "GetTargetWidth", "()I");
	width = env->CallIntMethod(jLauncher, method);

	method = env->GetMethodID(clazz, "GetTargetHeight", "()I");
	height = env->CallIntMethod(jLauncher, method);

	env->DeleteLocalRef(jLauncher);
}

int LauncherIMP::GetTargetWidth(){
	return width;
}

int LauncherIMP::GetTargetHeight(){
	return height;
}

void LauncherIMP::LogIt(const char* str){
	__android_log_print(ANDROID_LOG_DEBUG, "Cross++", "%s", str);
}
