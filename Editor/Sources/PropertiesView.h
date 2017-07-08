#ifndef PROPERTIES_VIEW
#define PROPERTIES_VIEW

#include "Cross.h"

#include <QDockWidget.h>

using namespace cross;

class PropertiesView : public QDockWidget
{
	Q_OBJECT
public:
	PropertiesView(QWidget* parent = 0);
	~PropertiesView();

protected:
	void showEvent(QShowEvent *event);
};

#endif