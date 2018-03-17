#include "Platform/MacOS/MacSystme.h"
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

    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow* window = glfwCreateWindow(width, height, "Cross++", NULL, NULL);
    if(!window){
        cout<<"Failed to create GLFW window"<<endl;
    }
    int screenWidth;
    int screenHeight;
    glfwGetFramebufferSize(window, &screenWidth, &screenHeight);
    
    glfwMakeContextCurrent(window);
    
    //if(glewInit()){
    //    std::cout<<"Failed to initialize GLEW"<<std::endl;
    //}
    
    glViewport(0, 0, screenWidth, screenHeight);
    
    while(!glfwWindowShouldClose(window)){
        glfwPollEvents();
        glClearColor(0.5f, 0.2f, 0.2f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);
        glfwSwapBuffers(window);
    }

    glfwTerminate();

    delete system;
}
