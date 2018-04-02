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
#include "ESystem.h"
#include "CrossEditor.h"

#include <QMessageBox>

bool ESystem::Alert(const String& msg) {
	paused = true;

	QMessageBox msgBox;
	msgBox.setText("Something goes wrong");
	msgBox.setInformativeText(msg.ToCStr());
	msgBox.setIcon(QMessageBox::Icon::Critical);

	msgBox.addButton("Ok", QMessageBox::ButtonRole::RejectRole);
	msgBox.addButton("Skip", QMessageBox::ButtonRole::AcceptRole);
	int reply = msgBox.exec();
	paused = false;
	if(reply == QMessageBox::Accepted) {
		return true;
	}
	return false;
}

bool ESystem::IsPaused() {
	return paused;
}