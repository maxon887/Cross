#pragma once

#include "Graphics2D.h"
#include "Screen.h"
#include "Vector2D.h"
#include "Texter.h"
#include "Debuger.h"
#include "Button.h"
#include "ToggleButton.h"

using namespace cross;


class MainScreen : public Screen
{
public:
	Font *font;
	void Start();
	void Update(float sec);
	~MainScreen();
};

