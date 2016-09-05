#pragma once

#include <QtWidgets/qopenglwidget.h>

class SceneView : public QOpenGLWidget
{
public:
	SceneView(QWidget* parent = 0);
	~SceneView();

protected:
	void initializeGL();
	void paintGL();
};

