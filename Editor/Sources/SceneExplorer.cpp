#include "SceneExplorer.h"
#include "Game.h"
#include "Scene.h"
#include "Entity.h"
#include "System.h"
#include "CrossEditor.h"

#include <QHeaderView.h>
#include <QMouseEvent>

QModelIndex SceneModel::index(int row, int column, const QModelIndex& parent) const {
	if(!hasIndex(row, column, parent)){
		return QModelIndex();
	}

	Entity* parentEntity = NULL;

	if(!parent.isValid()){
		CROSS_RETURN(game->GetCurrentScene(), QModelIndex(), "Can not obtain scene");
		parentEntity = game->GetCurrentScene()->GetRoot();
		CROSS_RETURN(parentEntity, QModelIndex(), "Invalid root entity");
	}else{
		parentEntity = static_cast<Entity*>(parent.internalPointer());
	}

	Entity* childEntity = parentEntity->FindChild(row);
	return createIndex(row, column, childEntity);
}

QModelIndex SceneModel::parent(const QModelIndex& index) const {
	if(!index.isValid() || !game->GetCurrentScreen()){
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
	if(role != Qt::DisplayRole && role != Qt::EditRole){
		return QVariant();
	}
	Entity* entity = static_cast<Entity*>(index.internalPointer());
	return QVariant(QString(entity->GetName().c_str()));
}

Qt::ItemFlags SceneModel::flags(const QModelIndex& index) const {
	return Qt::ItemIsEditable | QAbstractItemModel::flags(index);
}

bool SceneModel::setData(const QModelIndex &index, const QVariant &value, int role) {
	if(role != Qt::EditRole){
		return false;
	}
	Entity* entity = (Entity*)index.internalPointer();
	entity->SetName(value.toString().toStdString());
	emit dataChanged(index, index);
	return true;
}

QModelIndex SceneModel::AddEntity(Entity* entity, QModelIndex parent){
	beginInsertRows(parent, 0, rowCount(parent));

	Entity* parentEntity = (Entity*)parent.internalPointer();
	if(parentEntity){
		parentEntity->AddChild(entity);
	}else{
		game->GetCurrentScene()->AddEntity(entity);
	}

	endInsertRows();

	int rows = rowCount(parent);
	for(int i = 0; i < rowCount(parent); i++){
		QModelIndex inx = index(i, 0, parent);
		Entity* cur = (Entity*)inx.internalPointer();
		if(entity == cur){
			return inx;
		}
	}
	return QModelIndex();
}

void SceneModel::RemoveEntity(QModelIndex index){
	QModelIndex p = index.parent();
	beginRemoveRows(index.parent(), 0, rowCount(index.parent()));

	Entity* entity = (Entity*)index.internalPointer();
	Entity* parent = entity->GetParent();
	parent->RemoveChild(entity);
	delete entity;

	endRemoveRows();
}

SceneExplorer::SceneExplorer(QWidget* parent) :
	QTreeView(parent)
{
	scene_model = new SceneModel();
	setModel(scene_model);
	header()->hide();
	editor->ScreenChanged.Connect(this, &SceneExplorer::OnScreenChanged);

	connect(this, &QTreeView::clicked, this, &SceneExplorer::OnItemClick);
	connect(this, &QTreeView::doubleClicked, this, &SceneExplorer::OnItemDoubleClick);
	connect(scene_model, &SceneModel::dataChanged, this, &SceneExplorer::OnItemChanged);

	context_menu = new QMenu(this);

	QAction* createEntity = new QAction(context_menu);
	createEntity->setText("Create Entity");
	connect(createEntity, &QAction::triggered, this, &SceneExplorer::OnCreateEntity);
	context_menu->addAction(createEntity);
	QAction* deleteEntity = new QAction(context_menu);
	deleteEntity->setText("Delete Entity");
	connect(deleteEntity, &QAction::triggered, this, &SceneExplorer::OnDeleteEntity);
	context_menu->addAction(deleteEntity);
}

SceneExplorer::~SceneExplorer(){
	delete scene_model;
}

void SceneExplorer::OnScreenChanged(Screen* screen){
	Scene* scene = dynamic_cast<Scene*>(screen);
	if(scene){
		scene->EntityAdded.Connect(this, &SceneExplorer::OnEntityAdded);
	}
	reset();
}

void SceneExplorer::OnEntityAdded(Entity* entity){
	reset();
}

void SceneExplorer::contextMenuEvent(QContextMenuEvent *event) {
	context_menu->exec(event->globalPos());
}

void SceneExplorer::OnItemClick(QModelIndex index){
	Entity* selected = (Entity*)index.internalPointer();
	EntitySelected(selected);
}

void SceneExplorer::OnItemDoubleClick(QModelIndex index){
	Entity* selected = (Entity*)index.internalPointer();
	EntityGrabFocus(selected);
}

void SceneExplorer::OnItemChanged(QModelIndex top, QModelIndex bot){
	Entity* entity = (Entity*)top.internalPointer();
	EntityChanged(entity);
}

void SceneExplorer::mousePressEvent(QMouseEvent* e){
	QTreeView::mousePressEvent(e);
	QModelIndex index = indexAt(e->pos());
	if(index.row() == -1 && index.column() == -1) {
		clearSelection();
		const QModelIndex index;
		selectionModel()->setCurrentIndex(index, QItemSelectionModel::Select);
		EntitySelected(NULL);
	}
}

void SceneExplorer::OnCreateEntity(){
	QModelIndexList selected = selectedIndexes();
	EntitySelected(NULL);
	Entity* newEntity = new Entity("New Entity");
	if(selected.size() > 0){
		QModelIndex newIndex = scene_model->AddEntity(newEntity, selected[0]);
		edit(newIndex);
	}else{
		QModelIndex newIndex = scene_model->AddEntity(newEntity, QModelIndex());
		edit(newIndex);
	}
}

void SceneExplorer::OnDeleteEntity(){
	QModelIndexList selected = selectedIndexes();
	if(selected.size() > 0) {
		scene_model->RemoveEntity(selected[0]);
	}
}