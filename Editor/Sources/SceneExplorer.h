#ifndef SCENE_EXPLORER
#define SCENE_EXPLORER

#include "Cross.h"

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
};

class SceneExplorer : public QTreeView {
	Q_OBJECT
public:
	SceneExplorer(QWidget* parent = 0);
	~SceneExplorer();

	void OnScreenChanged(Screen* scene);
	void OnEntityAdded(Entity* entity);

private:
	SceneModel* scene_model;
};

#endif