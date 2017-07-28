#include "MeshComponent.h"
#include "Entity.h"
#include "Mesh.h"
#include "Material.h"
#include "Shaders/Shader.h"
#include <QLabel.h>

void MeshComponent::Initialize() {
	shader_filename = findChild<QLabel*>();
}

void MeshComponent::OnEntitySelected(Entity* entity){
	Mesh* mesh = entity->GetComponent<Mesh>();
	if(mesh){
		QString title = "Shader - ";
		title += mesh->GetMaterial()->GetShader()->GetFilename().c_str();
		shader_filename->setText(title);
	}else{
		hide();
	}
}