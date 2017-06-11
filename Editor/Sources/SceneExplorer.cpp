#include "SceneExplorer.h"

#include <QTreeWidgetItem>

//QModelIndex SceneModel::index(int row, int column, const QModelIndex& parent = QModelIndex()) const {
//	if(!hasIndex(row, column, parent)){
//		return QModelIndex();
//	}
//
//	Entity* parent = NULL;
//
//	if(!parent.isValid()){
//
//	}
//}

SceneExplorer::SceneExplorer(QWidget* parent) :
	QTreeView(parent)
{
}


SceneExplorer::~SceneExplorer(){

}