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
#include "Input.h"

struct ImDrawData;
struct ImFont;

namespace cross {

#define MAX_ACTIONS 20
#define DEFAULT_FONT_SIZE 13.f

/* Class reflect particular game scene. Like menu, level 1 etc.
   Every game must implement at least one Screen class.
   Whitch will be loaded through Game::GetStartScreen() function */
class Screen {
public:
	/* Occurs when virtual screen space changed */
	Event<float, float> SizeChanged;

	virtual ~Screen() { };
	/* Called before screen show up. */
	virtual void Start();
	/* Called when screen about to change on new one */
	virtual void Stop();
	/* Called every frame update. Ideally 60 times per second(60fps) */
	virtual void Update(float sec);
	/* Called after regularly Screen::Update()  */
	virtual void LateUpdate(float sec);
	/* Called when game need to be suspend like lost focus or input phone call */
	virtual void Suspend() { };
	/* Called when game about show again after suspending */
	virtual void Resume() { };
	/* Return virtual game width for 2D drawing */
	virtual float GetWidth();
	/* Return virtual game height for 2D drawing */
	virtual float GetHeight();
	/* Handle input action down that not drop on UI elements */
	virtual void ActionDown(Input::Action action) { };
	/* Handle input action move that not drop on UI elements */
	virtual void ActionMove(Input::Action action) { };
	/* Handle input action up that not drop on UI elements */
	virtual void ActionUp(Input::Action action) { };

	const string& GetName() const;
	void SetName(const string& name);

	Camera2D* GetCamera();
	/* Returns true if current screen is actually a 3D scene */
	bool IsScene() const;
	/* Reflect ratio between screen and target device width */
	float GetScaleFactor();
	/* Set background color for areas than not covered any other stuff */
	void SetBackground(const Color& background);
	void EnableInputs(bool enable);
	/* Obtain loaded into scene shader or load it by self in other way */
	Shader* GetShader(const string& shaderFile);

protected:
	bool is_scene					= false;
	bool enable_inputs				= true;
	Camera2D* camera2D				= NULL;
	ImFont* font					= NULL;

private:
	static const char* GetClipboardString(void* userData);

	string name						= "noname";
	string clipboard				= "";
	Dictionary<S32,Shader*> shaders = Dictionary<S32, Shader*>();
	Shader* ui_shader				= NULL;
	Texture* font_texture			= NULL;

	U32 vertex_buffer				= 0;
	U32 index_buffer				= 0;

	Vector2D action_pos				= Vector2D(0.f, 0.f);
	Array<bool> actions				= Array<bool>(MAX_ACTIONS, false);
	float mouse_wheel				= 0.0f;

	void RenderUI(ImDrawData*);
	bool CreateDeviceObjects();
	bool CreateFontsTexture();

	U64 down_del					= 0;
	U64 up_del						= 0;
	U64 move_del					= 0;
	void ActionDownHandle(Input::Action action);
	void ActionMoveHandle(Input::Action action);
	void ActionUpHandle(Input::Action action);

	U64 key_pressed_del				= 0;
	U64 key_released_del			= 0;
	U64 char_enter_del				= 0;
	void KeyPressed(Key key);
	void KeyReleased(Key key);
	void CharEnter(char c);

	U64 wheel_roll					= 0;
	void WheelRoll(float delta);
};
    
}
