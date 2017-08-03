#include "MeshComponent.h"
#include "Entity.h"
#include "Mesh.h"
#include "Game.h"
#include "Scene.h"
#include "Material.h"

#include <QLabel.h>
#include <QLineEdit>
#include <QCheckBox>

void MeshComponent::Initialize() {
	model_label = findChild<QLabel*>("modelLabel");
	poly_count_label = findChild<QLabel*>("polyCountLabel");
	material_value = findChild<QLineEdit*>("materialValue");
	face_culling_box = findChild<QCheckBox*>("faceCulling");
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
	model_label->setText(model->filename.c_str());

	poly_count_label->setText(QString::number(mesh->GetPolyCount()));

	material_value->setText(mesh->GetMaterial()->GetFilename().c_str());

	face_culling_box->setChecked(mesh->IsFaceCullingEnabled());
}