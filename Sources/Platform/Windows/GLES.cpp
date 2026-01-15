#ifdef GLES
#include "GLES.h"
#include "Internals/GraphicsGL.h"
#include "Internals/Audio.h"
#include "Native.h"
#include "Game.h"
#include "Screen.h"
#include "WINSystem.h"
#include "Config.h"
#include "Utils/Debugger.h"
#include "Platform/CrossEGL.h"

#include <time.h>

using namespace cross;

CrossEGL* crossEGL = nullptr;

int GLES_Main(){
#ifdef CROSS_MEMORY_PROFILE
	MemoryManager::dead = false;
#endif // CROSS_MEMORY_PROFILE

	crossEGL = CREATE CrossEGL();
	crossEGL->BindWindow(WinCreate());

	srand((U32)time(0));
	WINSystem* winSys = CREATE WINSystem(crossEGL->GetWindow());
	cross::os = winSys;
	game = CrossMain();
	input->KeyReleased.Connect(winSys, &WINSystem::KeyReleasedHandle);

	int winX = config->GetInt("WIN_POS_X", 100);
	int winY = config->GetInt("WIN_POS_Y", 100);
	int winWidth = config->GetInt("WIN_WIDTH", 960);
	int winHeight = config->GetInt("WIN_HEIGHT", 512);
	winSys->ResizeWindow(winX, winY, winWidth, winHeight);

	crossEGL->CreateContext(true);

	ShowWindow(crossEGL->GetWindow(), TRUE);

	audio = CREATE Audio();
	gfxGL = CREATE GraphicsGL();
	game->Start();

	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));
	while(msg.message != WM_QUIT) {
		while(PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)){
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			if(msg.message == WM_QUIT) break;
		}
		game->EngineUpdate();
		if(!game->IsSuspended()) {
			crossEGL->SwapBuffers();
		}
	}

	game->GetCurrentScreen()->Stop();
	game->Stop();
	delete gfxGL;
	crossEGL->DestroyContext(true);
	delete crossEGL;
	delete audio;
	delete game;
	delete cross::os;
	Debugger::Release();
#ifdef CROSS_MEMORY_PROFILE
	MemoryManager::Instance()->Dump();
	MemoryManager::dead = true;
#endif // CROSS_MEMORY_PROFILE
	return 0;
}

#endif