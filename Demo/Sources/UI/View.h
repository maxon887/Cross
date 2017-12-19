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

using namespace cross;

class View {
public:
	View(string name);
	virtual ~View() { }

    virtual void WillContent() { }
	virtual void Content(float sec) { }
	virtual void DidContent() { }

	void Show();
	void Hide();
	void Update(float sec);

	const string& GetName() const;

protected:
	void SetWindowFlags(U32 flags);

private:
	string name = "View";
	bool visible = false;
	U32 flags = 0;
};
