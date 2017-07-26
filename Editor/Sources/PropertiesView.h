#ifndef PROPERTIES_VIEW
#define PROPERTIES_VIEW

#include "Cross.h"
#include "ui_FileProperties.h"

#include <QDockWidget.h>

using namespace cross;

class ComponentView;
class ShaderView;

class PropertiesView : public QDockWidget
{
	Q_OBJECT
public:
	PropertiesView(QWidget* parent = 0);
	~PropertiesView();

	void OnUIInitialized();
	void Update(float sec);

	void OnScreenChanged(Screen*);
	void OnEntitySelected(Entity* entity);
	void OnEntityChanged(Entity* entity);
	void OnFileSelected(string filename);

private:
	Entity* selected_entity					= NULL;
	Array<ComponentView*> entity_components = Array<ComponentView*>();
	QWidget* file_properties_widget			= NULL;
	ShaderView* shader_properties			= NULL;
};

#endif