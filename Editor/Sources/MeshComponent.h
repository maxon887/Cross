#ifndef MESH_COMPONENT
#define MESH_COMPONENT

#include "Cross.h"

#include <QGroupBox.h>

class QLineEdit;

using namespace cross;

class MeshComponent : public QGroupBox
{
	Q_OBJECT
public:
	MeshComponent(QWidget* parent = 0);
	~MeshComponent();

};

#endif