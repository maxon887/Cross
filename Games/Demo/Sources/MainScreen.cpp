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
#include "Graphics3D/TriangleScene.h"
#include "Graphics3D/SolidModelScene.h"
#include "Graphics3D/TexturedModelScene.h"
#include "Graphics3D/SpecularScene.h"
#include "Graphics3D/SpecularDiffuseScene.h"
#include "Graphics3D/SpecularMapScene.h"
#include "Graphics3D/DirectionalLightScene.h"
#include "Graphics3D/PointLightScene.h"
#include "Graphics3D/SpotLightScreen.h"
#include "Graphics3D/MultiLightScreen.h"
#include "Graphics3D/ComplexModelScene.h"
#include "Graphics3D/DepthTestScreen.h"
#include "Graphics2D/PrimitivesScreen.h"
#include "Graphics2D/AnimationScreen.h"
#include "Graphics2D/SpritesScreen.h"
#include "Graphics2D/TextScreen.h"
#include "Graphics2D/Camera2DScreen.h"
#include "Demo.h"

void MainScreen::Start(){
	ScrollScreen::Start();
	next_screen = nullptr;
	next_scene = nullptr;
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
	Button* advancedBtn				= new Button("Advanced");
	simpleBtn->SetImages(button_sprite->Clone());
	lightBtn->SetImages(button_sprite->Clone());
	advancedBtn->SetImages(button_sprite->Clone());
	simpleBtn->Clicked += MakeDelegate(this, &MainScreen::OnSimpleClick);
	lightBtn->Clicked += MakeDelegate(this, &MainScreen::OnLightClick);
	advancedBtn->Clicked += MakeDelegate(this, &MainScreen::OnAdvancedClick);
	graphics3D_menu->AddButton(simpleBtn);
	graphics3D_menu->AddButton(lightBtn);
	graphics3D_menu->AddButton(advancedBtn);

	graphics3D_simple = new Menu(false);
	Button* triangleBtn				= new Button("Triangle");
	Button* solidModelBtn			= new Button("Solid Model");
	Button* texturedModelBtn		= new Button("Textured Model");
	Button* complexBtn				= new Button("Complex Model");
	triangleBtn->SetImages(button_sprite->Clone());
	solidModelBtn->SetImages(button_sprite->Clone());
	texturedModelBtn->SetImages(button_sprite->Clone());
	complexBtn->SetImages(button_sprite->Clone());
	triangleBtn->Clicked += MakeDelegate(this, &MainScreen::OnTriangleClick);
	solidModelBtn->Clicked += MakeDelegate(this, &MainScreen::OnSolidModelClick);
	texturedModelBtn->Clicked += MakeDelegate(this, &MainScreen::OnTexturedModelClick);
	complexBtn->Clicked += MakeDelegate(this, &MainScreen::OnComplexModelClick);
	graphics3D_simple->AddButton(triangleBtn);
	graphics3D_simple->AddButton(solidModelBtn);
	graphics3D_simple->AddButton(texturedModelBtn);
	graphics3D_simple->AddButton(complexBtn);

	graphics3D_light = new Menu(false);
	Button* specularBtn 			= new Button("Specular");
	Button* diffuseMap				= new Button("Diffuse");
	Button* diffuseSpecularMapBtn	= new Button("Specular Map");
	Button* directionalLigthBtn		= new Button("Directional Light");
	Button* pointLightBtn			= new Button("Point Light");
	Button* spotLightBtn			= new Button("Spot Light");
	Button* multiLightBtn			= new Button("Multi-Ligth");
	specularBtn->SetImages(button_sprite->Clone());
	diffuseMap->SetImages(button_sprite->Clone());
	diffuseSpecularMapBtn->SetImages(button_sprite->Clone());
	directionalLigthBtn->SetImages(button_sprite->Clone());
	pointLightBtn->SetImages(button_sprite->Clone());
	spotLightBtn->SetImages(button_sprite->Clone());
	multiLightBtn->SetImages(button_sprite->Clone());
	specularBtn->Clicked += MakeDelegate(this, &MainScreen::OnMaterialClick);
	diffuseMap->Clicked += MakeDelegate(this, &MainScreen::OnDiffuseMapClick);
	diffuseSpecularMapBtn->Clicked += MakeDelegate(this, &MainScreen::OnDiffuseSpecularMapClick);
	directionalLigthBtn->Clicked += MakeDelegate(this, &MainScreen::OnDirectionalLight);
	pointLightBtn->Clicked += MakeDelegate(this, &MainScreen::OnPointLightClick);
	spotLightBtn->Clicked += MakeDelegate(this, &MainScreen::OnSpotLightClick);
	multiLightBtn->Clicked += MakeDelegate(this, &MainScreen::OnMultiLightClick);
	graphics3D_light->AddButton(specularBtn);
	graphics3D_light->AddButton(diffuseMap);
	graphics3D_light->AddButton(diffuseSpecularMapBtn);
	graphics3D_light->AddButton(directionalLigthBtn);
	graphics3D_light->AddButton(pointLightBtn);
	graphics3D_light->AddButton(spotLightBtn);
	graphics3D_light->AddButton(multiLightBtn);

	graphics3D_advanced = new Menu(false);
	Button* depthTestBtn			= new Button("Depth Test");
	depthTestBtn->SetImages(button_sprite->Clone());
	depthTestBtn->Clicked += MakeDelegate(this, &MainScreen::OnDepthTestClick);
	graphics3D_advanced->AddButton(depthTestBtn);

	graphics2D_menu->Active(false);
	graphics3D_menu->Active(false);
	graphics3D_simple->Active(false);
	graphics3D_light->Active(false);
	graphics3D_advanced->Active(false);

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
	delete graphics3D_advanced;
	input->KeyPressed -= key_released_delegate;
	game->WindowResized -= window_resized_delegate;
}

void MainScreen::Update(float sec){
	current_menu->Update(sec);

	if(next_screen){
		gfx2D->GetCamera()->SetPosition(Vector2D(0.f, 0.f));
		game->SetScreen(next_screen);
		return;
	}
	
	if(next_scene){
		game->SetScene(next_scene);
		return;
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

void MainScreen::OnAdvancedClick(){
	AdjustScreenHeight(graphics3D_advanced);
	current_menu->Active(false);
	graphics3D_advanced->Active(true);
	current_menu = graphics3D_advanced;
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
	next_scene = new TriangleScene();
}

void MainScreen::OnSolidModelClick(){
	next_scene = new SolidModelScene();
}

void MainScreen::OnTexturedModelClick(){
	next_scene = new TexturedModelScene();
}

void MainScreen::OnComplexModelClick(){
	next_scene = new ComplexModelScene();
}

void MainScreen::OnAudioClick() {
	next_screen = new AudioScreen();
}

void MainScreen::OnMaterialClick(){
	next_scene = new SpecularScene();
}

void MainScreen::OnDiffuseMapClick(){
	next_scene = new SpecularDiffuseScene();
}

void MainScreen::OnDiffuseSpecularMapClick(){
	next_scene = new SpecularMapScene();
}

void MainScreen::OnDirectionalLight(){
	next_scene = new DirectionalLightScene();
}

void MainScreen::OnPointLightClick(){
	next_scene = new PointLightScene();
}

void MainScreen::OnSpotLightClick(){
	next_screen = new SpotLightScreen();
}

void MainScreen::OnMultiLightClick(){
	next_screen = new MultiLightScreen();
}

void MainScreen::OnDepthTestClick(){
	next_screen = new DepthTestScreen();
}