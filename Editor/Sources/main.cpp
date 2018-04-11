#include "CrossEditor.h"
#include "ESystem.h"

#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
	QApplication a(argc, argv);

	QSurfaceFormat format;
	format.setDepthBufferSize(4);
	format.setStencilBufferSize(4);
	format.setVersion(2, 0);
	format.setProfile(QSurfaceFormat::CoreProfile);
	QSurfaceFormat::setDefaultFormat(format);

	ESystem* winsys = new ESystem();
	system = winsys;
	CrossEditor w;
#ifdef WIN
	winsys->SetWND((HWND)w.winId());
#endif
	w.RestoreSettings();
	w.show();
	return a.exec();
}
