#include "Engine/application.h"
#include "Engine/window.h"
#include "Warmup/App.h"

// int main(int argc, char *argv[]) {
//     std::unique_ptr<Window> m_window = std::make_unique<Window>();

//     std::cout<<"Start"<<std::endl;
//     // Pass the app into window to run the game
//     if(m_window->start(0) != 0){
//         return -1;
//     }

//     std::cout<<"Loop"<<std::endl;
//     m_window->loop();
// }

#include "Engine/glu/gl2d.h"
#include <GLFW/glfw3.h>
#include <stb_image/stb_image.h>
#include <stb_truetype/stb_truetype.h>
#include <iostream>
#include <ctime>
#include "Engine/glu/include/platform/platformTools.h"
#include "Engine/glu/include/config.h"
#include "Engine/glu/include/platform/platformInput.h"
#include "Engine/glu/include/platform/otherPlatformFunctions.h"
#include "Engine/glu/include/gameLayer/gameLayer.h"
#include <fstream>
#include <chrono>

#define REMOVE_IMGUI 0

#if REMOVE_IMGUI == 0
	#include "imgui.h"
	#include "backends/imgui_impl_glfw.h"
	#include "backends/imgui_impl_opengl3.h"
	#include "imguiThemes.h"
#endif

#ifdef _WIN32
#include <Windows.h>
#endif

#undef min
#undef max

#pragma region globals 
bool currentFullScreen = 0;
bool fullScreen = 0;

#pragma endregion



void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
	
	if ((action == GLFW_REPEAT || action == GLFW_PRESS) && key == GLFW_KEY_BACKSPACE)
		platform::internal::addToTypedInput(8);

	bool state = 0;
	if(action == GLFW_PRESS) state = 1;
	else if(action == GLFW_RELEASE) state = 0;
	else return;
	
	if(key >= GLFW_KEY_A && key <= GLFW_KEY_Z) {
		int index = key - GLFW_KEY_A;
		platform::internal::setButtonState(platform::Button::A + index, state);
	} else if (key >= GLFW_KEY_0 && key <= GLFW_KEY_9) {
		int index = key - GLFW_KEY_0;
		platform::internal::setButtonState(platform::Button::NR0 + index, state);
	} else {

		if (key == GLFW_KEY_SPACE) platform::internal::setButtonState(platform::Button::Space, state);
		else if (key == GLFW_KEY_ENTER) platform::internal::setButtonState(platform::Button::Enter, state);
		else if (key == GLFW_KEY_ESCAPE) platform::internal::setButtonState(platform::Button::Escape, state);
		else if (key == GLFW_KEY_UP) platform::internal::setButtonState(platform::Button::Up, state);
		else if (key == GLFW_KEY_DOWN) platform::internal::setButtonState(platform::Button::Down, state);
		else if (key == GLFW_KEY_LEFT) platform::internal::setButtonState(platform::Button::Left, state);
		else if (key == GLFW_KEY_RIGHT) platform::internal::setButtonState(platform::Button::Right, state);
		else if (key == GLFW_KEY_LEFT_CONTROL) platform::internal::setButtonState(platform::Button::LeftCtrl, state);
	}
};

void mouseCallback(GLFWwindow *window, int key, int action, int mods) {
	
	bool state = 0;
	if (action == GLFW_PRESS) state = 1;
	else if (action == GLFW_RELEASE) state = 0;
	else return;
	
	if(key == GLFW_MOUSE_BUTTON_LEFT) platform::internal::setLeftMouseState(state);
	else if (key == GLFW_MOUSE_BUTTON_RIGHT) platform::internal::setRightMouseState(state);
}

bool windowFocus = 1;
void windowFocusCallback(GLFWwindow *window, int focused) {
	if (focused) windowFocus = 1;
	else {
		windowFocus = 0;
		platform::internal::resetInputsToZero();
	}
}

void windowSizeCallback(GLFWwindow *window, int x, int y) {
	platform::internal::resetInputsToZero();
}

int mouseMovedFlag = 0;
void cursorPositionCallback(GLFWwindow *window, double xpos, double ypos) {
	mouseMovedFlag = 1;
}

void characterCallback(GLFWwindow *window, unsigned int codepoint) {
	if (codepoint < 127) platform::internal::addToTypedInput(codepoint);
}

#pragma region platform functions
GLFWwindow *wind = 0;
namespace platform {

	void setRelMousePosition(int x, int y) { glfwSetCursorPos(wind, x, y); }
	bool isFullScreen() { return fullScreen; }
	void setFullScreen(bool f) { fullScreen = f; }
	glm::ivec2 getRelMousePosition() {
		double x = 0, y = 0;
		glfwGetCursorPos(wind, &x, &y);
		return { x, y };
	}

	glm::ivec2 getWindowSize() {
		int x = 0; int y = 0;
		glfwGetWindowSize(wind, &x, &y);
		return { x, y };
	}

	void showMouse(bool show) {
		if(show) glfwSetInputMode(wind, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		else glfwSetInputMode(wind, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	}

	bool isFocused() { return windowFocus; }
	bool mouseMoved() { return mouseMovedFlag; }
	bool writeEntireFile(const char *name, void *buffer, size_t size) {
		std::ofstream f(name, std::ios::binary);
		if(!f.is_open()) return 0;
		f.write((char*)buffer, size);
		f.close();
		return 1;
	}

	bool readEntireFile(const char *name, void *buffer, size_t size) {
		std::ifstream f(name, std::ios::binary);
		if (!f.is_open()) return 0;
		f.read((char *)buffer, size);
		f.close();
		return 1;
	}
};
#pragma endregion

int main() {

#ifdef _WIN32
#ifdef _MSC_VER 
#if INTERNAL_BUILD
	AllocConsole();
	(void)freopen("conin$", "r", stdin);
	(void)freopen("conout$", "w", stdout);
	(void)freopen("conout$", "w", stderr);
	std::cout.sync_with_stdio();
#endif
#endif
#endif

static int change = 0;
#pragma region window and opengl

	if(!glfwInit()){
        std::cout<<"GLFW init failed :("<<std::endl;
        return -1;
    }
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
	glfwWindowHint(GLFW_SAMPLES, 4);

	int w = 1000;
	int h = 1000;

	wind = glfwCreateWindow(w, h, "geam", nullptr, nullptr);
	glfwMakeContextCurrent(wind);
    glewExperimental = GL_TRUE;
    std::cout << "GLEWInit status: " << glewInit() << std::endl;

	glfwSwapInterval(1);

	glfwSetKeyCallback(wind, keyCallback);
	glfwSetMouseButtonCallback(wind, mouseCallback);
	glfwSetWindowFocusCallback(wind, windowFocusCallback);
	glfwSetWindowSizeCallback(wind, windowSizeCallback);
	glfwSetCursorPosCallback(wind, cursorPositionCallback);
	glfwSetCharCallback(wind, characterCallback);
	std::unique_ptr<Window> m_window = std::make_unique<Window>(wind);
#pragma endregion

#pragma region gl2d
	gl2d::init();
#pragma endregion


#pragma region imgui
	#if REMOVE_IMGUI == 0
		ImGui::CreateContext();
		//ImGui::StyleColorsDark();
		imguiThemes::embraceTheDarkness();

		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
		//io.ConfigViewportsNoAutoMerge = true;
		//io.ConfigViewportsNoTaskBarIcon = true;
	
		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
			style.Colors[ImGuiCol_WindowBg].w = 0.f;
			style.Colors[ImGuiCol_DockingEmptyBg].w = 0.f;
		}
	
		ImGui_ImplGlfw_InitForOpenGL(wind, true);
		ImGui_ImplOpenGL3_Init("#version 330 core");
	#endif
#pragma endregion

#pragma region initGame
	if (!initGame(change)) return 0;
#pragma endregion
	
	auto stop = std::chrono::high_resolution_clock::now();
	while (!glfwWindowShouldClose(wind)) {
		if(change) {
			change = 0;
			m_window->start(0);
			m_window->loop();
		} else {

			#pragma region deltaTime

			auto start = std::chrono::high_resolution_clock::now();
			float deltaTime = (std::chrono::duration_cast<std::chrono::microseconds>(start - stop)).count() / 1000000.0f;
			stop = std::chrono::high_resolution_clock::now();

			float augmentedDeltaTime = deltaTime;
			if (augmentedDeltaTime > 1.f / 10) { augmentedDeltaTime = 1.f / 10; }
		
			#pragma endregion

			#pragma region imgui
				#if REMOVE_IMGUI == 0
					ImGui_ImplOpenGL3_NewFrame();
					ImGui_ImplGlfw_NewFrame();
					ImGui::NewFrame();
					ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
				#endif
			#pragma endregion

			#pragma region game logic
			if (!gameLogic(augmentedDeltaTime)) {
				closeGame();
				return 0;
			}

			#pragma endregion
			#pragma region fullscreen 

			if (platform::isFocused() && currentFullScreen != fullScreen) {
				static int lastW = w;
				static int lastH = w;
				static int lastPosX = 0;
				static int lastPosY = 0;

				if (fullScreen) {
					lastW = w;
					lastH = h;
					glfwGetWindowPos(wind, &lastPosX, &lastPosY);
					auto monitor = getCurrentMonitor(wind);
					const GLFWvidmode* mode = glfwGetVideoMode(monitor);
					glfwSetWindowMonitor(wind, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
					currentFullScreen = 1;

				} else {
					glfwSetWindowMonitor(wind, nullptr, lastPosX, lastPosY, lastW, lastH, 0);
					currentFullScreen = 0;
				}

			}

			#pragma endregion
			#pragma region reset flags

			mouseMovedFlag = 0;
			platform::internal::updateAllButtons(deltaTime);
			platform::internal::resetTypedInput();

			#pragma endregion
			#pragma region window stuff
			#pragma region imgui
			#if REMOVE_IMGUI == 0
			ImGui::Render();
			int display_w, display_h;
			glfwGetFramebufferSize(wind, &display_w, &display_h);
			glViewport(0, 0, display_w, display_h);
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
				GLFWwindow* backup_current_context = glfwGetCurrentContext();
				ImGui::UpdatePlatformWindows();
				ImGui::RenderPlatformWindowsDefault();
				glfwMakeContextCurrent(backup_current_context);
			}
			#endif
			#pragma endregion

			glfwSwapBuffers(wind);
			glfwPollEvents();

			#pragma endregion
		}
	}
	closeGame();
}
