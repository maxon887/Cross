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
	srand( (unsigned int)time(NULL) );
	backr=random(255);
	backg=random(255);
	backb=random(255);
	realbackr=backr;	realbackg=backg;	realbackb=backb;
	for( int xx=0;xx<pNum;xx++)
	{
		createplayer(random(game->GetWidth() - 24),random(game->GetHeight() - 24),100);
	}
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
				updateplayer(sec, xx);
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
	/*
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
	}*/
	
	for(int xx=0;xx<game->GetHeight();xx+=48)
	{
		Rect r(0, xx, game->GetWidth(), xx+48);
		graphics->DrawRect(r, Color::Black, true);
		for(int xy=0;xy<game->GetWidth();xy+=48)
		{
			graphics->DrawRect(Rect(xy, xx, 24, 24), Color::Yellow, true);
		}
	}

	for( int xx=0;xx<playernumber;xx++)
	{
		if (playerhp[xx]>0)
		{
			drawplayer(xx);
		}
	}
	for(int xx=0;xx<23;xx++){
		graphics->DrawRect(Rect(0, 0, game->GetWidth(), xx+1), Color((255-xx*3)/2,(255-xx*3)/2,(255-xx*3)/2), true);
		graphics->DrawRect(Rect(0, 0, (int)((float)playerhp[PN]/(float)playerhpmax[PN]*game->GetWidth()), 1+xx), Color(255-xx*3,0,0), true);
	}
	//gfx.DrawRect(gfx.vx,gfx.vy,gfx.vx+RW,gfx.vy+24,0,0,0,true);
}

//PLAYER FUNCTIONS BEGIN
void GameScreen::createplayer(int x,int y,int hp)
{
	if (playernumber<450)
	{
		//set variables
		playerhp[playernumber]=hp;
		playerx[playernumber]=x;
		playery[playernumber]=y;
		playeratkn[playernumber]=0;
		playerrange[playernumber]=60;
		playeratkspd[playernumber]=60;
		kbdu[playernumber]=0;
		kbdd[playernumber]=0;
		kbdr[playernumber]=0;
		kbdl[playernumber]=0;
		aipause[playernumber]=0;
		aiwarmup[playernumber]=0;
		aidisttoPN[playernumber]=0;
		aggro[playernumber]=false;
		lastdir[playernumber]=0;
	
		if (playernumber==PN)
		{
			playerhp[playernumber]=5000;
		}
		else
		{
			playeratkspd[playernumber]=60;
		}

		//increase number
		playerhpmax[playernumber]=playerhp[playernumber];
		playernumber++;
	}
}


void GameScreen::drawplayer(int pID)
{
	//ai helth
	if (aidisttoPN[pID]<100 && !pID==PN)
	{
		for(int xx=0;xx<7;xx++)
		{
			//gfx.DrawRect(playerx[pID]+12-32,playery[pID]-16+xx,playerx[pID]+12+32,playery[pID]-15+xx,(255-xx*3)/2,(255-xx*3)/2,(255-xx*3)/2,false);
			Color c1((255-xx*3)/2,(255-xx*3)/2,(255-xx*3)/2);
			graphics->DrawRect(Rect(playerx[pID] + 12 -32, playery[pID]-16+xx, 64, 1), c1, true);
			//gfx.DrawRect(playerx[pID]+12-32,playery[pID]-16+xx,playerx[pID]+12-32+(int)((float)playerhp[pID]/(float)playerhpmax[pID]*64),playery[pID]-15+xx,(255-xx*3),0,0,false);
			Color c2((255-xx*3),0,0);
			graphics->DrawRect(Rect(playerx[pID]+12-32,playery[pID]-16+xx,(int)((float)playerhp[pID]/(float)playerhpmax[pID]*64),1), c2, true);
		}
		//gfx.DrawRect(playerx[pID]+12-32,playery[pID]-16,playerx[pID]+12+32,playery[pID]-8,0,0,0,true);
		graphics->DrawRect(Rect(playerx[pID]+12-32, playery[pID]-16, 64,8), Color::Black, false);
	}

	int r=255;int b=255;
	if (pID==PN)
	{
		r=0;
	}
	if (playeratkn[pID]>0)
	{
		if ((kbdu[pID]+kbdd[pID]+kbdr[pID]+kbdl[pID])>0)
		{
			int newatkspd=playerrange[pID]-(int)(max(0,(float)playeratkn[pID]-(float)playeratkspd[pID]+10)/10*(float)playerrange[pID]);
			if (playeratkn[pID]<10)
			{
				newatkspd=(int)((float)playeratkn[pID]/10*(float)playerrange[pID]);
			}
			for(int yy=0;yy<2;yy++)
			{
				for(int xx=0;xx<6;xx++)
				{
					//gfx.DrawLine(playerx[pID]+12+xx-3,playery[pID]+12+yy-3,playerx[pID]+12+((kbdr[pID]-kbdl[pID])*newatkspd),playery[pID]+12+((kbdd[pID]-kbdu[pID])*newatkspd),255,255,0);
					graphics->DrawLine(Point(playerx[pID]+12+xx-3,playery[pID]+12+yy-3), Point(playerx[pID]+12+((kbdr[pID]-kbdl[pID])*newatkspd),playery[pID]+12+((kbdd[pID]-kbdu[pID])*newatkspd)), Color(255,255,0));
				}
			}
		}
	}
	//gfx.DrawRect(playerx[pID],playery[pID],playerx[pID]+24,playery[pID]+24,r,b,255,false);
	graphics->DrawRect(Rect(playerx[pID],playery[pID], 24, 24), Color(r,b,255), true);
	//gfx.DrawRect(playerx[pID],playery[pID],playerx[pID]+24,playery[pID]+24,0,0,0,true);
	graphics->DrawRect(Rect(playerx[pID],playery[pID], 24, 24), Color::Black, false);
}


void GameScreen::updateplayer(float sec, int pID)
{
	playerhp[pID] = min(playerhpmax[pID],max(0,playerhp[pID]));
	playeratkn[pID] = max(0,playeratkn[pID]-1);
	if (turn==pID || 1==1)
	{
	bool dothis = false;
	aipause[pID]= max(0,aipause[pID]-1);
	for(int atkerxx=0;atkerxx<playernumber;atkerxx++)
	{
		for(int victimxx=0;victimxx<playernumber;victimxx++)
		{
			if( playerx[atkerxx]+playerrange[atkerxx]> playerx[victimxx]   &&
				playerx[atkerxx]< playerx[victimxx] + playerrange[atkerxx] &&
				playery[atkerxx]+playerrange[atkerxx]> playery[victimxx]   &&
				playery[atkerxx]< playery[victimxx] + playerrange[atkerxx] &&
				!(atkerxx==victimxx)									   &&
				playerhp[atkerxx]>0)
			{
				if (((atkerxx==PN) || (victimxx==PN)) && playeratkn[atkerxx]>0)
				{
					playerhp[victimxx]=max(0,playerhp[victimxx]-1);
				}
				else
				{
					dothis=true;
				}
			}
			if( playerx[atkerxx]+24> playerx[victimxx]   &&
				playerx[atkerxx]< playerx[victimxx] + 24 &&
				playery[atkerxx]+24> playery[victimxx]   &&
				playery[atkerxx]< playery[victimxx] + 24 &&
				!(atkerxx==victimxx)					 &&
				playerhp[atkerxx]>0						 &&
				dothis)
			{
				if (playerhp[atkerxx]>0 && playerhp[victimxx]>0)
				{
					if (playerx[atkerxx]>playerx[victimxx])
					{
						playerx[atkerxx]++;
					}
					if (playerx[atkerxx]<playerx[victimxx])
					{
						playerx[atkerxx]--;
					}
					if (playery[atkerxx]>playery[victimxx])
					{
						playery[atkerxx]++;
					}
					if (playery[atkerxx]<playery[victimxx])
					{
						playery[atkerxx]--;
					}
				}

			}
		}
	}
	float spd=5 * sec * 50;
	aggro[pID]=false;
	if (pID==PN)
	{
		/*
		for(int xx=0;xx<5;xx++)
		{
			if (playerx[PN]>(gfx.vx+VW/2+OFFX))
			{
				gfx.vx++;
			}
			if (playerx[PN]<(gfx.vx+VW/2-OFFX))
			{
				gfx.vx--;
			}
			if (playery[PN]>(gfx.vy+VH/2+OFFY))
			{
				gfx.vy++;
			}
			if (playery[PN]<(gfx.vy+VH/2-OFFY))
			{
				gfx.vy--;
			}
		}*/
		if (!shiftpressed)
		{
			if (input->HaveKey() && (input->key_key == Key::SPACE) && playeratkn[PN]==0)
			{
				shiftpressed=true;
				playeratkn[PN]=playeratkspd[PN];
				turntimer+=20;
			}
		}
		if (!(input->HaveKey() && (input->key_key == Key::SPACE)))
		{
			shiftpressed=false;
		}
	}else{
		int aa;
		int bbb;
		aa=abs( playerx[PN]-playerx[pID] );
		bbb=abs( playery[PN]-playery[pID] );
		aidisttoPN[pID]=min(aa,bbb);
		if (playeratkn[pID]==0)
		{
			if (aidisttoPN[pID]<98 && aipause[pID]==0)
			{
				aiwarmup[pID]++;
			}
			else
			{
				aiwarmup[pID]=0;
				aggro[pID]=true;
			}
			if (aiwarmup[pID]>60)
			{
				aiwarmup[pID]=0;
				playeratkn[pID]=playeratkspd[pID];
				turntimer+=20;
			}
		}
	}
	if (playeratkn[pID]==0)
	{
		kbdl[pID]=0;kbdr[pID]=0;kbdd[pID]=0;kbdu[pID]=0;
		if (pID==PN && input->HaveKey())
		{
			switch (input->GetKey())
			{
			case Key::UP:
				kbdu[pID]=1;
				break;
			case Key::DOWN:
				kbdd[pID]=1;
				break;
			case Key::LEFT:
				kbdl[pID]=1;
				break;
			case Key::RIGHT:
				kbdr[pID]=1;
				break;
			default:
				break;
			}
		}
		else
		{
			if ((aidisttoPN[pID]<200 && aipause[pID]==0 && aiwarmup[pID]==0) || playeratkn[pID]>0 || 1==1)
			{
				if (playerx[PN]<playerx[pID])				{					kbdl[pID]=1;				}
				if (playerx[PN]>playerx[pID])				{					kbdr[pID]=1;				}
				if (playery[PN]<playery[pID])				{					kbdu[pID]=1;				}
				if (playery[PN]>playery[pID])				{					kbdd[pID]=1;				}
			}
		}
	}
	if (kbdl[pID]+kbdr[pID]+kbdu[pID]+kbdd[pID]>1 && pID==PN){spd=3 * sec * 50;}
	if (kbdl[pID]+kbdr[pID]+kbdu[pID]+kbdd[pID]>2 && pID==PN){spd=0;}
	if (kbdl[pID]+kbdr[pID]+kbdu[pID]+kbdd[pID]==0 && playeratkn[pID]>0)
	{
		switch(lastdir[pID])
		{
		case 0:
				  kbdr[pID]=1;
				  break;
		case 180:
				  kbdl[pID]=1;
				  break;
		case 270:
				  kbdd[pID]=1;
				  break;
		case 90:
				  kbdu[pID]=1;
				  break;
		}
	}
	if ((!pID==PN) && aipause[pID]>0){
		spd=min(spd,1 * sec * 50);
	}
	if (playeratkn[pID]>0)
	{
		spd = 1 * sec * 50;
		if (playeratkn[pID]>(float)(playeratkspd[pID]/1.75)){spd=2* sec * 50;}
		if (playeratkn[pID]>(float)(playeratkspd[pID]/1.5)){spd=3* sec * 50;}
		if (playeratkn[pID]>(float)(playeratkspd[pID]/1.25)){spd=4* sec * 50;}
		if (playeratkn[pID]<(float)(playeratkspd[pID]/5)){spd=0;}
		if (playeratkn[pID]==1){aipause[pID]=30;}
	}
	if (!pID==PN)
	{
		spd/=2;
	}
	if (kbdr[pID]==1){
		lastdir[pID]=0;
		playerx[pID]=min(game->GetWidth()-50,playerx[pID]+spd);
	}
	if (kbdl[pID]==1){
		lastdir[pID]=180;
		playerx[pID]=max(50,playerx[pID]-spd);
	}
	if (kbdd[pID]==1){
		lastdir[pID]=270;
		playery[pID]=min(game->GetHeight()-50,playery[pID]+spd);
	}
	if (kbdu[pID]==1){
		lastdir[pID]=90;
		playery[pID]=max(50,playery[pID]-spd);
	}
	if (kbdr[pID]+kbdl[pID]+kbdd[pID]+kbdu[pID]>0 && spd>0){
		turntimer++;
	}
	if (turntimer>90)
	{
		nextturn();
		turntimer=0;
	}
}}

void GameScreen::nextturn()
{
	turn++;
	if (turn>=playernumber)
	{
		turn=0;
	}
}