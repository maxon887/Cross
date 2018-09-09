/*	Copyright © 2018 Maksim Lukyanov

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

using namespace cross;

class View {
public:
	View(const String& name);
	virtual ~View();

	virtual void Shown() { }
	virtual void Hidden() { }

	virtual void PreUpdate() { }
	virtual void Update(float sec) { }
	virtual void PostUpdate() { }

	virtual bool VisibleInMenu() { return true; }
	virtual bool AvailableInMenu() { return true; }

	void Show();
	void Hide();
	bool IsVisible() const;
	const String& GetName() const;

	void Run(float sec);

protected:
	void SetFlags(U32 flags);

private:
	String landscape_name = "View##Landscape";
	String portrait_name = "View##Portrait";
	bool visible = false;
	U32 flags = 0;
};
