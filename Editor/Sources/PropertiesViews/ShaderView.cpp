#include "ShaderView.h"
#include "File.h"

#include <QHBoxLayout>
#include <QPushButton>

ShaderView::ShaderView(QWidget* parent) :
	QGroupBox(parent)
{ }

ShaderView::~ShaderView() { }

void ShaderView::Initialize(){
	macrosies_box = findChild<QGroupBox*>("macrosies");
	macro_template = macrosies_box->findChild<QHBoxLayout*>("macroLayout");
	QPushButton* addMacroBtn = macrosies_box->findChild<QPushButton*>("addMacroBtn");
	connect(addMacroBtn, &QPushButton::clicked, this, &ShaderView::OnAddMacroClicked);

	properties_box = findChild<QGroupBox*>("properties");
	property_template = properties_box->findChild<QHBoxLayout*>("propertyLayout");
	QPushButton* addPropertyBtn = properties_box->findChild<QPushButton*>("addPropertyBtn");
	connect(addPropertyBtn, &QPushButton::clicked, this, &ShaderView::OnAddPropertyClicked);
}

void ShaderView::OnFileSelected(const string& filepath){
	string filename = File::FileFromPath(File::FileWithoutExtension(filepath));
	setTitle(QString("Shader: ") + filename.c_str());
}

void ShaderView::OnAddMacroClicked(){

}

void ShaderView::OnAddPropertyClicked(){

}