#ifndef COMPONENT_VIEW
#define COMPONENT_VIEW

#include "Cross.h"

#include <QGroupBox.h>

class QLineEdit;
class QLabel;

using namespace cross;

class ComponentView : public QGroupBox
{
	Q_OBJECT
public:
	ComponentView(QWidget* parent = 0);
	virtual ~ComponentView();

	virtual void OnEntitySelected(Entity* e) { }
	virtual void Update(float sec) { }

private:
	QLabel* shader_filename = NULL;
};

#endif