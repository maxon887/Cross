#include "MacOS/MacSystem.h"
#include "Internals/Audio.h"
#include "Internals/GraphicsGL.h"
#include "Game.h"
#include "Input.h"
#include "Screen.h"
#include "Config.h"
#include "Utils/Debugger.h"

#include <iostream>

#include <GLFW/glfw3.h>

#include "Linux/LinuxSystem.h"

using namespace std;
using namespace cross;

Vector2D mouse_pos;
float frame_to_window_ratio;

void GLFWErrorCallback(int error, const char* description) {
    cout << "GLFW Error occurred - " << error << "\n\t" << description << endl;
}

void GLFWResizeCallback(GLFWwindow* win, int width, int height) {
    cross::os->SetWindowSize(width, height);
}

void GLFWMouseMoveCallback(GLFWwindow* win, double xPos, double yPos) {
    if(input) {
        xPos *= frame_to_window_ratio;
        yPos *= frame_to_window_ratio;
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
	input->Scroll.Emit((float)yoffset);
}

void GLFWKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	Key mappedKey = (Key)key;
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
		mappedKey = Key::BACKSPACE;
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
	case GLFW_KEY_LEFT_SUPER:
	case GLFW_KEY_RIGHT_SUPER:
		mappedKey = Key::COMMAND;
		break;
	case GLFW_KEY_LEFT_ALT:
	case GLFW_KEY_RIGHT_ALT:
		mappedKey = Key::OPTION;
		break;
	case GLFW_KEY_LEFT_CONTROL:
	case GLFW_KEY_RIGHT_CONTROL:
		mappedKey = Key::CONTROL;
		break;
	case GLFW_KEY_LEFT_SHIFT:
	case GLFW_KEY_RIGHT_SHIFT:
		mappedKey = Key::SHIFT;
		break;
	case GLFW_KEY_CAPS_LOCK:
		mappedKey = Key::CAPSLOCK;
		break;
	case GLFW_KEY_UP:
		mappedKey = Key::UP;
		break;
	case GLFW_KEY_DOWN:
		mappedKey = Key::DOWN;
		break;
	case GLFW_KEY_LEFT:
		mappedKey = Key::LEFT;
		break;
	case GLFW_KEY_RIGHT:
		mappedKey = Key::RIGHT;
		break;
	case GLFW_KEY_GRAVE_ACCENT:
		mappedKey = Key::TILDE;
		break;
	case GLFW_KEY_APOSTROPHE:
		mappedKey = Key::APOSTROPHE;
		break;
	case GLFW_KEY_COMMA:
		mappedKey = Key::COMMA;
		break;
	case GLFW_KEY_MINUS:
		mappedKey = Key::MINUS;
		break;
	case GLFW_KEY_PERIOD:
		mappedKey = Key::DOT;
		break;
	case GLFW_KEY_SLASH:
		mappedKey = Key::SLASH;
		break;
	case GLFW_KEY_EQUAL:
		mappedKey = Key::PLUS;
		break;
	case GLFW_KEY_LEFT_BRACKET:
		mappedKey = Key::L_SQUARE_BRACKET;
		break;
	case GLFW_KEY_BACKSLASH:
		mappedKey = Key::BACKSLASH;
		break;
	case GLFW_KEY_RIGHT_BRACKET:
		mappedKey = Key::R_SQUARE_BRACKET;
		break;
	case GLFW_KEY_SEMICOLON:
		mappedKey = Key::SEMICOLON;
		break;
	case GLFW_KEY_F1:
		mappedKey = Key::F1;
		break;
	case GLFW_KEY_F2:
		mappedKey = Key::F2;
		break;
	case GLFW_KEY_F3:
		mappedKey = Key::F3;
		break;
	case GLFW_KEY_F4:
		mappedKey = Key::F4;
		break;
	case GLFW_KEY_F5:
		mappedKey = Key::F5;
		break;
	case GLFW_KEY_F6:
		mappedKey = Key::F6;
		break;
	case GLFW_KEY_F7:
		mappedKey = Key::F7;
		break;
	case GLFW_KEY_F8:
		mappedKey = Key::F8;
		break;
	case GLFW_KEY_F9:
		mappedKey = Key::F9;
		break;
	case GLFW_KEY_F10:
		mappedKey = Key::F10;
		break;
	case GLFW_KEY_F11:
		mappedKey = Key::F11;
		break;
	case GLFW_KEY_F12:
		mappedKey = Key::F12;
		break;
	case GLFW_KEY_UNKNOWN:
		return;
	default:
		CROSS_FAIL(key <= (int)Key::MAX_KEY_NUM, "Unknown key pressed with keycode more than cross can handle");
		break;
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
	input->CharEnter.Emit((char)codepoint);
}

GLFWmonitor* GetMonitorForWindow(GLFWwindow* window) {
    int monitorsCount = 0;
    GLFWmonitor** monitors = glfwGetMonitors(&monitorsCount);
    for (int i = 0; i < monitorsCount; i++) {
        GLFWmonitor* monitor = monitors[i];
        int XPos = 0;
        int YPos = 0;
        glfwGetMonitorPos(monitor, &XPos, &YPos);
        const GLFWvidmode* mode = glfwGetVideoMode(monitor);
        Rect screenRect((float)XPos, (float)YPos, (float)mode->width, (float)mode->height);
        int windowXPos = 0;
        int windowYPos = 0;
        glfwGetWindowPos(window, &windowXPos, &windowYPos);
        if (PointInRect(Vector2D((float)windowXPos, (float)windowYPos), screenRect)) {
            return monitor;
        }
    }
    return nullptr;
}

int main(int c, char **args) {
#ifdef CROSS_MEMORY_PROFILE
	cross::MemoryManager::dead = false;
#endif
#ifdef MACOS
    cross::os = CREATE MacSystem(args[0]);
#elif LINUX
	cross::os = CREATE LinuxSystem();
#endif

    if(!glfwInit()) {
        cout<<"Failed to initialize GLFW"<<endl;
    }

    glfwSetErrorCallback(GLFWErrorCallback);

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

    glfwMakeContextCurrent(window);

    audio = CREATE Audio();
    gfxGL = CREATE GraphicsGL();
    game = CrossMain();

	int windowWidth = config->GetInt("WindowWidth", 800);
	int windowHeight = config->GetInt("WindowHeight", 600);
	glfwSetWindowSize(window, windowWidth, windowHeight);
	int posX = config->GetInt("WindowPosX", 200);
	int posY = config->GetInt("WindowPosY", 100);
	glfwSetWindowPos(window, posX, posY);
    int frameWidth = 0;
    int frameHeight = 0;
	glfwGetFramebufferSize(window, &frameWidth, &frameHeight);
	cross::os->SetWindowSize(frameWidth, frameHeight);
    frame_to_window_ratio = (float)frameWidth / (float)windowWidth;

    int widthMM, heightMM;
    GLFWmonitor* monitor = GetMonitorForWindow(window);
    if(!monitor) {
        monitor = glfwGetPrimaryMonitor();
    }
	const char* monitorName = glfwGetMonitorName(monitor);
	os->LogIt("Monitor - #", monitorName);
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);
    glfwGetMonitorPhysicalSize(monitor, &widthMM, &heightMM);
    float dpi = (float)mode->width * frame_to_window_ratio / ((float)widthMM / 25.4f);

#ifdef MACOS
    MacSystem* macSystem = (MacSystem*)cross::os;
    macSystem->SetScreenDPI(dpi);
#ifdef CROSS_CMAKE
	macSystem->SetAppIcon();
#endif
#endif

    game->Start();

    while(!glfwWindowShouldClose(window)){
        glfwPollEvents();
        game->EngineUpdate();
        glfwSwapBuffers(window);
    }

    glfwGetWindowSize(window, &windowWidth, &windowHeight);
	config->SetInt("WindowWidth", windowWidth);
	config->SetInt("WindowHeight", windowHeight);
	glfwGetWindowPos(window, &posX, &posY);
	config->SetInt("WindowPosX", posX);
	config->SetInt("WindowPosY", posY);

	game->GetCurrentScreen()->Stop();
	game->Stop();

    glfwTerminate();

    delete game;
    delete gfxGL;
    delete audio;
    delete cross::os;
	cross::Debugger::Release();
	
#ifdef CROSS_MEMORY_PROFILE
	cross::MemoryManager::Instance()->Dump();
	cross::MemoryManager::dead = false;
#endif
}
