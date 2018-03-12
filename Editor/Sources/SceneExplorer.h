#ifndef SCENE_EXPLORER
#define SCENE_EXPLORER

#include "Cross.h"
#include "Event.h"

#include <QTreeView>

using namespace cross;

class SceneModel : public QAbstractItemModel {
	Q_OBJECT
public:
	QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const override;
	QModelIndex parent(const QModelIndex& index) const override;
	int rowCount(const QModelIndex& parent) const override;
	int columnCount(const QModelIndex& parent) const override;
	QVariant data(const QModelIndex& index, int role) const override;

	Qt::ItemFlags flags(const QModelIndex& index) const override;
	bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

	QModelIndex AddEntity(Entity* entity, QModelIndex parent);
	void RemoveEntity(QModelIndex index);
};

class SceneExplorer : public QTreeView {
	Q_OBJECT
public:
	Event<Entity*> EntitySelected;
	Event<Entity*> EntityGrabFocus;
	Event<Entity*> EntityChanged;

	SceneExplorer(QWidget* parent = 0);
	~SceneExplorer();

	void dragEnterEvent(QDragEnterEvent *event) override;
	void dragMoveEvent(QDragMoveEvent *event) override;

	void OnScreenChanged(Screen* scene);
	void OnEntityAdded(Entity* entity);

protected:
	void contextMenuEvent(QContextMenuEvent *event) override;

private:
	SceneModel* scene_model				= NULL;
	QMenu* context_menu					= NULL;
	bool drop_approved					= false;

	void OnItemClick(QModelIndex index);
	void OnItemDoubleClick(QModelIndex index);
	void OnItemChanged(QModelIndex top, QModelIndex bot);
	void mousePressEvent(QMouseEvent* e) override;

	void OnCreateEntity();
	void OnDeleteEntity();
};

#endif