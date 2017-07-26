#ifndef SHADER_PROPERTIES
#define SHADER_PROPERTIES

#include "Cross.h"

#include <QGroupBox.h>

using namespace cross;

class ShaderView : public QGroupBox
{
	Q_OBJECT
public:
	ShaderView(QWidget* parent = 0);
	~ShaderView();

	void Initialize();
};

#endif