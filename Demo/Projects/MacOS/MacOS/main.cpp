#include <iostream>

#define GLEW_STATIC
#include <GL/glew.h>

#include <GLFW/glfw3.h>

const GLuint width = 800;
const GLuint height = 600;

int main(){
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    
    GLFWwindow* window = glfwCreateWindow(width, height, "Cross++", NULL, NULL);
    if(!window){
        std::cout<<"Failed to create GLFW window"<<std::endl;
    }
    int screenWidth;
    int screenHeight;
    glfwGetFramebufferSize(window, &screenWidth, &screenHeight);
    
    glfwMakeContextCurrent(window);
    
    if(glewInit()){
        std::cout<<"Failed to initialize GLEW"<<std::endl;
    }
    
    glViewport(0, 0, screenWidth, screenHeight);
    
    while(!glfwWindowShouldClose(window)){
        glfwPollEvents();
        glClearColor(0.5f, 0.2f, 0.2f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);
        glfwSwapBuffers(window);
    }
}
