#include "ShaderView.h"
#include "File.h"
#include "System.h"
#include "Shaders/Shader.h"
#include "Game.h"
#include "Scene.h"
#include "FileHandler.h"

#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QComboBox>
#include <QFileDialog>
#include <QDragEnterEvent>
#include <QMimeData>

using namespace cross;

void ShaderView::Initialize(){
	vertex_handler = findChild<FileHandler*>("vertex_file");
	vertex_handler->SetFileExtension("vtx");
	connect(vertex_handler, &QLineEdit::textChanged, this, &ShaderView::OnSomethingChanged);
	fragment_handler = findChild<FileHandler*>("fragment_file");
	fragment_handler->SetFileExtension("fgm");
	connect(fragment_handler, &QLineEdit::textChanged, this, &ShaderView::OnSomethingChanged);

	macrosies_box = findChild<QGroupBox*>("macrosies");
	QPushButton* addMacroBtn = macrosies_box->findChild<QPushButton*>("addMacroBtn");
	connect(addMacroBtn, &QPushButton::clicked, this, &ShaderView::OnAddMacroClicked);
	properties_box = findChild<QGroupBox*>("properties");
	QPushButton* addPropertyBtn = properties_box->findChild<QPushButton*>("addPropertyBtn");
	connect(addPropertyBtn, &QPushButton::clicked, this, &ShaderView::OnAddPropertyClicked);

	apply_btn = findChild<QPushButton*>("apply_button");
	apply_btn->setDisabled(true);
	connect(apply_btn, &QPushButton::clicked, this, &ShaderView::OnApplyClick);
	revert_btn = findChild<QPushButton*>("revert_button");
	revert_btn->setDisabled(true);
	connect(revert_btn, &QPushButton::clicked, this, &ShaderView::OnRevertClick);

	setAcceptDrops(true);
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

void ShaderView::OnFileSelected(const String& filepath){
	if(File::ExtensionFromFile(filepath) != "sha") {
		PropertyView::OnFileSelected(filepath);
		return;
	}
	Clear();

	String filename = File::FileFromPath(File::FileWithoutExtension(filepath));
	setTitle(QString("Shader: ") + filename.ToCStr());

	delete shader;
	shader = new Shader();
	shader->Load(filepath);
	vertex_handler->SetFile(shader->GetVertexFilename().ToCStr());
	fragment_handler->SetFile(shader->GetFragmentFilename().ToCStr());

	for(String macro : shader->GetMacrosies()){
		QWidget* macroLayout = OnAddMacroClicked();
		QLineEdit* macroText = macroLayout->findChild<QLineEdit*>("macroText");
		macroText->setText(macro.ToCStr());
	}

	for(const Shader::Property& prop : shader->GetProperties()){
		QWidget* propLayout = OnAddPropertyClicked();
		QLineEdit* propertyName = propLayout->findChild<QLineEdit*>("propertyName");
		QLineEdit* propertyGLName = propLayout->findChild<QLineEdit*>("propertyGLName");
		QComboBox* propertyType = propLayout->findChild<QComboBox*>("propertyType");
		propertyName->setText(prop.GetName().ToCStr());
		propertyGLName->setText(prop.GetGLName().ToCStr());
		switch(prop.GetType()) {
		case Shader::Property::Type::INT:
			propertyType->setCurrentIndex(0);
			break;
		case Shader::Property::Type::FLOAT:
			propertyType->setCurrentIndex(1);
			break;
		case Shader::Property::Type::TEXTURE:
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
	removeBtn->setFixedWidth(75);
	removeBtn->setFixedHeight(23);
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
	propertyLayout->setSpacing(6);
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
	removeBtn->setFixedWidth(50);
	removeBtn->setFixedHeight(23);
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
	shader->SetVertexFilename(vertex_handler->GetFile().toLatin1().data());
	shader->SetFragmentFilename(fragment_handler->GetFile().toLatin1().data());

	shader->ClearMacrosies();
	QList<QWidget*> macrosies = macrosies_box->findChildren<QWidget*>("macroLayout");
	for(QWidget* macroL : macrosies){
		QLineEdit* macroEdit = macroL->findChild<QLineEdit*>("macroText");
		shader->AddMacro(macroEdit->text().toLatin1().data());
	}

	shader->ClearProperties();
	QList<QWidget*> properties = properties_box->findChildren<QWidget*>("propertyLayout");
	for(QWidget* propertyL : properties){
		QLineEdit* propertyName = propertyL->findChild<QLineEdit*>("propertyName");
		QLineEdit* propertyGLName = propertyL->findChild<QLineEdit*>("propertyGLName");
		QComboBox* propertyType = propertyL->findChild<QComboBox*>("propertyType");
		String name = propertyName->text().toLatin1().data();
		String glName = propertyGLName->text().toLatin1().data();
		switch(propertyType->currentIndex()) {
		case 0://Int 
			shader->AddProperty(name, glName, Shader::Property::INT);
			break;
		case 1://Float
			shader->AddProperty(name, glName, Shader::Property::FLOAT);
			break;
		case 2://Texture
			shader->AddProperty(name, glName, Shader::Property::TEXTURE);
			break;
		case 3://Color
			shader->AddProperty(name, glName, Shader::Property::COLOR);
			break;
		default:
			break;
		}
	}

	shader->Save(system->AssetsPath() + shader->GetFilename());
	OnRevertClick();
	game->GetCurrentScene()->ResetShaders();
	game->GetCurrentScene()->ResetMaterials();
}

void ShaderView::OnRevertClick(){
	OnFileSelected(shader->GetFilename());
}
