#ifndef COMPONENT_VIEW
#define COMPONENT_VIEW

#include "Cross.h"

#include <QGroupBox.h>

using namespace cross;

class PropertyView : public QGroupBox
{
	Q_OBJECT
public:
	PropertyView(QWidget* parent = 0) : QGroupBox(parent) { }
	virtual ~PropertyView() { }

	virtual void Initialize() { }
	virtual void OnEntitySelected(Entity* e) { }
	virtual void Update(float sec) { }
};

#endif