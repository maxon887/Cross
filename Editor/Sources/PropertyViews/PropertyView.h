#ifndef PROPERTY_VIEW
#define PROPERTY_VIEW

#include "Cross.h"

#include <QGroupBox>

using namespace cross;

class PropertyView : public QGroupBox
{
	Q_OBJECT
public:
	PropertyView(QWidget* parent = 0) : QGroupBox(parent) { }
	virtual ~PropertyView() { }

	virtual void Initialize() { }
	virtual void OnEntitySelected(Entity* e);
	virtual void OnFileSelected(const String& filename);
	virtual bool OnComponentSelected(Component* c) { return false; }
	virtual void Update(float sec) { }
};

#endif
