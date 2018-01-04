#include "ShaderView.h"
#include "File.h"
#include "System.h"
#include "Shaders/Shader.h"
#include "Game.h"
#include "Scene.h"

#include <QHBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QComboBox>
#include <QFileDialog>

using namespace cross;

void ShaderView::Initialize(){
	vertex_file = findChild<QLineEdit*>("vertexFile");
	connect(vertex_file, &QLineEdit::textChanged, this, &ShaderView::OnSomethingChanged);
	QPushButton* loadVertexFileBtn = findChild<QPushButton*>("vertexBtn");
	connect(loadVertexFileBtn, &QPushButton::clicked, this, &ShaderView::OnVertexFileClicked);
	fragment_file = findChild<QLineEdit*>("fragmentFile");
	connect(fragment_file, &QLineEdit::textChanged, this, &ShaderView::OnSomethingChanged);
	QPushButton* loadFragmentFileBtn = findChild<QPushButton*>("fragmentBtn");
	connect(loadFragmentFileBtn, &QPushButton::clicked, this, &ShaderView::OnFragmentFileClicked);

	macrosies_box = findChild<QGroupBox*>("macrosies");
	QPushButton* addMacroBtn = macrosies_box->findChild<QPushButton*>("addMacroBtn");
	connect(addMacroBtn, &QPushButton::clicked, this, &ShaderView::OnAddMacroClicked);
	properties_box = findChild<QGroupBox*>("properties");
	QPushButton* addPropertyBtn = properties_box->findChild<QPushButton*>("addPropertyBtn");
	connect(addPropertyBtn, &QPushButton::clicked, this, &ShaderView::OnAddPropertyClicked);

	apply_btn = findChild<QPushButton*>("applyBtn");
	apply_btn->setDisabled(true);
	connect(apply_btn, &QPushButton::clicked, this, &ShaderView::OnApplyClick);
	revert_btn = findChild<QPushButton*>("revertBtn");
	revert_btn->setDisabled(true);
	connect(revert_btn, &QPushButton::clicked, this, &ShaderView::OnRevertClick);
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
	QString path = QDir::currentPath() + "/" + QString(system->AssetsPath().c_str());
	QString filePath = QFileDialog::getOpenFileName(this, "Select Vertex File", path, "Vertex File (*.vtx)");
	if(!filePath.isEmpty()) {
		QDir root = path;
		QString filepath = root.relativeFilePath(filePath);
		vertex_file->setText(filepath);
	}
}

void ShaderView::OnFragmentFileClicked() {
	QString path = QDir::currentPath() + "/" + QString(system->AssetsPath().c_str());
	QString filePath = QFileDialog::getOpenFileName(this, "Select Fragment File", path, "Fragment File (*.fgm)");
	if(!filePath.isEmpty()) {
		QDir root = path;
		QString filepath = root.relativeFilePath(filePath);
		fragment_file->setText(filepath);
	}
}

void ShaderView::OnFileSelected(const string& filepath){
	if(File::ExtensionFromFile(filepath) != "sha") {
		PropertyView::OnFileSelected(filepath);
		return;
	}
	Clear();

	string filename = File::FileFromPath(File::FileWithoutExtension(filepath));
	setTitle(QString("Shader: ") + filename.c_str());

	delete shader;
	shader = new Shader();
	shader->Load(filepath);
	vertex_file->setText(shader->GetVertexFilename().c_str());
	fragment_file->setText(shader->GetFragmentFilename().c_str());

	for(string macro : shader->GetMacrosies()){
		QWidget* macroLayout = OnAddMacroClicked();
		QLineEdit* macroText = macroLayout->findChild<QLineEdit*>("macroText");
		macroText->setText(macro.c_str());
	}

	for(Shader::Property* prop : shader->GetProperties()){
		QWidget* propLayout = OnAddPropertyClicked();
		QLineEdit* propertyName = propLayout->findChild<QLineEdit*>("propertyName");
		QLineEdit* propertyGLName = propLayout->findChild<QLineEdit*>("propertyGLName");
		QComboBox* propertyType = propLayout->findChild<QComboBox*>("propertyType");
		propertyName->setText(prop->GetName().c_str());
		propertyGLName->setText(prop->GetGLName().c_str());
		switch(prop->GetType()) {
		case Shader::Property::Type::INT:
			propertyType->setCurrentIndex(0);
			break;
		case Shader::Property::Type::FLOAT:
			propertyType->setCurrentIndex(1);
			break;
		case Shader::Property::Type::SAMPLER:
			propertyType->setCurrentIndex(2);
			break;
		case Shader::Property::Type::COLOR:
			propertyType->setCurrentIndex(3);
		default:
			system->LogIt("Property type not supported");
			break;
		}
	}

	apply_btn->setDisabled(true);
	revert_btn->setDisabled(true);

	show();
}

void ShaderView::showEvent(QShowEvent* event) {
	QLabel* vertexLabel = findChild<QLabel*>("vertexLabel");
	QLabel* fragmentLabel = findChild<QLabel*>("fragmentLabel");
	vertexLabel->setMinimumWidth(fragmentLabel->size().width());
}

QWidget* ShaderView::OnAddMacroClicked(){
	QWidget* macroLayoutWidget = new QWidget(macrosies_box);
	macroLayoutWidget->setObjectName("macroLayout");
	QHBoxLayout* macroLayout = new QHBoxLayout(macroLayoutWidget);
	macroLayout->setSpacing(12);
	macroLayout->setMargin(0);
	QLineEdit* macroText = new QLineEdit(macroLayoutWidget);
	macroText->setObjectName("macroText");
	connect(macroText, &QLineEdit::textChanged, this, &ShaderView::OnSomethingChanged);
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

	OnSomethingChanged();

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
	propertyLayout->addWidget(propertyNameLabel);
	QLineEdit* propertyNameEdit = new QLineEdit(propertyLayoutWidget);
	propertyNameEdit->setObjectName("propertyName");
	connect(propertyNameEdit, &QLineEdit::textChanged, this, &ShaderView::OnSomethingChanged);
	propertyLayout->addWidget(propertyNameEdit);
	QLabel* propertyGLNameLabel = new QLabel(propertyLayoutWidget);
	propertyGLNameLabel->setText("glName:");
	propertyLayout->addWidget(propertyGLNameLabel);
	QLineEdit* propertyGLNameEdit = new QLineEdit(propertyLayoutWidget);
	propertyGLNameEdit->setObjectName("propertyGLName");
	connect(propertyGLNameEdit, &QLineEdit::textChanged, this, &ShaderView::OnSomethingChanged);
	propertyLayout->addWidget(propertyGLNameEdit);
	QLabel* typeLabel = new QLabel(propertyLayoutWidget);
	typeLabel->setText("Type:");
	propertyLayout->addWidget(typeLabel);
	QComboBox* typeBox = new QComboBox(propertyLayoutWidget);
	typeBox->setObjectName("propertyType");
	connect(typeBox, &QComboBox::currentTextChanged, this, &ShaderView::OnSomethingChanged);
	typeBox->addItem("Int");
	typeBox->addItem("Float");
	typeBox->addItem("Texture");
	typeBox->addItem("Color");
	propertyLayout->addWidget(typeBox);

	QPushButton* removeBtn = new QPushButton(propertyLayoutWidget);
	removeBtn->setText("remove");
	removeBtn->setFixedWidth(100);
	removeBtn->setFixedHeight(31);
	connect(removeBtn, &QPushButton::clicked, this, &ShaderView::OnRemoveClicked);
	propertyLayout->addWidget(removeBtn);

	QVBoxLayout* groupBoxLayout = dynamic_cast<QVBoxLayout*>(properties_box->layout());
	groupBoxLayout->insertWidget(groupBoxLayout->count() - 1, propertyLayoutWidget);
	propertyLayoutWidget->show();

	OnSomethingChanged();

	return propertyLayoutWidget;
}

void ShaderView::OnRemoveClicked(){
	QPushButton* button = dynamic_cast<QPushButton*>(sender());
	QWidget* parent = dynamic_cast<QWidget*>(button->parent());

	macrosies_box->layout()->removeWidget(parent);

	delete parent;

	OnSomethingChanged();
}

void ShaderView::OnSomethingChanged(){
	apply_btn->setDisabled(false);
	revert_btn->setDisabled(false);
}

void ShaderView::OnApplyClick(){
	shader->SetVertexFilename(vertex_file->text().toStdString());
	shader->SetFragmentFilename(fragment_file->text().toStdString());

	shader->ClearMacrosies();
	QList<QWidget*> macrosies = macrosies_box->findChildren<QWidget*>("macroLayout");
	for(QWidget* macroL : macrosies){
		QLineEdit* macroEdit = macroL->findChild<QLineEdit*>("macroText");
		shader->AddMacro(macroEdit->text().toStdString());
	}

	shader->ClearProperties();
	QList<QWidget*> properties = properties_box->findChildren<QWidget*>("propertyLayout");
	for(QWidget* propertyL : properties){
		QLineEdit* propertyName = propertyL->findChild<QLineEdit*>("propertyName");
		QLineEdit* propertyGLName = propertyL->findChild<QLineEdit*>("propertyGLName");
		QComboBox* propertyType = propertyL->findChild<QComboBox*>("propertyType");
		string name = propertyName->text().toStdString();
		string glName = propertyGLName->text().toStdString();
		switch(propertyType->currentIndex()) {
		case 0://Int 
			shader->AddProperty(new Shader::Property(name, glName, Shader::Property::INT));
			break;
		case 1://Float
			shader->AddProperty(new Shader::Property(name, glName, Shader::Property::FLOAT));
			break;
		case 2://Texture
			shader->AddProperty(new Shader::Property(name, glName, Shader::Property::SAMPLER));
			break;
		case 3://Color
			shader->AddProperty(new Shader::Property(name, glName, Shader::Property::COLOR));
			break;
		default:
			break;
		}
	}

	shader->Save(system->AssetsPath() + shader->GetFilename());
	OnRevertClick();
	game->GetCurrentScene()->ResetMaterials();
}

void ShaderView::OnRevertClick(){
	OnFileSelected(string(shader->GetFilename()));
}