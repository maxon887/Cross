#include "Screen.h"
#include "Internals/GraphicsGL.h"
#include "Color.h"

using namespace cross;

const String& Screen::GetName() const {
	return name;
}

void Screen::SetName(const String& name) {
	this->name = name;
}

void Screen::SetBackground(const Color& c) {
	SAFE(glClearColor(c.R, c.G, c.B, 1.f));
}