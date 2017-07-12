#ifndef PROPERTIES_VIEW
#define PROPERTIES_VIEW

#include "Cross.h"

#include <QDockWidget.h>

using namespace cross;

class EntityComponent;

class PropertiesView : public QDockWidget
{
	Q_OBJECT
public:
	PropertiesView(QWidget* parent = 0);
	~PropertiesView();

	void Update(float sec);

	void OnEntitySelected(const QModelIndex &index);

protected:
	void showEvent(QShowEvent *event);

private:
	Entity* selected_entity				= NULL;
	EntityComponent* entity_component	= NULL;
};

#endif