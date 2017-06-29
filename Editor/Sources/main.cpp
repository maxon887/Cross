#include "CrossEditor.h"
#include "Platform/Windows/WINSystem.h"

#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	QSurfaceFormat format;
	format.setDepthBufferSize(4);
	format.setStencilBufferSize(4);
	format.setVersion(2, 0);
	format.setProfile(QSurfaceFormat::CoreProfile);
	QSurfaceFormat::setDefaultFormat(format);

	WINSystem* winsys = new WINSystem();
	sys = winsys;
	CrossEditor w;
	w.RestoreSettings();
	w.show();
	return a.exec();
}