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
	
#include "GameScreen.h"
#include "Game.h"
#include <stdlib.h>
#include <time.h>
#include <math.h>

//#define VW 800 //Screen Width
//#define VH 600 //Screen Height
#define OFFX 150 //Offset x
#define OFFY 100 //Offset y
//#define RW 1200 //Room Width
//#define RH 800 //Room Height
#define PN 0 //This is the id of the playable character playerx[PN] is the x coordinate of the player you control

GameScreen::GameScreen(Game* game):Screen(game),
	playernumber( 0 ),
	pause( false ),
	shiftpressed( false ),
	enterpressed( false ),
	backr( 0 ),
	backg( 0 ),
	backb( 0 ),
	realbackr( 0 ),
	realbackg( 0 ),
	realbackb( 0 ),
	dangertimer( 0 ),
	spawntimer( 0 ),
	turn( 0 ),
	turntimer( 0 ),
	deathtimer( 0 ),
	pNum( 10 ){

}

void GameScreen::Start(){
}

void GameScreen::Update(float sec){
	graphics->Clear(.25f, .25f, .25f);
	if (playerhp[PN]<=0){
		deathtimer++;
		if (deathtimer>120){
			//restartgame();
		}
	}else{
		deathtimer=0;
	}
	//gfx.vx=max(0,min(RW-VW,gfx.vx));
	//gfx.vy=max(0,min(RH-VH,gfx.vy));
	if (!pause){
		for(int xx=0;xx<playernumber;xx++){
			if (playerhp[xx]>0){
				//updateplayer(xx);
			}
		}
	}
	if (!(input->HaveKey() && input->GetKey() == Key::ENTER)){//kbd.EnterIsPressed())
		enterpressed=false;
	}
	if ((input->HaveKey() && input->GetKey() == Key::ENTER) && enterpressed==false){
		enterpressed=true;
		if (!pause){
			pause=true;
		}else{
			pause=false;
		}
	}
	//gfx.BeginFrame();
	ComposeFrame();
	//gfx.EndFrame();

	int checkdead=0;
	
	for(int xx=1;xx<playernumber;xx++)
	{
		if (playerhp[xx]>0)
		{
			checkdead=1;
		}
	}
	spawntimer++;
	if (checkdead==1)
	{
		spawntimer=0;
	}
	/*
	if (checkdead==0 && spawntimer>120)
	{
		playernumber=1;
		//srand( (unsigned int)time(NULL) );
		for( int xx=0;xx<pNum;xx++)
		{
			backr=random(255);
			backg=random(255);
			backb=random(255);
			createplayer(random(RW - 24),random(RH - 24),100);
		}
		pNum+=2;
	}*/
}

int GameScreen::random(int number)
{
	return(rand() % (number));
}

void GameScreen::ComposeFrame()
{
	//gfx.vx=max(0,min(RW-VW,gfx.vx));
	//gfx.vy=max(0,min(RH-VH,gfx.vy));
	dangertimer++;
	if (playerhp[PN]<playerhpmax[PN]/4)
	{
		if (dangertimer>100)
		{
			dangertimer=0;
			realbackr=backr;	realbackg=backg;	realbackb=backb;
		}
		
		realbackg=max(0,realbackg-12);
		realbackb=max(0,realbackb-12);
	}
	if (!(realbackr==backr && realbackg==backg && realbackb==backb))
	for(int xx=0;xx<3;xx++)
	{
		if (realbackr>backr)		{			realbackr--;		}
		if (realbackr<backr)		{			realbackr++;		}
		if (realbackg>backg)		{			realbackg--;		}
		if (realbackg<backg)		{			realbackg++;		}
		if (realbackb>backb)		{			realbackb--;		}
		if (realbackb<backb)		{			realbackb++;		}

	}
	
	for(int xx=0;xx<game->GetHeight();xx+=48)
	{
		//gfx.DrawRect(0,xx,RW,xx+48,(int)(((float)xx/(float)RH)*realbackr),realbackg,(int)(((float)xx/(float)RH)*realbackb),false);
		Color c((int)((xx/game->GetHeight())*realbackr),realbackg,(int)((xx/game->GetHeight())*realbackb));
		Rect r(0, xx, game->GetWidth(), xx+48);
		graphics->DrawRect(r, c, true);
		for(int xy=0;xy<game->GetWidth();xy+=48)
		{
			//gfx.DrawRect(xy,xx,xy+24,xx+24,255-realbackr,255-realbackg,abs((int)(((float)xx/(float)RH)*255)-realbackb),false);
			Color c(255-realbackr,255-realbackg,abs((int)((xx/game->GetHeight())*255)-realbackb));
			graphics->DrawRect(Rect(xy, xx, 24, 24), c, true);
		}
	}

	//graphics->DrawRect(Rect(0, 0, 24, 24), Color::Red, true);

	for( int xx=0;xx<playernumber;xx++)
	{
		if (playerhp[xx]>0)
		{
			//drawplayer(xx);
		}
	}
	for(int xx=0;xx<23;xx++)
	{
		//gfx.DrawRect(gfx.vx,gfx.vy+xx,gfx.vx+RW,gfx.vy+xx+1,(255-xx*3)/2,(255-xx*3)/2,(255-xx*3)/2,false);
		//gfx.DrawRect(gfx.vx,gfx.vy+xx,gfx.vx+(int)((float)playerhp[PN]/(float)playerhpmax[PN]*RW),gfx.vy+1+xx,255-xx*3,0,0,false);
	}
	//gfx.DrawRect(gfx.vx,gfx.vy,gfx.vx+RW,gfx.vy+24,0,0,0,true);
}