#ifndef GL_HANDLER
#define GL_HANDLER

#include "Cross.h"
#include "GraphicsGL.h"

#include <QtWidgets/qopenglwidget.h>

using namespace cross;

class GLHandler : public QOpenGLWidget
{
	Q_OBJECT
public:
	GLHandler(QWidget* parent = 0);
	~GLHandler();

	void initializeGL();
	void update();
	void paintGL();
	void resizeGL();
	void shutDown();

	void mousePressEvent(QMouseEvent* eve);
	void mouseMoveEvent(QMouseEvent* eve);
	void mouseReleaseEvent(QMouseEvent* eve);
private:
};

#endif // !GL_HANDLER