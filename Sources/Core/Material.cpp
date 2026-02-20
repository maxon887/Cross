#include "Material.h"
#include "System.h"
#include "File.h"
#include "Scene.h"
#include "Graphics.h"

#include "Libs/TinyXML2/tinyxml2.h"

using namespace cross;
using namespace tinyxml2;

Material::Material(Shader* shader) :
	shader(shader)
{
	Reset();//also loads uniforms from shader
}

Material::Material(const Material& obj) :
	shader(obj.shader),
	active_texture_slot(obj.active_texture_slot),
	transparent(obj.transparent),
	properties(obj.properties)
{ }

void Material::SetShader(Shader* shader) {
	this->shader = shader;
	Reset();
}

Shader* Material::GetShader() {
	return shader;
}

const String& Material::GetFilename() const {
	return filename;
}

bool Material::Load(const String& inFilename, Scene* scene) {
	CROSS_RETURN(os->IsAssetFileExists(inFilename), false, "Material file doesn't exists:\n#", inFilename)
	filename = inFilename;
	File* xmlFile = os->LoadAssetFile(filename);
	CROSS_RETURN(xmlFile, false, "Can't load material file:\n#", filename);
	XMLDocument doc;
	XMLError error = doc.Parse((const char*)xmlFile->data, (Size)xmlFile->size);
	delete xmlFile;
	CROSS_RETURN(error == XML_SUCCESS, false, "Can't parse material xml file");

	XMLElement* materialXML = doc.FirstChildElement("Material");
	String shaderfilename = materialXML->Attribute("shader");
	CROSS_RETURN(shaderfilename, false, "Material file not contain 'shader' filename");

	if(!shaderfilename.IsEmpty()) {
		if(scene) {
			shader = scene->GetShader(shaderfilename);
		} else {
			delete shader;
			shader = gfx->LoadShader(shaderfilename);
		}
		Reset();
	}

	XMLElement* transparencyXML = materialXML->FirstChildElement("Transparent");
	if(transparencyXML) {
		String transText = transparencyXML->GetText();
		if(transText == "True") {
			transparent = true;
		}
	}

	XMLElement* propertyXML = materialXML->FirstChildElement("Property");
	while(propertyXML) {
		const char* name = propertyXML->Attribute("name");
		CROSS_RETURN(name, false, "Property without name");
		if(HaveProperty(name)) {
			Shader::Uniform* prop = GetProperty(name);
			switch(prop->type) {
			case Shader::Uniform::INT: {
				int value = propertyXML->IntAttribute("value");
				prop->SetValue(value);
			} break;
			case Shader::Uniform::FLOAT: {
				double value = propertyXML->DoubleAttribute("value");
				prop->SetValue((float)value);
			} break;
			case Shader::Uniform::COLOR: {
				String value = propertyXML->Attribute("value");
				prop->SetValue(Color(value));
			} break;
			case Shader::Uniform::TEXTURE: {
				String textureFilename = propertyXML->Attribute("value");
				if(!textureFilename.IsEmpty()) {
					if(scene) {
						Texture* texture = scene->GetTexture(textureFilename);
						prop->SetValue(texture);
					} else {
						Texture* texture = CREATE Texture();
						texture->Load(textureFilename);
						prop->SetValue(texture);
					}
				}
			} break;
			default:
				CROSS_ASSERT(false, "Unsupported property type");
			}
		} else {
			CROSS_ASSERT(false, "Material loading error. Shader # doesn't contain property '#'", shader->GetFilename(), name);
		}
		propertyXML = propertyXML->NextSiblingElement("Property");
	}
	return true;
}

void Material::Save(const String& filename) {
	XMLDocument doc;
	XMLElement* materialXML = doc.NewElement("Material");

	String shaderName = "";
	if(GetShader()) {
		shaderName = GetShader()->GetFilename();
	}
	materialXML->SetAttribute("shader", shaderName);

	doc.LinkEndChild(materialXML);

	if(IsTransparent()) {
		XMLElement* transparencyXML = doc.NewElement("Transparent");
		transparencyXML->SetText("True");
		materialXML->LinkEndChild(transparencyXML);
	}

	for(const Shader::Uniform& prop : properties) {
		XMLElement* propertyXML = doc.NewElement("Property");
		propertyXML->SetAttribute("name", prop.name);
		switch(prop.type) {
		case Shader::Uniform::INT: {
			propertyXML->SetAttribute("value", prop.value.s32);
			break;
		}
		case Shader::Uniform::FLOAT: {
			propertyXML->SetAttribute("value", prop.value.f);
			break;
		}
		case Shader::Uniform::Type::COLOR: {
			String color = prop.value.color.ToString();
			propertyXML->SetAttribute("value", color);
			break;
		}
		case Shader::Uniform::Type::TEXTURE: {
			Texture* texture = prop.value.texture;
			if(texture && texture->GetName() != "") {
				propertyXML->SetAttribute("value", texture->GetName());
			} else {
				propertyXML->SetAttribute("value", "");
			}
			break;
		}
		default:
			CROSS_ASSERT(false, "Unknown material property to save");
		}
		materialXML->LinkEndChild(propertyXML);
	}

	XMLPrinter printer;
	doc.Accept(&printer);
	File saveFile;
	saveFile.name = filename;
	saveFile.size = printer.CStrSize() - 1;//-1 because we don't need to save null-terminated string
	saveFile.data = (Byte*)printer.CStr();
	os->SaveFile(&saveFile);
	saveFile.data = nullptr;
}

void Material::Reset() {
	properties.Clear();
	properties = shader->GetUniforms();
}

bool Material::HaveProperty(const String& name) {
	for(const Shader::Uniform& prop : properties) {
		if(prop.name == name) {
			return true;
		}
	}
	return false;
}

Shader::Uniform* Material::GetProperty(const String& name) {
	for(Shader::Uniform& prop : properties){
		if(prop.name == name){
			return &prop;
		}
	}
	CROSS_RETURN(false, nullptr, "Can not find property '#'", name);
}

Shader::Uniform* Material::GetProperty(U64 glID) {
	for(Shader::Uniform& prop : properties){
		if(prop.glId == glID){
			return &prop;
		}
	}
	CROSS_RETURN(false, nullptr, "Can not find property by ID(#)", glID);
}

Array<Shader::Uniform>& Material::GetProperties() {
	return properties;
}

void Material::SetPropertyValue(const String& name, S32 value) {
	Shader::Uniform* prop = GetProperty(name);
	CROSS_FAIL(prop, "Can not set property '#'", name);
	prop->SetValue(value);
}

void Material::SetPropertyValue(const String& name, float value) {
	Shader::Uniform* prop = GetProperty(name);
	CROSS_FAIL(prop, "Can not set property '#'", name);
	prop->SetValue(value);
}

void Material::SetPropertyValue(const String& name, const Color& value) {
	Shader::Uniform* prop = GetProperty(name);
	CROSS_FAIL(prop, "Can not set property '#'", name);
	prop->SetValue(value);
}

void Material::SetPropertyValue(const String& name, Vector3D& value) {
	Shader::Uniform* prop = GetProperty(name);
	CROSS_FAIL(prop, "Can not set property '#'", name);
	prop->SetValue(value);
}

void Material::SetPropertyValue(const String& name, Vector4D& value) {
	Shader::Uniform* prop = GetProperty(name);
	CROSS_FAIL(prop, "Can not set property '#'", name);
	prop->SetValue(value);
}

void Material::SetPropertyValue(const String& name, Matrix& value) {
	Shader::Uniform* prop = GetProperty(name);
	CROSS_FAIL(prop, "Can not set property '#'", name);
	prop->SetValue(value);
}

void Material::SetPropertyValue(const String& name, Texture* value) {
	Shader::Uniform* prop = GetProperty(name);
	CROSS_FAIL(prop, "Can not set property '#'", name);
	prop->SetValue(value);
}

void Material::SetPropertyValue(U64 glID, S32 value) {
	Shader::Uniform* prop = GetProperty(glID);
	CROSS_FAIL(prop, "Can not set property by id(#)", glID);
	prop->SetValue(value);
}

void Material::SetPropertyValue(U64 glID, float value) {
	Shader::Uniform* prop = GetProperty(glID);
	CROSS_FAIL(prop, "Can not set property by id(#)", glID);
	prop->SetValue(value);
}

void Material::SetPropertyValue(U64 glID, const Color& value) {
	Shader::Uniform* prop = GetProperty(glID);
	CROSS_FAIL(prop, "Can not set property by id(#)", glID);
	prop->SetValue(value);
}

void Material::SetPropertyValue(U64 glID, Vector3D& value) {
	Shader::Uniform* prop = GetProperty(glID);
	CROSS_FAIL(prop, "Can not set property by id(#)", glID);
	prop->SetValue(value);
}

void Material::SetPropertyValue(U64 glID, Vector4D& value) {
	Shader::Uniform* prop = GetProperty(glID);
	CROSS_FAIL(prop, "Can not set property by id(#)", glID);
	prop->SetValue(value);
}

void Material::SetPropertyValue(U64 glID, Matrix& value) {
	Shader::Uniform* prop = GetProperty(glID);
	CROSS_FAIL(prop, "Can not set property by id(#)", glID);
	prop->SetValue(value);
}

void Material::SetPropertyValue(U64 glID, Texture* value) {
	Shader::Uniform* prop = GetProperty(glID);
	CROSS_FAIL(prop, "Can not set property by id(#)", glID);
	prop->SetValue(value);
}

void Material::SetTransparent(bool yes) {
	transparent = yes;
}

bool Material::IsTransparent() const {
	return transparent;
}

void Material::EnableTransparency(bool yes) {
	transparent = yes;
}

Material* Material::Clone() const {
	return CREATE Material(*this);
}