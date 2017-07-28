#ifndef SHADER_VIEW
#define SHADER_VIEW

#include "Cross.h"

#include <QGroupBox.h>

using namespace cross;

class QHBoxLayout;

class ShaderView : public QGroupBox
{
	Q_OBJECT
public:
	ShaderView(QWidget* parent = 0);
	~ShaderView();

	void Initialize();
	void OnFileSelected(const string& filename);
	
private:
	QGroupBox* macrosies_box		= NULL;
	QGroupBox* properties_box		= NULL;

	void OnAddMacroClicked();
	void OnAddPropertyClicked();
	void OnRemoveClicked();
};

#endif