#ifndef GL_HANDLER
#define GL_HANDLER

#include "Cross.h"
#include "Internals/GraphicsGL.h"

#include <QOpenGLWidget>

using namespace cross;

class GLHandler : public QOpenGLWidget
{
	Q_OBJECT
public:
	GLHandler(QWidget* parent = 0);
	~GLHandler();

	void initializeGL() override;
	void paintGL() override;
	void resizeGL(int w, int h) override;

	void mousePressEvent(QMouseEvent* eve) override;
	void mouseMoveEvent(QMouseEvent* eve) override;
	void mouseReleaseEvent(QMouseEvent* eve) override;

	void wheelEvent(QWheelEvent* wheel) override;

	void Update();
	void ShutDown();

protected:
	S32 MouseButtonID(QMouseEvent* e);

private:
	bool skip_repainting = false;
};

#endif // !GL_HANDLER