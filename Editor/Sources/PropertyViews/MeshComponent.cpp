#include "MeshComponent.h"
#include "Entity.h"
#include "Mesh.h"
#include "../CrossEditor.h"
#include "Scene.h"
#include "Material.h"
#include "System.h"

#include <QLabel.h>
#include <QLineEdit>
#include <QCheckBox>
#include <QPushButton>
#include <QFileDialog>

void MeshComponent::Initialize() {
	model_value = findChild<QLabel*>("modelValue");
	poly_count_value = findChild<QLabel*>("polyCountLabel");
	material_value = findChild<QLineEdit*>("materialValue");
	face_culling_box = findChild<QCheckBox*>("faceCulling");
	QPushButton* loadBtn = findChild<QPushButton*>("loadBtn");
	connect(loadBtn, &QPushButton::clicked, this, &MeshComponent::OnLoadClick);

	editor->AdjustSize(findChild<QLabel*>("modelLabel"));
	editor->AdjustSize(findChild<QLabel*>("polyCountLabel"));
	editor->AdjustSize(findChild<QLabel*>("materialLabel"));
	editor->AdjustSize(findChild<QLabel*>("faceCullingLabel"));
}

void MeshComponent::Show(Mesh* mesh) {
	if(mesh->GetModel()) {
		model_value->setText(mesh->GetModel()->GetFilename().ToCStr());
	}

	poly_count_value->setText(QString::number(mesh->GetPolyCount()));

	if(mesh->GetMaterial()) {
		material_value->setText(mesh->GetMaterial()->GetFilename().ToCStr());
	}

	face_culling_box->setChecked(mesh->IsFaceCullingEnabled());
}

void MeshComponent::OnLoadClick() {
	QString path = QDir::currentPath() + "/" + QString(system->AssetsPath().ToCStr());
	QString filePath = QFileDialog::getOpenFileName(this, "Select Material File", path, "Material File (*.mat)");
	if(!filePath.isEmpty()) {
		QDir root = path;
		QString filepath = root.relativeFilePath(filePath);
		material_value->setText(filepath);
		
		Scene* scene = game->GetCurrentScene();
		Material* newMaterial = scene->GetMaterial(filepath.toLatin1().data());
		component->SetMaterial(newMaterial);
	}
}