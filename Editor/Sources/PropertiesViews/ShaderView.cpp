#include "ShaderView.h"
#include "File.h"
#include "System.h"

#include "Libs/TinyXML2/tinyxml2.h"

#include <QHBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QComboBox>

using namespace cross;
using namespace tinyxml2;

ShaderView::ShaderView(QWidget* parent) :
	QGroupBox(parent)
{ }

ShaderView::~ShaderView() { }

void ShaderView::Initialize(){
	vertex_file = findChild<QLineEdit*>("vertexFile");
	QPushButton* loadVertexFileBtn = findChild<QPushButton*>("vertexBtn");
	connect(loadVertexFileBtn, &QPushButton::clicked, this, &ShaderView::OnVertexFileClicked);
	fragment_file = findChild<QLineEdit*>("fragmentFile");
	QPushButton* loadFragmentFileBtn = findChild<QPushButton*>("fragmentBtn");
	connect(loadFragmentFileBtn, &QPushButton::clicked, this, &ShaderView::OnFragmentFileClicked);

	macrosies_box = findChild<QGroupBox*>("macrosies");
	QPushButton* addMacroBtn = macrosies_box->findChild<QPushButton*>("addMacroBtn");
	connect(addMacroBtn, &QPushButton::clicked, this, &ShaderView::OnAddMacroClicked);
	properties_box = findChild<QGroupBox*>("properties");
	QPushButton* addPropertyBtn = properties_box->findChild<QPushButton*>("addPropertyBtn");
	connect(addPropertyBtn, &QPushButton::clicked, this, &ShaderView::OnAddPropertyClicked);
}

void ShaderView::Clear(){
	QWidget* macroLayout = NULL;
	do{
		delete macroLayout;
		macroLayout = macrosies_box->findChild<QWidget*>("macroLayout");
	} while(macroLayout);

	QWidget* propertyLayout = NULL;
	do {
		delete propertyLayout;
		propertyLayout = properties_box->findChild<QWidget*>("propertyLayout");
	} while(propertyLayout);
}

void ShaderView::OnVertexFileClicked() {

}

void ShaderView::OnFragmentFileClicked() {

}

void ShaderView::OnFileSelected(const string& filepath){
	Clear();

	string filename = File::FileFromPath(File::FileWithoutExtension(filepath));
	setTitle(QString("Shader: ") + filename.c_str());


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

	XMLElement* shaderXML = doc.FirstChildElement("Shader");
	XMLElement* vertexXML = shaderXML->FirstChildElement("Vertex");
	const char* vertex = vertexXML->Attribute("filename");
	XMLElement* fragmentXML = shaderXML->FirstChildElement("Fragment");
	const char* fragment = fragmentXML->Attribute("filename");

	vertex_file->setText(vertex);
	fragment_file->setText(fragment);

	XMLElement* macrosiesXML = shaderXML->FirstChildElement("Macrosies");
	if(macrosiesXML){
		XMLElement* macroXML = macrosiesXML->FirstChildElement("Macro");
		while(macroXML){
			const char* text = macroXML->GetText();
			QWidget* macroLayoutWidget = OnAddMacroClicked();
			QLineEdit* macroText = macroLayoutWidget->findChild<QLineEdit*>("macroText");
			macroText->setText(text);
			macroXML = macroXML->NextSiblingElement("Macro");
		}
	}
	XMLElement* propertiesXML = shaderXML->FirstChildElement("Properties");
	if(propertiesXML){
		XMLElement* propertyXML = propertiesXML->FirstChildElement("Property");
		while(propertyXML){
			const char* name = propertyXML->Attribute("name");
			const char* glName = propertyXML->Attribute("glName");
			QWidget* propertyLayout = OnAddPropertyClicked();
			QLineEdit* propertyName = propertyLayout->findChild<QLineEdit*>("propertyName");
			QLineEdit* propertyGLName = propertyLayout->findChild<QLineEdit*>("propertyGLName");
			propertyName->setText(name);
			propertyGLName->setText(glName);
			propertyXML = propertyXML->NextSiblingElement("Property");
		}
	}
}

QWidget* ShaderView::OnAddMacroClicked(){
	QWidget* macroLayoutWidget = new QWidget(macrosies_box);
	macroLayoutWidget->setObjectName("macroLayout");
	QHBoxLayout* macroLayout = new QHBoxLayout(macroLayoutWidget);
	macroLayout->setSpacing(12);
	macroLayout->setMargin(0);
	QLineEdit* macroText = new QLineEdit(macroLayoutWidget);
	macroText->setObjectName("macroText");
	QPushButton* removeBtn = new QPushButton(macroLayoutWidget);
	removeBtn->setText("remove");
	removeBtn->setFixedWidth(100);
	removeBtn->setFixedHeight(31);
	connect(removeBtn, &QPushButton::clicked, this, &ShaderView::OnRemoveClicked);
	macroLayout->addWidget(macroText);
	macroLayout->addWidget(removeBtn);

	QVBoxLayout* groupBoxLayout = dynamic_cast<QVBoxLayout*>(macrosies_box->layout());
	groupBoxLayout->insertWidget(groupBoxLayout->count() - 1, macroLayoutWidget);
	macroLayoutWidget->show();
	macroText->show();
	removeBtn->show();

	return macroLayoutWidget;
}

QWidget* ShaderView::OnAddPropertyClicked(){
	QWidget* propertyLayoutWidget = new QWidget(properties_box);
	propertyLayoutWidget->setObjectName("propertyLayout");
	QHBoxLayout* propertyLayout = new QHBoxLayout(propertyLayoutWidget);
	propertyLayout->setSpacing(12);
	propertyLayout->setMargin(0);
	QLabel* propertyNameLabel = new QLabel(propertyLayoutWidget);
	propertyNameLabel->setText("Name:");
	QLineEdit* propertyNameEdit = new QLineEdit(propertyLayoutWidget);
	propertyNameEdit->setObjectName("propertyName");
	QLabel* propertyGLNameLabel = new QLabel(propertyLayoutWidget);
	propertyGLNameLabel->setText("glName:");
	QLineEdit* propertyGLNameEdit = new QLineEdit(propertyLayoutWidget);
	propertyGLNameEdit->setObjectName("propertyGLName");
	QLabel* typeLabel = new QLabel(propertyLayoutWidget);
	typeLabel->setText("Type:");
	QComboBox* typeBox = new QComboBox(propertyLayoutWidget);
	typeBox->addItem("Float");
	typeBox->addItem("Int");
	typeBox->addItem("Texture");

	QPushButton* removeBtn = new QPushButton(propertyLayoutWidget);
	removeBtn->setText("remove");
	removeBtn->setFixedWidth(100);
	removeBtn->setFixedHeight(31);
	connect(removeBtn, &QPushButton::clicked, this, &ShaderView::OnRemoveClicked);

	propertyLayout->addWidget(propertyNameLabel);
	propertyLayout->addWidget(propertyNameEdit);
	propertyLayout->addWidget(propertyGLNameLabel);
	propertyLayout->addWidget(propertyGLNameEdit);
	propertyLayout->addWidget(typeLabel);
	propertyLayout->addWidget(typeBox);
	propertyLayout->addWidget(removeBtn);

	QVBoxLayout* groupBoxLayout = dynamic_cast<QVBoxLayout*>(properties_box->layout());
	groupBoxLayout->insertWidget(groupBoxLayout->count() - 1, propertyLayoutWidget);
	propertyLayoutWidget->show();
	propertyNameLabel->show();
	propertyNameEdit->show();
	propertyGLNameLabel->show();
	propertyGLNameEdit->show();
	removeBtn->show();

	return propertyLayoutWidget;
}

void ShaderView::OnRemoveClicked(){
	QPushButton* button = dynamic_cast<QPushButton*>(sender());
	QWidget* parent = dynamic_cast<QWidget*>(button->parent());

	macrosies_box->layout()->removeWidget(parent);

	delete parent;
}