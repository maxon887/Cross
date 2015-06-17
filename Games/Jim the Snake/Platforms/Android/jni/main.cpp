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
#include "JimTheSnake.h"

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "Cross++", __VA_ARGS__))

JimTheSnake* game = NULL;

void android_main(android_app* application){
	LOGI("android_main()");
	LauncherAndroid* launcher = CrossInit(application);
	if(game == NULL)
		game = new JimTheSnake(launcher);
	else
		game->launcher = launcher;
	CrossMain(game);
}
