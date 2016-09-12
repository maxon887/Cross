#include "CrossEditor.h"
#include <QtWidgets/QApplication>
#include "Platform/Windows/LauncherWIN.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	QSurfaceFormat format;
	format.setDepthBufferSize(4);
	format.setStencilBufferSize(4);
	format.setVersion(2, 0);
	format.setProfile(QSurfaceFormat::CoreProfile);
	QSurfaceFormat::setDefaultFormat(format);

	LauncherWIN* launcherWIN = new LauncherWIN();
	launcher = launcherWIN;
	CrossEditor w;
	w.show();
	return a.exec();
}
