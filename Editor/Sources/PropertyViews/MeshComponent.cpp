#include "MeshComponent.h"
#include "Entity.h"
#include "Mesh.h"
#include "Game.h"
#include "Scene.h"
#include <QLabel.h>

void MeshComponent::Initialize() {
	model_label = findChild<QLabel*>("modelLabel");
	material_label = findChild<QLabel*>("material_label");
}

void MeshComponent::OnEntitySelected(Entity* entity){
	if(!entity){
		PropertyView::OnEntitySelected(entity);
		return;
	}
	Mesh* mesh = entity->GetComponent<Mesh>();
	if(!mesh){
		PropertyView::OnEntitySelected(entity);
		return;
	}
	show();

	pair<S32, S32> pair = game->GetCurrentScene()->GetModelMeshID(mesh);
	Model* model = game->GetCurrentScene()->GetModel(pair.first);
	model_label->setText(QString("Model - ") + model->filename.c_str());
}