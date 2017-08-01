#include "MaterialView.h"
#include "File.h"
#include "System.h"
#include "Shaders/Shader.h"
#include "Material.h"

#include "Libs/TinyXML2/tinyxml2.h"

#include <QLabel>
#include <QLineEdit>
#include <QHBoxLayout>

using namespace cross;
using namespace tinyxml2;

MaterialView::~MaterialView(){
	delete shader;
}

void MaterialView::Initialize(){
	shaderLabel = findChild<QLabel*>("shaderLabel");
	properties_box = findChild<QGroupBox*>("properties");
}

void MaterialView::OnFileSelected(const string& filepath){
	if(File::ExtensionFromFile(filepath) != "mat") {
		PropertyView::OnFileSelected(filepath);
		return;
	}
	Clear();

	string filename = File::FileFromPath(File::FileWithoutExtension(filepath));
	setTitle(QString("Material: ") + filename.c_str());

	string path = sys->AssetsPath() + filepath;
	XMLDocument doc;
	XMLError error = doc.LoadFile(path.c_str());
	if(error != XML_SUCCESS) {
		if(error == XML_ERROR_FILE_NOT_FOUND) {
			throw CrossException("File not found %s", path.c_str());
		} else {
			throw CrossException("Can not parse XML document");
		}
	}

	XMLElement* materialXML = doc.FirstChildElement("Material");
	const char* shaderfilename = materialXML->Attribute("shader");
	shaderLabel->setText(QString("Shader - ") + shaderfilename);

	delete shader;
	shader = new Shader(shaderfilename);
	
	XMLElement* propertyXML = materialXML->FirstChildElement("Property");
	Array<pair<const char*, const char*> > matProperties;
	while(propertyXML){
		const char* name = propertyXML->Attribute("name");
		const char* value = propertyXML->Attribute("value");
		propertyXML = propertyXML->NextSiblingElement("Property");
	}

	for(Shader::Property* prop : shader->GetProperties()){
		QWidget* propLayput = CreateProperty(prop->GetName());
	}

	show();
}

void MaterialView::Clear(){
	QWidget* propertyLayout = NULL;
	do {
		delete propertyLayout;
		propertyLayout = properties_box->findChild<QWidget*>("propertyLayout");
	} while(propertyLayout);
}

QWidget* MaterialView::CreateProperty(const string& name){
	QWidget* propertyLayoutWidget = new QWidget(properties_box);
	propertyLayoutWidget->setObjectName("propertyLayout");
	QHBoxLayout* propertyLayout = new QHBoxLayout(propertyLayoutWidget);
	propertyLayout->setSpacing(12);
	propertyLayout->setMargin(0);
	QLabel* propertyNameLabel = new QLabel(propertyLayoutWidget);
	propertyNameLabel->setText(name.c_str());
	QLineEdit* valueBox = new QLineEdit(propertyLayoutWidget);
	propertyLayout->addWidget(propertyNameLabel);
	propertyLayout->addWidget(valueBox);

	QVBoxLayout* groupBoxLayout = dynamic_cast<QVBoxLayout*>(properties_box->layout());
	groupBoxLayout->insertWidget(groupBoxLayout->count() - 1, propertyLayoutWidget);


	propertyLayoutWidget->show();

	return propertyLayoutWidget;
}