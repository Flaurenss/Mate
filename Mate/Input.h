#pragma once
#include <glfw/glfw3.h>
#include <string>
#include <unordered_map>

/* Function keys */
enum class KeyCode {
    None = 0,
    A, B, C, D, E, F, G, H, I, J, K, L, M,
    N, O, P, Q, R, S, T, U, V, W, X, Y, Z,

    Space,
    LeftShift,
    LeftControl,
};

class Input
{
public:
	static void Init(GLFWwindow* window);

    static void Update();

    static bool GetKeyDown(KeyCode key);
    static bool GetKey(KeyCode key);
    static bool GetKeyUp(KeyCode key);

private:
    static GLFWwindow* ptrWindow;
    static std::unordered_map<int, bool> currentKeysMap;
    static std::unordered_map<int, bool> previousKeysMap;

    // TODO: implement mouse input 
    //static std::unordered_map<int, bool> currentMouseButtonsMap;
    //static std::unordered_map<int, bool> previousMouseButtonsMap;

    static int MapToGLFWKey(KeyCode keyCode);
};

