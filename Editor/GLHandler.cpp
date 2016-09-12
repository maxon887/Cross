#include "GLHandler.h"

#include "Platform/Windows/LauncherWIN.h"
#include "Game.h"
#include "Graphics3D.h"
#include "Graphics2D.h"
#include "SceneView.h"

#include <QMouseEvent>
#include <QTimer>

GLHandler::GLHandler(QWidget* parent) :
	QOpenGLWidget(parent)
{ 
	setMouseTracking(true);
}


GLHandler::~GLHandler()
{ }

void GLHandler::initializeGL(){
	LauncherWIN* launcherWIN = (LauncherWIN*)launcher;
	QSize size = this->frameSize();
	launcherWIN->SetWindowSize(size.width(), size.height());
	
	gfxGL = new GraphicsGL();
	gfx2D = new Graphics2D();
	gfx3D = new Graphics3D();
	game->Start();
	game->SetScene(new SceneView());

	auto pTimer = new QTimer(this);
	connect(pTimer, &QTimer::timeout, this, &GLHandler::update); 
	pTimer->start(1000 / 60.0);
}

void GLHandler::update(){
	QOpenGLWidget::update();
}

void GLHandler::paintGL(){
	//QOpenGLWidget::paintGL();
	game->Update();
}

void GLHandler::shutDown(){
	game->GetCurrentScreen()->Stop();
	game->Stop();
	delete gfx3D;
	delete gfx2D;
	delete gfxGL;
}

void GLHandler::mousePressEvent(QMouseEvent* eve){
	TRIGGER_EVENT(input->TargetActionDown, (float)eve->x(), (float)eve->y(), 0);
}

void GLHandler::mouseMoveEvent(QMouseEvent* eve){
	TRIGGER_EVENT(input->TargetActionMove, (float)eve->x(), (float)eve->y(), 0);
}

void GLHandler::mouseReleaseEvent(QMouseEvent* eve){
	TRIGGER_EVENT(input->TargetActionUp, (float)eve->x(), (float)eve->y(), 0);
}