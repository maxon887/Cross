#include "GLHandler.h"

#include "CrossEditor.h"
#include "Platform/Windows/WINSystem.h"
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
	setFocus();
}


GLHandler::~GLHandler()
{ }

void GLHandler::initializeGL(){
	WINSystem* launcherWIN = (WINSystem*)sys;
	QSize size = this->frameSize();
	launcherWIN->SetWindowSize(size.width(), size.height());
	
	gfxGL = new GraphicsGL();
	gfx2D = new Graphics2D();
	gfx3D = new Graphics3D();
	game->Start();
	game->SetScreen(game->GetStartScreen());

	auto pTimer = new QTimer(this);
	connect(pTimer, &QTimer::timeout, this, &GLHandler::Update); 
	pTimer->start(1000 / 60.0);
}

void GLHandler::Update(){
	QOpenGLWidget::update();
}

void GLHandler::paintGL(){
	game->EngineUpdate();
}

void GLHandler::resizeGL(int w, int h){
	WINSystem* winLanch = (WINSystem*)sys;
	winLanch->SetWindowSize(w, h);
	//TODO resizing
	//game->WindowResized(w, h);
}

void GLHandler::ShutDown(){
	game->GetCurrentScreen()->Stop();
	game->Stop();
	delete gfx3D;
	delete gfx2D;
	delete gfxGL;
}

void GLHandler::mousePressEvent(QMouseEvent* eve){
	setFocus();
	U32 id = MouseButtonID(eve);
	input->TargetActionDown((float)eve->x(), (float)eve->y(), id);
}

void GLHandler::mouseMoveEvent(QMouseEvent* eve){
	S32 id = MouseButtonID(eve);
	if(id >= 0){
		input->TargetActionMove((float)eve->x(), (float)eve->y(), id);
	}
}

void GLHandler::mouseReleaseEvent(QMouseEvent* eve){
	S32 id = 0;
	switch(eve->button()) {
	case Qt::LeftButton:
		id = 0;
		break;
	case Qt::RightButton:
		id = 1;
		break;
	case Qt::MidButton:
		id = 2;
		break;
	}
	input->TargetActionUp((float)eve->x(), (float)eve->y(), id);
}

void GLHandler::wheelEvent(QWheelEvent* wheel){
	if(wheel->delta() > 0){
		input->MouseWheelUp();
	}else{
		input->MouseWheelDown();
	}
}

S32 GLHandler::MouseButtonID(QMouseEvent* e){
	switch(e->buttons())	{
	case Qt::LeftButton:
		return 0;
	case Qt::RightButton:
		return 1;
	case Qt::MidButton:
		return 2;
	default:
		return -1;
	}
}