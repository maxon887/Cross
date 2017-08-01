#ifndef MATERIAL_VIEW
#define MATERIAL_VIEW

#include "Cross.h"
#include "PropertyView.h"

class QLabel;

class MaterialView : public PropertyView
{
	Q_OBJECT
public:
	MaterialView(QWidget* parent = 0) : PropertyView(parent) { }
	~MaterialView() override { }

	void Initialize();
	void OnFileSelected(const string& filename);
	
private:
	QLabel* shader				= NULL;
	QGroupBox* properties_box	= NULL;

	void CreateProperty(const string& name);
};

#endif