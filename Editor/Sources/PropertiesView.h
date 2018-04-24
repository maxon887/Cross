#ifndef PROPERTIES_VIEW
#define PROPERTIES_VIEW

#include "Cross.h"
#include "System.h"

#include <QDockWidget>
#include <QVBoxLayout>

using namespace cross;

class PropertyView;
class QMenu;

class PropertiesView : public QDockWidget
{
	Q_OBJECT
public:
	Event<bool> VisibilityChanged;

	PropertiesView(QWidget* parent = 0);

	void OnUIInitialized();
	void Update(float sec);

	void OnScreenChanged(Screen*);
	void OnEntitySelected(Entity* entity);
	void OnEntityChanged(Entity* entity);
	void OnFileSelected(QString filename);

protected:
	void contextMenuEvent(QContextMenuEvent *event) override;
	void hideEvent(QHideEvent *eve) override;
	void showEvent(QShowEvent *eve) override;

private:
	Entity* selected_entity					= nullptr;
	Array<PropertyView*> views				= Array<PropertyView*>();
	QVBoxLayout* layout						= nullptr;
	QMenu* context_menu						= nullptr;

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
