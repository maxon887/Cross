#include "MaterialView.h"
#include "File.h"
#include "System.h"
#include "Shaders/Shader.h"
#include "Material.h"
#include "Game.h"
#include "Scene.h"

#include "Libs/TinyXML2/tinyxml2.h"

#include <iomanip>

#include <QLabel>
#include <QLineEdit>
#include <QSlider>
#include <QHBoxLayout>

using namespace cross;
using namespace tinyxml2;

MaterialView::~MaterialView(){
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

	material = game->GetCurrentScene()->GetMaterial(filepath);

	for(Shader::Property* prop : material->GetProperties()) {
		QWidget* propLayout = CreateProperty(prop->GetName(), prop->GetType());
		switch(prop->GetType()) {
		case Shader::Property::Type::SAMPLER: {
			break;
		}
		case Shader::Property::Type::INT:
		case Shader::Property::Type::FLOAT:
		case Shader::Property::Type::COLOR: {
			Color c(0.f);
			c.SetData((const char*)prop->GetValue());
			int rInt = c.R * 255;
			int gInt = c.G * 255;
			int bInt = c.B * 255;
			int aInt = c.A * 255;

			std::stringstream ss;
			ss << std::hex;
			ss << std::setw(2) << std::setfill('0') << rInt;
			ss << std::setw(2) << std::setfill('0') << gInt;
			ss << std::setw(2) << std::setfill('0') << bInt;
			ss << std::setw(2) << std::setfill('0') << aInt;
			string text = ss.str();
			std::transform(text.begin(), text.end(), text.begin(), ::toupper);

			QLineEdit* valueBox = propLayout->findChild<QLineEdit*>("valueBox");
			valueBox->setText(text.c_str());

			break;
		}
		default:
			break;
		}
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

void MaterialView::OnValueChanged(){
	QLineEdit* valueBox = dynamic_cast<QLineEdit*>(sender());
	QWidget* parent = dynamic_cast<QWidget*>(valueBox->parent());
	QLabel* nameLabel = parent->findChild<QLabel*>("nameLabel");
	
	Shader::Property* prop = material->GetProperty(nameLabel->text().toStdString());
	switch(prop->GetType())	{
	case Shader::Property::COLOR: {
		Color c(valueBox->text().toStdString().c_str());
		prop->SetValue(c);
		break;
	}
	default:
		throw CrossException("Unsupported property type");
	}
}

QWidget* MaterialView::CreateProperty(const string& name, Shader::Property::Type type){
	QWidget* propertyLayoutWidget = new QWidget(properties_box);
	propertyLayoutWidget->setObjectName("propertyLayout");
	QHBoxLayout* propertyLayout = new QHBoxLayout(propertyLayoutWidget);
	propertyLayout->setSpacing(12);
	propertyLayout->setMargin(0);

	QLabel* propertyNameLabel = new QLabel(propertyLayoutWidget);
	propertyNameLabel->setObjectName("nameLabel");
	propertyNameLabel->setText(name.c_str());
	propertyNameLabel->setFixedWidth(250);
	propertyLayout->addWidget(propertyNameLabel);

	switch(type) {
	case cross::Shader::Property::SAMPLER: {
		QLineEdit* filename = new QLineEdit(propertyLayoutWidget);
		filename->setObjectName("filename");
		propertyLayout->addWidget(filename);
		break;
	}
	case cross::Shader::Property::FLOAT: {
		QSlider* valueSlider = new QSlider(propertyLayoutWidget);
		valueSlider->setOrientation(Qt::Horizontal);
		propertyLayout->addWidget(valueSlider);

		QLineEdit* valueBox = new QLineEdit(propertyLayoutWidget);
		valueBox->setObjectName("valueBox");
		propertyLayout->addWidget(valueBox);
		break;
	}
	case cross::Shader::Property::INT: {
		QLineEdit* valueBox = new QLineEdit(propertyLayoutWidget);
		valueBox->setObjectName("valueBox");
		propertyLayout->addWidget(valueBox);
		break;
	}
	case cross::Shader::Property::COLOR: {
		QWidget* colorPicker = new QWidget(propertyLayoutWidget);
		QPalette pal = palette();
		pal.setColor(QPalette::Background, Qt::red);
		colorPicker->setAutoFillBackground(true);
		colorPicker->setPalette(pal);
		colorPicker->setFixedWidth(60);
		colorPicker->setFixedHeight(31);
		colorPicker->show();
		propertyLayout->addWidget(colorPicker);

		QLineEdit* valueBox = new QLineEdit(propertyLayoutWidget);
		valueBox->setObjectName("valueBox");
		valueBox->setFixedWidth(100);
		connect(valueBox, &QLineEdit::returnPressed, this, &MaterialView::OnValueChanged);
		propertyLayout->addWidget(valueBox);
		

		QSpacerItem* spacer = new QSpacerItem(0, 0, QSizePolicy::Expanding);
		propertyLayout->addItem(spacer);
		break;
	}
	default:
		break;
	}

	QVBoxLayout* groupBoxLayout = dynamic_cast<QVBoxLayout*>(properties_box->layout());
	groupBoxLayout->insertWidget(groupBoxLayout->count() - 1, propertyLayoutWidget);

	propertyLayoutWidget->show();

	return propertyLayoutWidget;
}