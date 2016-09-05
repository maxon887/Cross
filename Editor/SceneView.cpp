#include "SceneView.h"

#include "Platform/Windows/LauncherWIN.h"
#include "Game.h"

#include "Graphics3D.h"
#include "Graphics2D.h"

#include "TriangleScene.h"


SceneView::SceneView(QWidget* parent) :
	QOpenGLWidget(parent)
{ }


SceneView::~SceneView()
{ }

void SceneView::initializeGL(){
	LauncherWIN* launcherWIN = new LauncherWIN();
	launcher = launcherWIN;
	launcherWIN->SetWindowSize(200, 200);
	game = new Game();
	gfxGL = new GraphicsGL();
	gfx2D = new Graphics2D();
	gfx3D = new Graphics3D();

	triangle_scene = new TriangleScene();

	game->SetScene(triangle_scene);
}

void SceneView::paintGL(){
	triangle_scene->Update(0.1f);
}