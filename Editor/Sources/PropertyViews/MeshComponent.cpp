#include "MeshComponent.h"
#include "Entity.h"
#include "Mesh.h"
#include "Game.h"
#include "Scene.h"
#include "Material.h"

#include <QLabel.h>
#include <QLineEdit>
#include <QCheckBox>
#include <QPushButton>
#include <QFileDialog>

void MeshComponent::Initialize() {
	model_label = findChild<QLabel*>("modelLabel");
	poly_count_label = findChild<QLabel*>("polyCountLabel");
	material_value = findChild<QLineEdit*>("materialValue");
	face_culling_box = findChild<QCheckBox*>("faceCulling");
	QPushButton* loadBtn = findChild<QPushButton*>("loadBtn");
	connect(loadBtn, &QPushButton::clicked, this, &MeshComponent::OnLoadClick);
}

void MeshComponent::Show(Entity* entity){
	Mesh* mesh = entity->GetComponent<Mesh>();
	pair<S32, S32> pair = game->GetCurrentScene()->GetModelMeshID(mesh);
	Model* model = game->GetCurrentScene()->GetModel(pair.first);
	model_label->setText(model->filename.c_str());

	poly_count_label->setText(QString::number(mesh->GetPolyCount()));

	material_value->setText(mesh->GetMaterial()->GetFilename().c_str());

	face_culling_box->setChecked(mesh->IsFaceCullingEnabled());
}

void MeshComponent::OnLoadClick(){
	QString path = QDir::currentPath() + "/" + QString(sys->AssetsPath().c_str());
	QString filePath = QFileDialog::getOpenFileName(this, "Select Material File", path, "Material File (*.mat)");
	if(!filePath.isEmpty()) {
		QDir root = path;
		QString filepath = root.relativeFilePath(filePath);
		material_value->setText(filepath);
		
		Scene* scene = game->GetCurrentScene();
		Material* newMaterial = scene->GetMaterial(filepath.toStdString());
		Mesh* mesh = selected_entity->GetComponent<Mesh>();
		mesh->SetMaterial(newMaterial);
	}
}