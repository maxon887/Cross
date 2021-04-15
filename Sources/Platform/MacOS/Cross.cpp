#include "Platform/MacOS/MacSystem.h"
#include "Internals/Audio.h"
#include "Internals/GraphicsGL.h"
#include "Game.h"
#include "Input.h"
#include "Screen.h"
#include "Config.h"

#include <iostream>

#include <GLFW/glfw3.h>

using namespace std;
using namespace cross;

Vector2D mouse_pos;

void GLFWErrorCallback(int error, const char* description) {
    cout << "GLFW Error occured - " << error << "\n\t" << description << endl;
}

void GLFWResizeCallback(GLFWwindow* win, int width, int height) {
    system->SetWindowSize(width, height);
}

void GLFWMouseMoveCallback(GLFWwindow* win, double xPos, double yPos) {
    if(input) {
        xPos *= 2;
        yPos *= 2;
        mouse_pos.x = (float)xPos;
        mouse_pos.y = (float)yPos;
        input->TargetActionMove.Emit((float)xPos, (float)yPos, 0);
    }
}

void GLFWMouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    if(input) {
        switch (action) {
        case GLFW_PRESS:
            input->TargetActionDown.Emit(mouse_pos.x, mouse_pos.y, button);
            break;
        case GLFW_RELEASE:
            input->TargetActionUp.Emit(mouse_pos.x, mouse_pos.y, button);
            break;
        default:
            break;
        }
    }
}

void GLFWScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
	input->Scroll.Emit(yoffset);
}

void GLFWKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	Key mappedKey = Key::ESCAPE;
	switch(key) {
	case GLFW_KEY_ESCAPE:
		mappedKey = Key::ESCAPE;
		break;
	case GLFW_KEY_ENTER:
		mappedKey = Key::ENTER;
		break;
	case GLFW_KEY_TAB:
		mappedKey = Key::TAB;
		break;
	case GLFW_KEY_BACKSPACE:
		mappedKey = Key::BACK;
		break;
	case GLFW_KEY_INSERT:
		mappedKey = Key::INSERT;
		break;
	case GLFW_KEY_DELETE:
		mappedKey = Key::DEL;
		break;
	case GLFW_KEY_HOME:
		mappedKey = Key::HOME;
		break;
	default:
		return;
	}
	switch(action) {
	case GLFW_PRESS:
		input->KeyPressed.Emit(mappedKey);
		break;
	case GLFW_RELEASE:
		input->KeyReleased.Emit(mappedKey);
		break;
	}
}

void GLFWCharCallback(GLFWwindow* window, unsigned int codepoint) {
	input->CharEnter.Emit(codepoint);
}

int main(int c,char **args) {
    system = new MacSystem(args[0]);

    if(!glfwInit()) {
        cout<<"Failed to initialize GLFW"<<endl;
    }

    glfwSetErrorCallback(GLFWErrorCallback);

    //glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    //glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "Cross++", NULL, NULL);
    if(!window){
        cout<<"Failed to create GLFW window"<<endl;
    }
    glfwSetFramebufferSizeCallback(window, GLFWResizeCallback);
    glfwSetCursorPosCallback(window, GLFWMouseMoveCallback);
    glfwSetMouseButtonCallback(window, GLFWMouseButtonCallback);
	glfwSetScrollCallback(window, GLFWScrollCallback);
	glfwSetKeyCallback(window, GLFWKeyCallback);
	glfwSetCharCallback(window, GLFWCharCallback);

    int widthMM, heightMM;
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);
    glfwGetMonitorPhysicalSize(monitor, &widthMM, &heightMM);
    float dpi = mode->width * 2.f / (widthMM / 25.4);
    MacSystem* macSystem = (MacSystem*)system;
    macSystem->SetScreenDPI(dpi);
    
    glfwMakeContextCurrent(window);

    audio = new Audio();
    gfxGL = new GraphicsGL();
    game = CrossMain();

	int width = config->GetInt("WindowWidth", 800);
	int height = config->GetInt("WindowHeight", 600);
	glfwSetWindowSize(window, width / 2.f, height / 2.f);
	int posX = config->GetInt("WindowPosX", 200);
	int posY = config->GetInt("WindowPosY", 100);
	glfwSetWindowPos(window, posX, posY);
	glfwGetFramebufferSize(window, &width, &height);
	system->SetWindowSize(width, height);

    game->Start();
    
    while(!glfwWindowShouldClose(window)){
        glfwPollEvents();
        game->EngineUpdate();
        glfwSwapBuffers(window);
    }

	config->SetInt("WindowWidth", system->GetWindowWidth());
	config->SetInt("WindowHeight", system->GetWindowHeight());
	glfwGetWindowPos(window, &posX, &posY);
	config->SetInt("WindowPosX", posX);
	config->SetInt("WindowPosY", posY);

	game->GetCurrentScreen()->Stop();
	game->Stop();

    glfwTerminate();

    delete game;
    delete gfxGL;
    delete audio;
    delete system;
}
