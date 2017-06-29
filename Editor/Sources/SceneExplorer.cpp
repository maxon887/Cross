#include "SceneExplorer.h"
#include "Game.h"
#include "Scene.h"
#include "Entity.h"
#include "System.h"

#include <QHeaderView.h>

QModelIndex SceneModel::index(int row, int column, const QModelIndex& parent) const {
	if(!hasIndex(row, column, parent)){
		return QModelIndex();
	}

	Entity* parentEntity = NULL;

	if(!parent.isValid()){
		parentEntity = game->GetCurrentScene()->GetRoot();
	}else{
		parentEntity = static_cast<Entity*>(parent.internalPointer());
	}

	Entity* childEntity = parentEntity->FindChild(row);
	return createIndex(row, column, childEntity);
}

QModelIndex SceneModel::parent(const QModelIndex& index) const {
	if(!index.isValid()){
		return QModelIndex();
	}
	Entity* childEntity = static_cast<Entity*>(index.internalPointer());
	Entity* parentEntity = childEntity->GetParent();

	if(parentEntity == game->GetCurrentScene()->GetRoot()){
		return QModelIndex();
	}
	//finding row
	int row = 0;
	cross::List<Entity*>& list = parentEntity->GetParent()->GetChildren();
	for(Entity* e : list){
		if(e == parentEntity){
			break;
		}
		row++;
	}
	return createIndex(row, 0, parentEntity);
}

int SceneModel::rowCount(const QModelIndex& parent) const {
	if(parent.column() > 0){
		return 0;
	}
	Entity* parentEntity = NULL;
	if(!parent.isValid()){
		if(game->GetCurrentScreen()){
			parentEntity = game->GetCurrentScene()->GetRoot();
		}else{
			return 0;
		}
	}else{
		parentEntity = static_cast<Entity*>(parent.internalPointer());
	}
	cross::List<Entity*>& children = parentEntity->GetChildren();
	return children.size();
}

int SceneModel::columnCount(const QModelIndex& parent) const {
	return 1;
}

QVariant SceneModel::data(const QModelIndex& index, int role) const {
	if(!index.isValid()){
		return QVariant();
	}
	if(role != Qt::DisplayRole){
		return QVariant();
	}
	Entity* entity = static_cast<Entity*>(index.internalPointer());
	return QVariant(QString(entity->GetName().c_str()));
}

SceneExplorer::SceneExplorer(QWidget* parent) :
	QTreeView(parent)
{
	scene_model = new SceneModel();
	setModel(scene_model);
	header()->hide();
}


SceneExplorer::~SceneExplorer(){

}