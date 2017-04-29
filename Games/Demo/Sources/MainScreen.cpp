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
#include "System.h"
#include "Sprite.h"
#include "AudioScreen.h"
#include "Camera2D.h"
#include "Graphics2D/PrimitivesScreen.h"
#include "Graphics2D/AnimationScreen.h"
#include "Graphics2D/SpritesScreen.h"
#include "Graphics2D/TextScreen.h"
#include "Graphics3D/Simple/TriangleScene.h"
#include "Graphics3D/Simple/SolidModelScene.h"
#include "Graphics3D/Simple/TexturedModelScene.h"
#include "Graphics3D/Light/MaterialScene.h"
#include "Graphics3D/Light/DirectionalLightScene.h"
#include "Graphics3D/Light/PointLightScene.h"
#include "Graphics3D/Light/SpotLightScene.h"
#include "Graphics3D/Light/MultiLightScene.h"
#include "Graphics3D/Maps/NakedScene.h"
#include "Graphics3D/Maps/DiffuseScene.h"
#include "Graphics3D/Maps/SpecularScene.h"
#include "Graphics3D/Maps/RoughnessScene.h"
#include "Graphics3D/Maps/NormalScene.h"
#include "Graphics3D/Misc/DepthScene.h"
#include "Graphics3D/Misc/TransparencyScene.h"
#include "Graphics3D/Misc/SkyboxScene.h"
#include "Graphics3D/Misc/ApocalypseScene.h"
#include "Physics/PhysicsScreen.h"
#include "Demo.h"
#include "Event.h"

void MainScreen::Start(){
	ScrollScreen::Start();

	SetBackground(Color(0.3f, 0.3f, 0.3f));

	Sprite* buttonSprite = demo->GetCommonSprite("ButtonTemplate.png");
	Sprite* buttonSpritePressed = demo->GetCommonSprite("ButtonTemplatePressed.png");

	resize_del = cross::system->WindowResized.Connect(this, &MainScreen::WindowResizedHandle);

	font = new Font("Engine/Fonts/VeraMonoBold.ttf", 80, Color(0.f, 0.f, 0.f, 0.70f));

	//main menu
	main_menu = new Menu(true);
	current_menu = main_menu;
	Button* graphics2Dbtn		= new Button("Graphics 2D", font->Clone());
	Button* graphics3Dbtn		= new Button("Graphics 3D", font->Clone());
	Button* physicsBtn			= new Button("Physics", font->Clone());
	Button* audioBtn			= new Button("Audio", font->Clone());
	graphics2Dbtn->Clicked.Connect(this, &MainScreen::OnGraphics2DClick);
	graphics3Dbtn->Clicked.Connect(this, &MainScreen::OnGraphics3DClick);
	physicsBtn->Clicked.Connect(this, &MainScreen::OnPhysicsClick);
	audioBtn->Clicked.Connect([](){ game->SetScreen(new AudioScreen()); });
	main_menu->AddButton(graphics2Dbtn);
	main_menu->AddButton(graphics3Dbtn);
	main_menu->AddButton(physicsBtn);
	main_menu->AddButton(audioBtn);
	main_menu->SetImages(buttonSprite, buttonSpritePressed);
	//graphics 2D menu
	graphics2D_menu = new Menu(false);
	Button* primitivesBtn		= new Button("Primitives", font->Clone());
	Button* spritesBtn			= new Button("Sprites", font->Clone());
	Button* textBtn				= new Button("Text Drawing", font->Clone());
	Button* animationBtn		= new Button("Animation", font->Clone());
	primitivesBtn->Clicked.Connect([](){ game->SetScreen(new PrimitivesScreen()); });
	spritesBtn->Clicked.Connect([](){ game->SetScreen(new SpritesScreen()); });
	animationBtn->Clicked.Connect([](){ game->SetScreen(new AnimationScreen()); });
	textBtn->Clicked.Connect([](){ game->SetScreen(new TextScreen()); });
	graphics2D_menu->AddButton(primitivesBtn);
	graphics2D_menu->AddButton(spritesBtn);
	graphics2D_menu->AddButton(textBtn);
	graphics2D_menu->AddButton(animationBtn);
	graphics2D_menu->SetImages(buttonSprite, buttonSpritePressed);
	//graphics 3D
	graphics3D_menu = new Menu(false);
	Button* simpleBtn			= new Button("Simple", font->Clone());
	Button* lightBtn			= new Button("Light", font->Clone());
	Button* mapsBtn				= new Button("Maps", font->Clone());
	Button* miscBtn				= new Button("Misc", font->Clone());
	simpleBtn->Clicked.Connect(this, &MainScreen::OnSimpleClick);
	lightBtn->Clicked.Connect(this, &MainScreen::OnLightClick);
	mapsBtn->Clicked.Connect(this, &MainScreen::OnMapsClick);
	miscBtn->Clicked.Connect(this, &MainScreen::OnMiscClick);
	graphics3D_menu->AddButton(simpleBtn);
	graphics3D_menu->AddButton(lightBtn);
	graphics3D_menu->AddButton(mapsBtn);
	graphics3D_menu->AddButton(miscBtn);
	graphics3D_menu->SetImages(buttonSprite, buttonSpritePressed);
	//graphics 3D simple
	graphics3D_simple = new Menu(false);
	Button* triangleBtn			= new Button("Triangle", font->Clone());
	Button* solidModelBtn		= new Button("Solid Model", font->Clone());
	Button* texturedModelBtn	= new Button("Textured Model", font->Clone());
	triangleBtn->Clicked.Connect([](){ game->SetScreen(new TriangleScene()); });
	solidModelBtn->Clicked.Connect([](){ game->SetScreen(new SolidModelScene()); });
	texturedModelBtn->Clicked.Connect([](){ game->SetScreen(new TexturedModelScene()); });
	graphics3D_simple->AddButton(triangleBtn);
	graphics3D_simple->AddButton(solidModelBtn);
	graphics3D_simple->AddButton(texturedModelBtn);
	graphics3D_simple->SetImages(buttonSprite, buttonSpritePressed);
	//graphics 3D light
	graphics3D_light = new Menu(false);
	Button* materialBtn 		= new Button("Material", font->Clone());
	Button* directionalBtn		= new Button("Directional", font->Clone());
	Button* pointLightBtn		= new Button("Point Light", font->Clone());
	Button* spotLightBtn		= new Button("Spot Light", font->Clone());
	Button* multiLightBtn		= new Button("Multi-Light", font->Clone());
	materialBtn->Clicked.Connect([](){ game->SetScreen(new MaterialScene()); });
	directionalBtn->Clicked.Connect([](){ game->SetScreen(new DirectionalLightScene()); });
	pointLightBtn->Clicked.Connect([](){ game->SetScreen(new PointLightScene()); });
	spotLightBtn->Clicked.Connect([](){ game->SetScreen(new SpotLightScene()); });
	multiLightBtn->Clicked.Connect([](){ game->SetScreen(new MultiLightScene()); });
	graphics3D_light->AddButton(materialBtn);
	graphics3D_light->AddButton(directionalBtn);
	graphics3D_light->AddButton(pointLightBtn);
	graphics3D_light->AddButton(spotLightBtn);
	graphics3D_light->AddButton(multiLightBtn);
	graphics3D_light->SetImages(buttonSprite, buttonSpritePressed);
	//graphics 3D maps
	graphics3D_maps = new Menu(false);
	Button* nakedBtn			= new Button("Naked", font->Clone());
	Button* diffuseBtn			= new Button("Diffuse", font->Clone());
	Button* specularBtn			= new Button("Specular", font->Clone());
	Button* roughnessBtn		= new Button("Roughness", font->Clone());
	Button* normalBtn			= new Button("Normal", font->Clone());
	nakedBtn->Clicked.Connect([](){ game->SetScreen(new NakedScene()); });
	diffuseBtn->Clicked.Connect([](){ game->SetScreen(new DiffuseScene()); });
	specularBtn->Clicked.Connect([](){ game->SetScreen(new SpecularScene()); });
	roughnessBtn->Clicked.Connect([](){ game->SetScreen(new RoughnessScene()); });
	normalBtn->Clicked.Connect([](){ game->SetScreen(new NormalScene()); });
	graphics3D_maps->AddButton(nakedBtn);
	graphics3D_maps->AddButton(diffuseBtn);
	graphics3D_maps->AddButton(specularBtn);
	graphics3D_maps->AddButton(roughnessBtn);
	graphics3D_maps->AddButton(normalBtn);
	graphics3D_maps->SetImages(buttonSprite, buttonSpritePressed);
	//graphics 3D misc
	graphics3D_misc = new Menu(false);
	Button* depthTestBtn		= new Button("Depth Test", font->Clone());
	Button* transparencyBtn		= new Button("Transparency", font->Clone());
	Button* skyboxBtn			= new Button("Skybox", font->Clone());
	Button* apocalypseBtn		= new Button("Apocalypse", font->Clone());
	depthTestBtn->Clicked.Connect([](){ game->SetScreen(new DepthScene()); });
	transparencyBtn->Clicked.Connect([](){ game->SetScreen(new TransparencyScene()); });
	skyboxBtn->Clicked.Connect([](){ game->SetScreen(new SkyboxScene()); });
	apocalypseBtn->Clicked.Connect([](){ game->SetScreen(new ApocalypseScene()); });
	graphics3D_misc->AddButton(depthTestBtn);
	graphics3D_misc->AddButton(transparencyBtn);
	graphics3D_misc->AddButton(skyboxBtn);
	graphics3D_misc->AddButton(apocalypseBtn);
	graphics3D_misc->SetImages(buttonSprite, buttonSpritePressed);

	physics_menu = new Menu(true);
	Button* testBtn				= new Button("Test", font->Clone());
	testBtn->Clicked.Connect([](){ game->SetScreen(new PhysicsScreen()); });
	physics_menu->AddButton(testBtn);
	physics_menu->SetImages(buttonSprite, buttonSpritePressed);

	graphics2D_menu->Active(false);
	graphics3D_menu->Active(false);
	graphics3D_simple->Active(false);
	graphics3D_light->Active(false);
	graphics3D_maps->Active(false);
	graphics3D_misc->Active(false);
	physics_menu->Active(false);
    
	delete buttonSprite;
	delete buttonSpritePressed;
}

void MainScreen::Stop(){
	ScrollScreen::Stop();
	delete physics_menu;
	delete graphics2D_menu;
	delete graphics3D_menu;
	delete graphics3D_maps;
    delete graphics3D_light;
	delete graphics3D_simple;
	delete graphics3D_misc;
	delete main_menu;
	delete font;
    cross::system->WindowResized.Disconnect(resize_del);
}

void MainScreen::Update(float sec){
	ScrollScreen::Update(sec);
	current_menu->Update(sec);
}

void MainScreen::SetMenu(Menu* menu){
	SetHeight(menu->GetHeight());
	Vector2D camPos(0.f, 0.f);
	camPos.y = GetHeight() - gfx2D->GetCamera()->GetViewHeight();
	gfx2D->GetCamera()->SetPosition(camPos);

	current_menu->Active(false);
	menu->Active(true);
	current_menu = menu;
}

void MainScreen::WindowResizedHandle(S32 width, S32 height){
	ResetScreenSize();
}

//main menu
void MainScreen::OnGraphics2DClick(){
	SetMenu(graphics2D_menu);
}

void MainScreen::OnGraphics3DClick(){
	SetMenu(graphics3D_menu);
}

void MainScreen::OnPhysicsClick(){
	SetMenu(physics_menu);
}

//graphics 3D menu
void MainScreen::OnSimpleClick(){
	SetMenu(graphics3D_simple);
}

void MainScreen::OnLightClick(){
	SetMenu(graphics3D_light);
}

void MainScreen::OnMapsClick(){
	SetMenu(graphics3D_maps);
}

void MainScreen::OnMiscClick(){
	SetMenu(graphics3D_misc);
}