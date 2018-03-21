#include "Platform/MacOS/MacSystme.h"
#include "Internals/Audio.h"
#include "Internals/GraphicsGL.h"
#include "Game.h"

#include <iostream>

#include <GLFW/glfw3.h>

using namespace std;
using namespace cross;

void GLFWErrorCallback(int error, const char* description) {
    cout << "GLFW Error occured - " << error << "\n\t" << description << endl;
}

void GLFWResizeCallback(GLFWwindow* win, int width, int height) {
    system->SetWindowSize(width, height);
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


    int screenWidth;
    int screenHeight;
    glfwGetFramebufferSize(window, &screenWidth, &screenHeight);
    system->SetWindowSize(screenWidth, screenHeight);
    
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
