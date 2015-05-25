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
	
#include "Snake.h"

const float Snake::face_radius = 26.f;
const float Snake::body_radius = 26.f;
const float Snake::speedV = 240.f;
const float Snake::speedW = speedV / 1.33f;
const float Snake::nod_length = 50.f;
const float Snake::star_speedW = 2.f;
Game* Snake::game = NULL;
Graphics* Snake::graphics = NULL;

Snake::Snake(Game* game){
	body_length = 150.f;
	Snake::game = game;
	Snake::graphics = game->graphics;
	body_img = graphics->LoadImage("Game/Body.png");
	star_img = graphics->LoadImage("Game/Star.png");
	face_dead = graphics->LoadImage("Game/Face/FaceDead.png");

	face_pos.x = 200;
	face_pos.y = 400;
	Image* framesBottom[16];
	framesBottom[0] = graphics->LoadImage("Game/Face/00Bottom.png");
	framesBottom[1] = graphics->LoadImage("Game/Face/01Bottom.png");
	framesBottom[2] = graphics->LoadImage("Game/Face/02Bottom.png");
	framesBottom[3] = graphics->LoadImage("Game/Face/03Bottom.png");
	framesBottom[4] = graphics->LoadImage("Game/Face/04Bottom.png");
	framesBottom[5] = graphics->LoadImage("Game/Face/05Bottom.png");
	framesBottom[6] = graphics->LoadImage("Game/Face/06Bottom.png");
	framesBottom[7] = framesBottom[5];
	framesBottom[8] = framesBottom[4];
	framesBottom[9] = framesBottom[3];
	framesBottom[10] = framesBottom[2];
	framesBottom[11] = framesBottom[1];
	framesBottom[12] = framesBottom[0];
	framesBottom[13] = framesBottom[0];
	framesBottom[14] = framesBottom[0];
	framesBottom[15] = framesBottom[0];

	Image* framesTop[16];
	framesTop[0] = graphics->LoadImage("Game/Face/00Top.png");
	framesTop[1] = graphics->LoadImage("Game/Face/01Top.png");
	framesTop[2] = graphics->LoadImage("Game/Face/02Top.png");
	framesTop[3] = graphics->LoadImage("Game/Face/03Top.png");
	framesTop[4] = graphics->LoadImage("Game/Face/04Top.png");
	framesTop[5] = graphics->LoadImage("Game/Face/05Top.png");
	framesTop[6] = graphics->LoadImage("Game/Face/06Top.png");
	framesTop[7] = framesTop[5];
	framesTop[8] = framesTop[4];
	framesTop[9] = framesTop[3];
	framesTop[10] = framesTop[2];
	framesTop[11] = framesTop[1];
	framesTop[12] = framesTop[0];
	framesTop[13] = framesTop[0];
	framesTop[14] = framesTop[0];
	framesTop[15] = framesTop[0];

	face_bottom_anim = new Animation(graphics, 10.f / speedV, framesBottom, 16);
	face_top_anim = new Animation(graphics, 10.f / speedV, framesTop, 16);

	if(game->saver->LoadBool(KEY_SOUND)){
		eat_snd = new Audio("Game/Eat.wav", false, false);
	}else{
		eat_snd = NULL;
	}
	
	body_length = 150.f;
	body_path.push_back(PointX(0, 400));
	face_angle = 0;
}

bool Snake::OnCollision(PointX center, float radius){
	for(PointX node : body_nodes){
		if(CircleOnCollision(node, body_radius, center, radius))
			return true;
	}
	if(CircleOnCollision(face_pos, face_radius, center, radius))
		return true;
	return false;
}

void Snake::EatableNear(Eatable* eatable){
	if(CircleOnCollision(face_pos, face_radius, eatable->GetPosition(), eatable->GetRadius() + 60.f)) {
		if(!face_bottom_anim->IsRunning()) {
			face_bottom_anim->Start();
			face_top_anim->Start();
			if(eat_snd != NULL)
				eat_snd->Play();
		}
		if(CircleOnCollision(face_pos, face_radius, eatable->GetPosition(), 1.f)) {
			apple_time_left = 0.025f;
			near_eatable = eatable;
		}
	}
}

void Snake::DrawFace(float sec){
	Image* face = face_bottom_anim->GetImage();
	graphics->Rotate(face, face_angle + 90.f);
	graphics->DrawImage(face_pos, face);
	if(apple_time_left > 0){
		near_eatable->Draw();
		apple_time_left -= sec;
		if(apple_time_left < 0){
			big_nodes.push_back(1);
			GameScreen* gameScr = (GameScreen*)game->GetCurrentScreen();
			if(Apple* apple = dynamic_cast<Apple*>(near_eatable)){
				switch (apple->GetState())
				{
				case AppleState::FRESH:
					gameScr->AddScore(2);
					break;
				case AppleState::ROT:
					gameScr->AddScore(1);
					break;
				default:
					break;
				}
				apple->SetLifeTime(0);
				int roll = rand() % 101;
				if(roll > 25){
					gameScr->StartSpider();
				}
				return;
			}
			if(Spider* spider = dynamic_cast<Spider*>(near_eatable)){
				gameScr->AddScore(3);
				spider->SetState(SpiderState::HIDING);
			}
		}
	}
	face = face_top_anim->GetImage();
	graphics->Rotate(face, face_angle + 90.f);
	graphics->DrawImage(face_pos, face);
}

void Snake::DrawFaceDeadST0(){
	graphics->Rotate(face_dead, face_angle + 90.f);
	graphics->DrawImage(face_pos, face_dead);
}

void Snake::DrawFaceDeadST1(float sec){
	DrawFaceDeadST0();
	PointX stPos;
	for(int i = 0; i < 6; i++){
		stPos.x = (float)(face_radius * cos(star_angle + i * 72.f / 180.f * PI));
		stPos.y = (float)(face_radius * sin(star_angle + i * 72.f / 180.f * PI));
		stPos.x = face_pos.x;
		stPos.y = face_pos.y;
		graphics->DrawImage(stPos, star_img);
	}
	star_angle += star_speedW * sec;
}

void Snake::DrawBody(float sec){
	body_path.insert(body_path.begin(), face_pos);
	body_nodes.clear();
	CalcBigNodes(sec);
	//drawing body nodes
	float pathLen = 0;
	int nodIndex = 1;
	for(unsigned int i = 0; i < body_path.size() - 1; i++){
		PointX curr = body_path[i];
		PointX next = body_path[i + 1];
		//length between two path points
		float len = Distance(curr, next);
		pathLen += len;
		//if need to draw node on this path line
		if(pathLen > nod_length * nodIndex){
			float l = abs(pathLen - len - nod_length * nodIndex);
			float k = l/len;
			while( k < 1 ){
				float x = curr.x + (next.x - curr.x) * k;
				float y = curr.y + (next.y - curr.y) * k;
				if(nodIndex <= body_length / nod_length){
					body_nodes.push_back(PointX(x, y));
					bool bigNode = false;
					for(unsigned int j = 0; j < big_nodes.size(); j++){
						int appNode = big_nodes[j];
						if(appNode == nodIndex){
							graphics->ScaleImage(body_img, game->GetScaleFactor() * 1.2f);
							graphics->DrawImage(x, y, body_img);
							graphics->ScaleImage(body_img, game->GetScaleFactor());
							bigNode = true;
						}
					}
					if(!bigNode)
						graphics->DrawImage(x, y, body_img);
				}
				nodIndex++;
				l += nod_length;
				k = l / len;
			}
		}
		//remove unused path
		if(pathLen > body_length){
			while (body_path.size() - 2 > i){
				body_path.erase(body_path.begin() + i + 2);
			}
		}
	}
}

void Snake::CalcHead(float sec){
	face_bottom_anim->Update(sec);
	face_top_anim->Update(sec);
	face_pos.x += speedV * cos(face_angle / 180.f * PI) * sec;
	face_pos.y += -speedV * sin(face_angle / 180.f * PI) * sec;
}

void Snake::CalcBigNodes(float sec){
	if(body_time_left <= 0){
		for(auto it = big_nodes.begin(); it != big_nodes.end(); it++){
			int bigNode = (*it);
			if(bigNode < body_length / nod_length){
				bigNode++;
				(*it) = bigNode;
			}else{
				big_nodes.erase(it++);
				body_length += nod_length;
			}
		}
		body_time_left = 0.3f;
	}else{
		body_time_left -= sec;
	}
}

bool Snake::OnBiteYouself(){
	for(unsigned int i = 1; i < body_nodes.size(); i++){
		PointX p = body_nodes[i];
		if(CircleOnCollision(p, body_radius, face_pos, face_radius)){
			return true;	
		}
	}
	return false;
}

bool Snake::OnBorder(){
	if( face_pos.x - face_radius < 0 ) {
		face_pos.x = face_radius;
		return true;
	}
	if( face_pos.x + face_radius > game->GetWidth()) {
		face_pos.x = game->GetWidth() - face_radius;
		return true;
	}
	if( face_pos.y - face_radius < 0 ) {
		face_pos.y = face_radius;
		return true;
	}
	if( face_pos.y + face_radius > game->GetHeight()) {
		face_pos.y = game->GetHeight() - face_radius;
		return true;
	}
	return false;
}

float Snake::GetSpeedW(){
	return speedW;
}