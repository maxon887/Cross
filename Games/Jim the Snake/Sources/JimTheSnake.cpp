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
	
#include "JimTheSnake.h"
#include "GameScreen.h"
#include "MenuScreen.h"
#include "Debuger.h"

#define GAME_WIDTH 900

#define PROPERTY_SCORE 		"SCORE"
#define PROPERTY_MUSIC 		"MUSIC"
#define PROPERTY_SOUND 		"SOUND"
#define PROPERTY_CONTROL	"CONTROL"
#define PROPERTY_PURCHASED 	"PURCHASED"

using namespace std::placeholders;

JimTheSnake::JimTheSnake(Launcher* launcher):Game(launcher, GAME_WIDTH){

}

void JimTheSnake::Start(){
	launcher->LogIt("SnakyGame->Start()");
	music = saver->LoadBool(PROPERTY_MUSIC, true);
	sound = saver->LoadBool(PROPERTY_SOUND, true);
	score = saver->LoadInt(PROPERTY_SCORE, 0);
	control = (Control)saver->LoadInt(PROPERTY_CONTROL, SLIDE);
	purchased = saver->LoadBool(PROPERTY_PURCHASED, false);
	commercial = launcher->GetCommercial();
	graphics->SetPrimitiveEnable(false);
	if(commercial != NULL){
		commercial->RegisterCallback(bind(&JimTheSnake::CommercialCallback, this, _1));
	}
	Game::Start();
}

Screen* JimTheSnake::GetStartScreen(){
	return new MenuScreen(this);
}

bool JimTheSnake::IsMusicEnabled(){
	return music;
}
bool JimTheSnake::IsSoundEnabled(){
	return sound;
}
int JimTheSnake::BestScore(){
	return score;
}

Control JimTheSnake::GetControl(){
	return control;
}

bool JimTheSnake::IsPurchased(){
	return purchased;
}

void JimTheSnake::SetMusicEnabled(bool enabled){
	saver->SaveBool(PROPERTY_MUSIC, enabled);
	music = enabled;
}
void JimTheSnake::SetSoundEnabled(bool enabled){
	saver->SaveBool(PROPERTY_SOUND, enabled);
	sound = enabled;
}
void JimTheSnake::SetBestScore(int best){
	saver->SaveInt(PROPERTY_SCORE, best);
	score = best;
}
void JimTheSnake::SetControl(Control control){
	saver->SaveInt(PROPERTY_CONTROL, control);
	this->control = control;
}

Commercial* JimTheSnake::GetCommercial(){
	return commercial;
}

void JimTheSnake::CommercialCallback(Commercial::Event e){
	switch (e){
	case Commercial::AD_LOADED:
		launcher->LogIt("Ad loaded");
		break;
	case Commercial::AD_LOAD_FAILED:
		launcher->LogIt("Ad failed to load");
		break;
	case Commercial::PURCHASE_COMPLETE:
		launcher->LogIt("Purchase complete");
        saver->SaveBool(PROPERTY_PURCHASED, true);
        purchased = true;
		break;
	case Commercial::PURCHASE_CANCELED:
		launcher->LogIt("Purchase canceled");
		break;
	case Commercial::PURCHASE_FAILED:
		launcher->LogIt("Purchase failed");
		break;
	default:
		break;
	}
}
