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
	filename_label = findChild<QLabel*>("filename_label");
	poly_count_value = findChild<QLabel*>("polycount_value");
	material_value = findChild<QLineEdit*>("material_file");
	face_culling_box = findChild<QCheckBox*>("face_culling");
	QPushButton* loadBtn = findChild<QPushButton*>("load_button");
	connect(loadBtn, &QPushButton::clicked, this, &MeshComponent::OnLoadClick);
}

void MeshComponent::Show(Mesh* mesh) {
	if(mesh->GetModel()) {
		filename_label->setText(mesh->GetModel()->GetFilename().ToCStr());
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