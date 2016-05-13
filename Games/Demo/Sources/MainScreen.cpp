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
#include "MainScreen.h"
#include "Graphics2D.h"
#include "Launcher.h"
#include "Sprite.h"
#include "AudioScreen.h"
#include "Camera2D.h"
#include "Graphics3D/TriangleScreen.h"
#include "Graphics3D/SolidModelScreen.h"
#include "Graphics3D/TexturedModelScreen.h"
#include "Graphics3D/MaterialScreen.h"
#include "Graphics3D/DiffuseMapScreen.h"
#include "Graphics3D/DiffuseSpecularMapScreen.h"
#include "Graphics3D/DirectionalLightScreen.h"
#include "Graphics3D/PointLightScreen.h"
#include "Graphics3D/SpotLightScreen.h"
#include "Graphics3D/MultiLightScreen.h"
#include "Graphics3D/NanosuitScreen.h"
#include "Graphics2D/PrimitivesScreen.h"
#include "Graphics2D/AnimationScreen.h"
#include "Graphics2D/SpritesScreen.h"
#include "Graphics2D/TextScreen.h"
#include "Graphics2D/Camera2DScreen.h"
#include "Demo.h"

void MainScreen::Start(){
	ScrollScreen::Start();
	next_screen = nullptr;
	button_sprite = demo->GetCommonSprite("DefaultButton.png");
	button_sprite->SetScale(1.35f);
	//main menu
	main_menu = new Menu(true);
	current_menu = main_menu;
	Button* graphics2Dbtn = new Button("Graphics 2D");
	Button* graphics3Dbtn = new Button("Graphics 3D");
	Button* audioBtn = new Button("Audio");
	graphics2Dbtn->SetImages(button_sprite->Clone(), nullptr);
	graphics3Dbtn->SetImages(button_sprite->Clone(), nullptr);
	audioBtn->SetImages(button_sprite->Clone(), nullptr);
	graphics2Dbtn->Clicked += MakeDelegate(this, &MainScreen::OnGraphics2DClick);
	graphics3Dbtn->Clicked += MakeDelegate(this, &MainScreen::OnGraphics3DClick);
	audioBtn->Clicked += MakeDelegate(this, &MainScreen::OnAudioClick);
	main_menu->AddButton(graphics2Dbtn);
	main_menu->AddButton(graphics3Dbtn);
	main_menu->AddButton(audioBtn);
	//graphics 2D menu
	graphics2D_menu = new Menu(false);
	Button* primitivesBtn = new Button("Primitives");
	Button* spritesBtn = new Button("Sprites");
	Button* textBtn = new Button("Text Drawing");
	Button* canvasBtn = new Button("2D Camera");
	Button* animationBtn = new Button("Animation");
	primitivesBtn->SetImages(button_sprite->Clone());
	spritesBtn->SetImages(button_sprite->Clone());
	textBtn->SetImages(button_sprite->Clone());
	canvasBtn->SetImages(button_sprite->Clone());
	animationBtn->SetImages(button_sprite->Clone());
	primitivesBtn->Clicked += MakeDelegate(this, &MainScreen::OnPrimitivesClick);
	spritesBtn->Clicked += MakeDelegate(this, &MainScreen::OnSpritesClick);
	canvasBtn->Clicked += MakeDelegate(this, &MainScreen::On2DCameraClick);
	animationBtn->Clicked += MakeDelegate(this, &MainScreen::OnAnimationClick);
	textBtn->Clicked += MakeDelegate(this, &MainScreen::OnTextClick);
	graphics2D_menu->AddButton(primitivesBtn);
	graphics2D_menu->AddButton(spritesBtn);
	graphics2D_menu->AddButton(textBtn);
	graphics2D_menu->AddButton(canvasBtn);
	graphics2D_menu->AddButton(animationBtn);
	//graphics 3D menu
	graphics3D_menu = new Menu(true);
	Button* simpleBtn				= new Button("Simple");
	Button* lightBtn				= new Button("Light");
	simpleBtn->SetImages(button_sprite->Clone());
	lightBtn->SetImages(button_sprite->Clone());
	simpleBtn->Clicked += MakeDelegate(this, &MainScreen::OnSimpleClick);
	lightBtn->Clicked += MakeDelegate(this, &MainScreen::OnLightClick);
	graphics3D_menu->AddButton(simpleBtn);
	graphics3D_menu->AddButton(lightBtn);

	graphics3D_simple = new Menu(true);
	Button* triangleBtn				= new Button("Triangle");
	Button* solidModelBtn			= new Button("Solid Model");
	Button* texturedModelBtn		= new Button("Textured Model");
	Button* nanosuitBtn				= new Button("Nanosuit");
	triangleBtn->SetImages(button_sprite->Clone());
	solidModelBtn->SetImages(button_sprite->Clone());
	texturedModelBtn->SetImages(button_sprite->Clone());
	nanosuitBtn->SetImages(button_sprite->Clone());
	triangleBtn->Clicked += MakeDelegate(this, &MainScreen::OnTriangleClick);
	solidModelBtn->Clicked += MakeDelegate(this, &MainScreen::OnSolidModelClick);
	texturedModelBtn->Clicked += MakeDelegate(this, &MainScreen::OnTexturedModelClick);
	nanosuitBtn->Clicked += MakeDelegate(this, &MainScreen::OnNanosuitClick);
	graphics3D_simple->AddButton(triangleBtn);
	graphics3D_simple->AddButton(solidModelBtn);
	graphics3D_simple->AddButton(texturedModelBtn);
	graphics3D_simple->AddButton(nanosuitBtn);

	graphics3D_light = new Menu(false);
	Button* materialBtn 			= new Button("Material");
	Button* diffuseMap				= new Button("Diffuse Map");
	Button* diffuseSpecularMapBtn	= new Button("Diffuse Specular Map");
	Button* directionalLigthBtn		= new Button("Directional Light");
	Button* pointLightBtn			= new Button("Point Light");
	Button* spotLightBtn			= new Button("Spot Light");
	Button* multiLightBtn			= new Button("Multi-Ligth");
	materialBtn->SetImages(button_sprite->Clone());
	diffuseMap->SetImages(button_sprite->Clone());
	diffuseSpecularMapBtn->SetImages(button_sprite->Clone());
	directionalLigthBtn->SetImages(button_sprite->Clone());
	pointLightBtn->SetImages(button_sprite->Clone());
	spotLightBtn->SetImages(button_sprite->Clone());
	multiLightBtn->SetImages(button_sprite->Clone());
	materialBtn->Clicked += MakeDelegate(this, &MainScreen::OnMaterialClick);
	diffuseMap->Clicked += MakeDelegate(this, &MainScreen::OnDiffuseMapClick);
	diffuseSpecularMapBtn->Clicked += MakeDelegate(this, &MainScreen::OnDiffuseSpecularMapClick);
	directionalLigthBtn->Clicked += MakeDelegate(this, &MainScreen::OnDirectionalLight);
	pointLightBtn->Clicked += MakeDelegate(this, &MainScreen::OnPointLightClick);
	spotLightBtn->Clicked += MakeDelegate(this, &MainScreen::OnSpotLightClick);
	multiLightBtn->Clicked += MakeDelegate(this, &MainScreen::OnMultiLightClick);
	graphics3D_light->AddButton(materialBtn);
	graphics3D_light->AddButton(diffuseMap);
	graphics3D_light->AddButton(diffuseSpecularMapBtn);
	graphics3D_light->AddButton(directionalLigthBtn);
	graphics3D_light->AddButton(pointLightBtn);
	graphics3D_light->AddButton(spotLightBtn);
	graphics3D_light->AddButton(multiLightBtn);

	graphics2D_menu->Active(false);
	graphics3D_menu->Active(false);

	key_released_delegate = MakeDelegate(this, &MainScreen::KeyReleasedHandle);
	input->KeyPressed += key_released_delegate;

	window_resized_delegate = MakeDelegate(this, &MainScreen::WindowResizedHandle);
	game->WindowResized += window_resized_delegate;

	delete button_sprite;
	//expirimental
}

void MainScreen::Stop(){
	ScrollScreen::Stop();
	delete main_menu;
	delete graphics2D_menu;
	delete graphics3D_menu;
	delete graphics3D_simple;
	delete graphics3D_light;
	input->KeyPressed -= key_released_delegate;
	game->WindowResized -= window_resized_delegate;
}

void MainScreen::Update(float sec){
	current_menu->Update(sec);

	if(next_screen){
		gfx2D->GetCamera()->SetPosition(Vector2D(0.f, 0.f));
		game->SetScreen(next_screen);
	}
}

void MainScreen::AdjustScreenHeight(Menu* menu){
	SetHeight(menu->GetHeight());
	Vector2D camPos(0.f, 0.f);
	camPos.y = GetHeight() - gfx2D->GetCamera()->GetViewHeight();
	gfx2D->GetCamera()->SetPosition(camPos);
}

void MainScreen::KeyReleasedHandle(Key key){
	if(current_menu == main_menu){
		launcher->PromtToExit();
	}
	current_menu->Active(false);
	main_menu->Active(true);
	current_menu = main_menu;
	float scrWidth = 1600.f;
	float scrHeight = scrWidth / launcher->GetAspectRatio();
	SetWidth(scrWidth);
	SetHeight(scrHeight);
	gfx2D->GetCamera()->SetPosition(Vector2D(0.f, 0.f));
}

void MainScreen::WindowResizedHandle(int width, int height){
	ResetScreenSize();
}

void MainScreen::OnGraphics2DClick(){
	AdjustScreenHeight(graphics2D_menu);
	current_menu->Active(false);
	graphics2D_menu->Active(true);
	current_menu = graphics2D_menu;
}

void MainScreen::OnGraphics3DClick(){
	AdjustScreenHeight(graphics3D_menu);
	current_menu->Active(false);
	graphics3D_menu->Active(true);
	current_menu = graphics3D_menu;
}

void MainScreen::OnAnimationClick(){
	next_screen = new AnimationScreen();
}

void MainScreen::OnSimpleClick(){
	AdjustScreenHeight(graphics3D_simple);
	current_menu->Active(false);
	graphics3D_simple->Active(true);
	current_menu = graphics3D_simple;
}

void MainScreen::OnLightClick(){
	AdjustScreenHeight(graphics3D_light);
	current_menu->Active(false);
	graphics3D_light->Active(true);
	current_menu = graphics3D_light;
}

void MainScreen::OnPrimitivesClick(){
	next_screen = new PrimitivesScreen();
}

void MainScreen::OnSpritesClick(){
	next_screen = new SpritesScreen();
}

void MainScreen::OnTextClick(){
	next_screen = new TextScreen();
}

void MainScreen::On2DCameraClick(){
	next_screen = new Camera2DScreen();
}

void MainScreen::OnTriangleClick(){
	next_screen = new TriangleScreen();
}

void MainScreen::OnSolidModelClick(){
	next_screen = new SolidModelScreen();
}

void MainScreen::OnTexturedModelClick(){
	next_screen = new TexturedModelScreen();
}

void MainScreen::OnNanosuitClick(){
	next_screen = new NanosuitScreen();
}

void MainScreen::OnAudioClick() {
	next_screen = new AudioScreen();
}

void MainScreen::OnMaterialClick(){
	next_screen = new MaterialScreen();
}

void MainScreen::OnDiffuseMapClick(){
	next_screen = new DiffuseMapScreen();
}

void MainScreen::OnDiffuseSpecularMapClick(){
	next_screen = new DiffuseSpecularMapScreen();
}

void MainScreen::OnDirectionalLight(){
	next_screen = new DirectionalLightScreen();
}

void MainScreen::OnPointLightClick(){
	next_screen = new PointLightScreen();
}

void MainScreen::OnSpotLightClick(){
	next_screen = new SpotLightScreen();
}

void MainScreen::OnMultiLightClick(){
	next_screen = new MultiLightScreen();
}