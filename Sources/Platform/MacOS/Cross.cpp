#include "Platform/MacOS/MacSystme.h"
#include "Internals/Audio.h"
#include "Internals/GraphicsGL.h"

#include <iostream>

#include <GLFW/glfw3.h>

using namespace std;
using namespace cross;

const GLuint width = 800;
const GLuint height = 600;

void GLFWErrorCallback(int error, const char* description) {
    cout << "GLFW Error occured - " << error << "\n\t" << description << endl;
}

int main() {
    system = new MacSystem();

    if(!glfwInit()) {
        cout<<"Failed to initialize GLFW"<<endl;
    }

    glfwSetErrorCallback(GLFWErrorCallback);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(width, height, "Cross++", NULL, NULL);
    if(!window){
        cout<<"Failed to create GLFW window"<<endl;
    }
    int screenWidth;
    int screenHeight;
    glfwGetFramebufferSize(window, &screenWidth, &screenHeight);
    
    glfwMakeContextCurrent(window);
    
    //glViewport(0, 0, screenWidth, screenHeight);

    audio = new Audio();
    gfxGL = new GraphicsGL();
    
    while(!glfwWindowShouldClose(window)){
        glfwPollEvents();
        glClearColor(0.5f, 0.2f, 0.2f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);
        glfwSwapBuffers(window);
    }

    glfwTerminate();

    delete gfxGL;
    delete audio;
    delete system;
}
