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
#pragma once
#include "Cross.h"
#include "ScrollScreen.h"
#include "Utils/Button.h"
#include "Menu.h"

using namespace cross;

class MainScreen : public ScrollScreen{
public:
	void Start();
	void Stop();
	void Update(float sec);

private:
	Font* font;

	Menu* current_menu;
	Menu* main_menu;

	Menu* graphics2D_menu;
	Menu* graphics3D_menu;

	Menu* graphics3D_simple;
	Menu* graphics3D_light;
	Menu* graphics3D_maps;
	Menu* graphics3D_misc;

	Screen* next_screen;

	void AdjustScreenHeight(Menu* menu);

	FastDelegate1<Key, void> key_released_delegate;
	FastDelegate2<S32, S32, void> window_resized_delegate;
	//connections
	void KeyReleasedHandle(Key key);
	void WindowResizedHandle(S32 width, S32 height);
	//main menu
	void OnGraphics2DClick();
	void OnGraphics3DClick();
	void OnAudioClick();
	//graphics 2d menu
	void OnPrimitivesClick();
	void OnSpritesClick();
	void OnTextClick();
	void OnAnimationClick();
	//graphics 3d menu
	void OnSimpleClick();
	void OnLightClick();
	void OnMapsClick();
	void OnMiscClick();
	//graphics 3d simple
	void OnTriangleClick();
	void OnSolidModelClick();
	void OnTexturedModelClick();
	void OnComplexModelClick();
	//graphics 3d light
	void OnMaterialClick();
	void OnDirectionalLight();
	void OnPointLightClick();
	void OnSpotLightClick();
	void OnMultiLightClick();
	//graphics 3d maps
	void OnNakedClick();
	void OnDiffuseClick();
	void OnSpecularClick();
	void OnRoughnessClick();
	void OnNormalClick();
	//graphics 3d misc
	void OnDepthTestClick();
	void OnStencilTestClick();
	void OnTransparencyClick();
	void OnSkyboxClick();
	void OnApocalypseClick();
};
