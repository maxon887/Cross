#pragma once

#include "GraphicsGL.h"

#include <QtWidgets/qopenglwidget.h>
#include "Cross.h"

using namespace cross;

class SceneView : public QOpenGLWidget
{
public:
	SceneView(QWidget* parent = 0);
	~SceneView();

protected:
	void initializeGL();
	void paintGL();
private:
	Scene* triangle_scene;
};

