#ifndef PROPERTIES_VIEW
#define PROPERTIES_VIEW

#include "Cross.h"
#include "System.h"

#include <QDockWidget>

using namespace cross;

class PropertyView;
class QVBoxLayout;
class QMenu;

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
	void OnFileSelected(QString filename);

protected:
	void contextMenuEvent(QContextMenuEvent *event) override;

private:
	Entity* selected_entity					= NULL;
	Array<PropertyView*> views				= Array<PropertyView*>();
	QVBoxLayout* layout						= NULL;
	QMenu* context_menu						= NULL;

	template<class View, class UI>
	void CreateView(const QString& name);

	void OnAddComponent();
};

template<class View, class UIClass>
void PropertiesView::CreateView(const QString& name){
	QWidget* container = new QWidget(this);
	UIClass ui;
	ui.setupUi(container);
	layout->insertWidget(layout->count() - 1, container);
	View* view = container->findChild<View*>(name);
	CROSS_FAIL(view, "View '#' not found", name.toLatin1().data());
	views.push_back(view);
}

#endif
