#ifndef SCENE_EXPLORER
#define SCENE_EXPLORER

#include "Cross.h"

#include <QTreeView>

using namespace cross;

class SceneModel : public QAbstractItemModel {
	Q_OBJECT
public:
	QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const override;
};

class SceneExplorer : public QTreeView {
	Q_OBJECT
public:
	SceneExplorer(QWidget* parent = 0);
	~SceneExplorer();
};

#endif