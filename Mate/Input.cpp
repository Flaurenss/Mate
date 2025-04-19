#include "Input.h"
#include "Logger.h"

Vector3 Input::MousePosition = Vector3::Zero;

GLFWwindow* Input::ptrWindow = nullptr;
std::unordered_map<int, bool> Input::currentKeysMap;
std::unordered_map<int, bool> Input::previousKeysMap;

int Input::MapToGLFWKey(KeyCode keyCode)
{
    switch (keyCode) {
        case KeyCode::A: return GLFW_KEY_A;
        case KeyCode::B: return GLFW_KEY_B;
        case KeyCode::C: return GLFW_KEY_C;
        case KeyCode::D: return GLFW_KEY_D;
        case KeyCode::E: return GLFW_KEY_E;
        case KeyCode::F: return GLFW_KEY_F;
        case KeyCode::G: return GLFW_KEY_G;
        case KeyCode::H: return GLFW_KEY_H;
        case KeyCode::I: return GLFW_KEY_I;
        case KeyCode::J: return GLFW_KEY_J;
        case KeyCode::K: return GLFW_KEY_K;
        case KeyCode::L: return GLFW_KEY_L;
        case KeyCode::M: return GLFW_KEY_M;
        case KeyCode::N: return GLFW_KEY_N;
        case KeyCode::O: return GLFW_KEY_O;
        case KeyCode::P: return GLFW_KEY_P;
        case KeyCode::Q: return GLFW_KEY_Q;
        case KeyCode::R: return GLFW_KEY_R;
        case KeyCode::S: return GLFW_KEY_S;
        case KeyCode::T: return GLFW_KEY_T;
        case KeyCode::U: return GLFW_KEY_U;
        case KeyCode::V: return GLFW_KEY_V;
        case KeyCode::W: return GLFW_KEY_W;
        case KeyCode::X: return GLFW_KEY_X;
        case KeyCode::Y: return GLFW_KEY_Y;
        case KeyCode::Z: return GLFW_KEY_Z;

        case KeyCode::Space: return GLFW_KEY_SPACE;
        case KeyCode::LeftShift: return GLFW_KEY_LEFT_SHIFT;
        case KeyCode::LeftControl: return GLFW_KEY_LEFT_CONTROL;

        default: return -1;
    }
}

void Input::Init(GLFWwindow* window)
{
    ptrWindow = window;
    glfwSetCursorPosCallback(window, MouseCallBack);
}

void Input::Update()
{
    if (!ptrWindow)
    {
        Logger::Err("Input class requires an proper initialization.");
    }
    previousKeysMap = currentKeysMap;
    for (int key = GLFW_KEY_SPACE; key <= GLFW_KEY_LAST; key++) {
        int state = glfwGetKey(ptrWindow, key);
        currentKeysMap[key] = (state == GLFW_PRESS || state == GLFW_REPEAT);
    }
}

bool Input::GetKeyDown(KeyCode key)
{
    int glfwKey = MapToGLFWKey(key);
    return currentKeysMap[glfwKey] && !previousKeysMap[glfwKey];
}

bool Input::GetKey(KeyCode key)
{
    int glfwKey = MapToGLFWKey(key);
    return currentKeysMap[glfwKey];
}

bool Input::GetKeyUp(KeyCode key)
{
    int glfwKey = MapToGLFWKey(key);
    return !currentKeysMap[glfwKey] && previousKeysMap[glfwKey];
}

void Input::MouseCallBack(GLFWwindow* window, double xpos, double ypos)
{
    MousePosition = Vector3((float)xpos, (float)ypos, 0);
}