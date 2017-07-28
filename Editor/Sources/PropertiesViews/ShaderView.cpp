#include "ShaderView.h"
#include "File.h"

#include <QHBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>

ShaderView::ShaderView(QWidget* parent) :
	QGroupBox(parent)
{ }

ShaderView::~ShaderView() { }

void ShaderView::Initialize(){
	macrosies_box = findChild<QGroupBox*>("macrosies");
	QPushButton* addMacroBtn = macrosies_box->findChild<QPushButton*>("addMacroBtn");
	connect(addMacroBtn, &QPushButton::clicked, this, &ShaderView::OnAddMacroClicked);
	properties_box = findChild<QGroupBox*>("properties");
	QPushButton* addPropertyBtn = properties_box->findChild<QPushButton*>("addPropertyBtn");
	connect(addPropertyBtn, &QPushButton::clicked, this, &ShaderView::OnAddPropertyClicked);
}

void ShaderView::OnFileSelected(const string& filepath){
	string filename = File::FileFromPath(File::FileWithoutExtension(filepath));
	setTitle(QString("Shader: ") + filename.c_str());
}

void ShaderView::OnAddMacroClicked(){
	QWidget* macroLayoutWidget = new QWidget(macrosies_box);
	QHBoxLayout* macroLayout = new QHBoxLayout(macroLayoutWidget);
	macroLayout->setSpacing(12);
	macroLayout->setMargin(0);
	QLineEdit* macroText = new QLineEdit(macroLayoutWidget);
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
}

void ShaderView::OnAddPropertyClicked(){
	QWidget* propertyLayoutWidget = new QWidget(properties_box);
	QHBoxLayout* propertyLayout = new QHBoxLayout(propertyLayoutWidget);
	propertyLayout->setSpacing(12);
	propertyLayout->setMargin(0);
	QLabel* propertyNameLabel = new QLabel(propertyLayoutWidget);
	propertyNameLabel->setText("Name:");
	QLineEdit* propertyNameEdit = new QLineEdit(propertyLayoutWidget);
	QLabel* propertyGLNameLabel = new QLabel(propertyLayoutWidget);
	propertyGLNameLabel->setText("glName:");
	QLineEdit* propertyGLNameEdit = new QLineEdit(propertyLayoutWidget);
	QPushButton* removeBtn = new QPushButton(propertyLayoutWidget);
	removeBtn->setText("remove");
	removeBtn->setFixedWidth(100);
	removeBtn->setFixedHeight(31);
	connect(removeBtn, &QPushButton::clicked, this, &ShaderView::OnRemoveClicked);

	propertyLayout->addWidget(propertyNameLabel);
	propertyLayout->addWidget(propertyNameEdit);
	propertyLayout->addWidget(propertyGLNameLabel);
	propertyLayout->addWidget(propertyGLNameEdit);
	propertyLayout->addWidget(removeBtn);

	QVBoxLayout* groupBoxLayout = dynamic_cast<QVBoxLayout*>(properties_box->layout());
	groupBoxLayout->insertWidget(groupBoxLayout->count() - 1, propertyLayoutWidget);
	propertyLayoutWidget->show();
	propertyNameLabel->show();
	propertyNameEdit->show();
	propertyGLNameLabel->show();
	propertyGLNameEdit->show();
	removeBtn->show();
}

void ShaderView::OnRemoveClicked(){
	QPushButton* button = dynamic_cast<QPushButton*>(sender());
	QWidget* parent = dynamic_cast<QWidget*>(button->parent());

	macrosies_box->layout()->removeWidget(parent);

	delete parent;
}