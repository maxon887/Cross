#include "CrossEditor.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	CrossEditor w;
	w.show();
	return a.exec();
}
