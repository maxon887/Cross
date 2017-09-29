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
#ifdef GLES
#include "GLES.h"
#include "Native.h"
#include "Game.h"
#include "Screen.h"
#include "WINSystem.h"
#include "GraphicsGL.h"
#include "Graphics2D.h"
#include "Graphics3D.h"
#include "Config.h"
#include "Audio.h"
#include "Utils/Debugger.h"
#include "Platform/CrossEGL.h"

#include <time.h>

using namespace cross;

CrossEGL* crossEGL = NULL;

int GLES_Main(){

	crossEGL = new CrossEGL();
	crossEGL->BindWindow(WinCreate());

	srand((U32)time(0));
	WINSystem* winSys = new WINSystem(crossEGL->GetWindow());
	cross::system = winSys;
	game = CrossMain();
	input->KeyReleased.Connect(winSys, &WINSystem::KeyReleasedHandle);

	int winX = config->GetInt("WIN_POS_X", 0);
	int winY = config->GetInt("WIN_POS_Y", 0);
	int winWidth = config->GetInt("WIN_WIDTH", 500);
	int winHeight = config->GetInt("WIN_HEIGHT", 500);
	winSys->ResizeWindow(winX, winY, winWidth, winHeight);

	crossEGL->CreateContext(true);

	ShowWindow(crossEGL->GetWindow(), TRUE);

	audio = new Audio();
	gfxGL = new GraphicsGL();
	gfx2D = new Graphics2D();
	gfx3D = new Graphics3D();
	game->Start();
	game->SetScreen(game->GetStartScreen());

	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));
	while(msg.message != WM_QUIT) {
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)){
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		game->EngineUpdate();
		crossEGL->SwapBuffers();
	}

	game->GetCurrentScreen()->Stop();
	game->Stop();
	Debugger::Release();
	delete gfx3D;
	delete gfx2D;
	delete gfxGL;
	crossEGL->DestroyContext(true);
	delete crossEGL;
	delete audio;
	delete game;
	delete cross::system;

	unsigned long leaked = MemoryManager::Instance()->Dump();
	if(leaked > 0) {
		char buf[256];
		sprintf(buf, "Memory leak.Total bytes = %d\n", leaked);
		OutputDebugString(buf);
		return -1;
	} else {
		OutputDebugString("No memory leak detected\n");
	}
	return 0;
}

#endif