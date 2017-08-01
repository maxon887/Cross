#include "MaterialView.h"
#include "File.h"
#include "System.h"

#include "Libs/TinyXML2/tinyxml2.h"

#include <QLabel>
#include <QLineEdit>
#include <QHBoxLayout>

using namespace cross;
using namespace tinyxml2;

void MaterialView::Initialize(){
	shader = findChild<QLabel*>("shaderLabel");
}

void MaterialView::OnFileSelected(const string& filepath){
	if(File::ExtensionFromFile(filepath) != "mat") {
		PropertyView::OnFileSelected(filepath);
		return;
	}

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
	shader->setText(QString("Shader - ") + shaderfilename);

	XMLElement* propertyXML = materialXML->FirstChildElement("Property");
	if(propertyXML){
		const char* name = propertyXML->Attribute("name");


	}

	show();
}

void MaterialView::CreateProperty(const string& name){
	QWidget* propertyLayoutWidget = new QWidget(properties_box);
	propertyLayoutWidget->setObjectName("propertyLayout");
	QHBoxLayout* propertyLayout = new QHBoxLayout(propertyLayoutWidget);
	propertyLayout->setSpacing(12);
	propertyLayout->setMargin(0);
	QLabel* propertyNameLabel = new QLabel(propertyLayoutWidget);
	propertyNameLabel->setText(name.c_str());
	QLineEdit* valueBox = new QLineEdit(propertyLayoutWidget);
}