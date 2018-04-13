#include "Platform/MacOS/MacSystem.h"
#include "Internals/Audio.h"
#include "Internals/GraphicsGL.h"
#include "Game.h"
#include "Input.h"

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
    xPos *= 2;
    yPos *= 2;
    mouse_pos.x = (float)xPos;
    mouse_pos.y = (float)yPos;
    input->TargetActionMove.Emit((float)xPos, (float)yPos, 0);
}

void GLFWMouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
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

int main() {
    system = new MacSystem();

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


    int screenWidth;
    int screenHeight;
    glfwGetFramebufferSize(window, &screenWidth, &screenHeight);
    system->SetWindowSize(screenWidth, screenHeight);

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
    game->Start();
    
    while(!glfwWindowShouldClose(window)){
        glfwPollEvents();
        game->EngineUpdate();
        glfwSwapBuffers(window);
    }

    glfwTerminate();

    delete game;
    delete gfxGL;
    delete audio;
    delete system;
}
