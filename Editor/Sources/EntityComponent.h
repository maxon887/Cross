#ifndef ENTITY_COMPONENT
#define ENTITY_COMPONENT

#include "Cross.h"

#include <QGroupBox.h>

using namespace cross;

class EntityComponent : public QGroupBox
{
	Q_OBJECT
public:
	EntityComponent(QWidget* parent = 0);
	~EntityComponent();

private:
	Entity* entity = NULL;
};

#endif