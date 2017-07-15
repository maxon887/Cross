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
	try{
		WINSystem* launcherWIN = (WINSystem*)sys;
		QSize size = this->frameSize();
		launcherWIN->SetWindowSize(size.width(), size.height());
	
		gfxGL = new GraphicsGL();
		gfx2D = new Graphics2D();
		gfx3D = new Graphics3D();
		game->Start();
		game->SetScreen(game->GetStartScreen());

		auto pTimer = new QTimer(this);
		connect(pTimer, &QTimer::timeout, this, &GLHandler::update); 
		pTimer->start(1000 / 60.0);
	}catch(Exception &exc){
		string msg = string(exc.message) +
			+"\nFile: " + string(exc.filename) +
			+"\nLine: " + to_string(exc.line);
		((CrossEditor*)game)->ExceptionMsgBox(msg.c_str());
	}
}

void GLHandler::update(){
	QOpenGLWidget::update();
}

void GLHandler::paintGL(){
	try{
		game->EngineUpdate();
	} catch(Exception &exc) {
		((CrossEditor*)game)->OnNewSceneClick();
		string msg = string(exc.message) +
			+"\nFile: " + string(exc.filename) +
			+"\nLine: " + to_string(exc.line);
		((CrossEditor*)game)->ExceptionMsgBox(msg.c_str());
	}
}

void GLHandler::resizeGL(int w, int h){
	WINSystem* winLanch = (WINSystem*)sys;
	winLanch->SetWindowSize(w, h);
	//TODO resizing
	//game->WindowResized(w, h);
}

void GLHandler::shutDown(){
	try{
		game->GetCurrentScreen()->Stop();
		game->Stop();
		delete gfx3D;
		delete gfx2D;
		delete gfxGL;
	} catch(Exception &exc) {
		string msg = string(exc.message) +
			+"\nFile: " + string(exc.filename) +
			+"\nLine: " + to_string(exc.line);
		((CrossEditor*)game)->ExceptionMsgBox(msg.c_str());
	}
}

void GLHandler::mousePressEvent(QMouseEvent* eve){
	setFocus();
	input->TargetActionDown((float)eve->x(), (float)eve->y(), 0);
}

void GLHandler::mouseMoveEvent(QMouseEvent* eve){
	input->TargetActionMove((float)eve->x(), (float)eve->y(), 0);
}

void GLHandler::mouseReleaseEvent(QMouseEvent* eve){
	input->TargetActionUp((float)eve->x(), (float)eve->y(), 0);
}

void GLHandler::keyPressEvent(QKeyEvent* key){
	Key k = (Key)key->nativeVirtualKey();
	if(k < Key::MAX_KEY_NUM){
		input->KeyPressed(k);
	}
}

void GLHandler::keyReleaseEvent(QKeyEvent* key){
	Key k = (Key)key->nativeVirtualKey();
	if(k < Key::MAX_KEY_NUM) {
		input->KeyReleased(k);
	}
}