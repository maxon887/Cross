#include "Input.h"
#include "System.h"

using namespace cross;
using namespace std;

bool Input::IsPressed(Key key) const {
	return pressed_keys[(S32)key];
}

Input::Input() {
	KeyPressed.Connect(this, &Input::KeyPressedHandle);
	KeyReleased.Connect(this, &Input::KeyReleasedHandle);
}

Input::~Input() {
	KeyPressed.Disconnect(this, &Input::KeyPressedHandle);
	KeyReleased.Disconnect(this, &Input::KeyReleasedHandle);
}

void Input::TargetActionDownHandle(float x, float y, S32 actionID) {
	Action action;
	action.pos = Vector2D(x, y);
	action.id = actionID;
	input_mutex.lock();
	action_stack.emplace_back(action, 0);
	input_mutex.unlock();
}

void Input::TargetActionMoveHandle(float x, float y, S32 actionID) {
	Action action;
	action.pos = Vector2D(x, y);
	action.id = actionID;
	input_mutex.lock();
	action_stack.emplace_back(action, 1);
	input_mutex.unlock();
}

void Input::TargetActionUpHandle(float x, float y, S32 actionID) {
	Action action;
	action.pos = Vector2D(x, y);
	action.id = actionID;
	input_mutex.lock();
	action_stack.emplace_back(action, 2);
	input_mutex.unlock();
}

void Input::ResetKeys() {
	for(S32 i = 0; i < pressed_keys.Size(); ++i) {
		if(pressed_keys[i]) {
			KeyReleased.Emit((Key)i);
		}
	}
}

void Input::Update() {
	while(!action_stack.empty()) {
		input_mutex.lock();
		Action action = action_stack.front().first;
		int actionState = action_stack.front().second;
		action_stack.pop_front();
		input_mutex.unlock();

		action.pos = TargetToWordConvert(action.pos.x, action.pos.y);
		switch(actionState) {
		case 0:
			ActionDown.Emit(action);
			break;
		case 1:
			ActionMove.Emit(action);
			break;
		case 2:
			ActionUp.Emit(action);
			break;
		default:
			break;
		}
	}
}

Vector2D Input::TargetToWordConvert(float x, float y) {
	Vector2D result;
	result.x = x;
	result.y = (float)os->GetWindowHeight() - y;
	return result;
}

void Input::KeyPressedHandle(Key key) {
	pressed_keys[(S32)key] = true;
}

void Input::KeyReleasedHandle(Key key) {
	pressed_keys[(S32)key] = false;
}