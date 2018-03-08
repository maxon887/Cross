#include "GLHandler.h"

#include "CrossEditor.h"
#include "ESystem.h"
#include "Game.h"
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
	ESystem* esystem = (ESystem*)system;
	if(!esystem->IsPaused()) {
		QSize size = this->frameSize();
		esystem->SetWindowSize(size.width(), size.height());
	
		gfxGL = new GraphicsGL();
		game->Start();

		auto pTimer = new QTimer(this);
		connect(pTimer, &QTimer::timeout, this, &GLHandler::Update); 
		pTimer->start(1000 / 60.0);
	}
}

void GLHandler::Update(){
	QOpenGLWidget::update();
}

void GLHandler::paintGL(){
	ESystem* esystem = (ESystem*)system;
	if(!esystem->IsPaused()) {
		if(!skip_repainting) {
			game->EngineUpdate();
		} else {
			skip_repainting = false;
		}
	}
}

void GLHandler::resizeGL(int w, int h){
	WINSystem* winLanch = (WINSystem*)system;
	winLanch->SetWindowSize(w, h);
	skip_repainting = true;
}

void GLHandler::ShutDown(){
	if(game->GetCurrentScene()) {
		game->GetCurrentScreen()->Stop();
	}
	game->Stop();
	delete gfxGL;
}

void GLHandler::mousePressEvent(QMouseEvent* eve){
	setFocus();
	U32 id = MouseButtonID(eve);
	input->TargetActionDown.Emit((float)eve->x(), (float)eve->y(), id);
}

void GLHandler::mouseMoveEvent(QMouseEvent* eve){
	S32 id = MouseButtonID(eve);
	if(id >= 0){
		input->TargetActionMove.Emit((float)eve->x(), (float)eve->y(), id);
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
	input->TargetActionUp.Emit((float)eve->x(), (float)eve->y(), id);
}

void GLHandler::wheelEvent(QWheelEvent* wheel){
	if(wheel->delta() > 0) {
		input->MouseWheelRoll.Emit(+15.f);
	} else {
		input->MouseWheelRoll.Emit(-15.f);
	}
}

S32 GLHandler::MouseButtonID(QMouseEvent* e){
	switch(e->buttons()) {
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