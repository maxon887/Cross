#include "SceneView.h"



SceneView::SceneView(QWidget* parent) :
	QOpenGLWidget(parent)
{ }


SceneView::~SceneView()
{ }

void SceneView::initializeGL(){
	glClearColor(1.f, 0.f, 0.f, 1.f);
}

void SceneView::paintGL(){
	glClear(GL_COLOR_BUFFER_BIT);
}