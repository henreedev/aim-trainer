#include "window.h"
#include "Warmup/App.h"

Window::Window(){

}

Window::~Window(){
    std::cout<<"Window destructor"<<std::endl;
    if(m_coreAllocated){
        delete(m_core);
    }
    if(m_windowAllocated){
        glfwDestroyWindow(m_GLFWwindow);
    }
    if(m_glfwInitialized){
        glfwTerminate();
    }
}

int Window::start(int appId){
    // Testing glfw
    if(!glfwInit()){
        std::cout<<"GLFW init failed :("<<std::endl;
        return -1;
    }
    m_glfwInitialized = true;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);

    m_GLFWwindow = glfwCreateWindow(1280, 960, "CS1950U Engine", NULL, NULL);
    if (!m_GLFWwindow)
    {
        std::cout<<"Window Creation Failed :("<<std::endl;
        glfwTerminate();
        return -1;
    }
    m_windowAllocated = true;

    glfwMakeContextCurrent(m_GLFWwindow);

    // Get the primary monitor
    GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
    if (!primaryMonitor) {
        glfwTerminate();
        return -1;
    }

    // Get the video mode of the primary monitor
    const GLFWvidmode* videoMode = glfwGetVideoMode(primaryMonitor);
    if (!videoMode) {
        glfwTerminate();
        return -1;
    }

    // Switch the window to fullscreen mode
//    glfwSetWindowMonitor(m_GLFWwindow, primaryMonitor, 0, 0, videoMode->width, videoMode->height, videoMode->refreshRate);


    Global::graphics.initializeGLEW(); // IMPORTANT: Can't make ANY OpenGL calls before this occurs!
    Global::graphics.initialize();
    int width, height;
    glfwGetWindowSize(m_GLFWwindow, &width, &height);
    Global::graphics.setWindowSize(glm::ivec2(width, height));
    glfwGetFramebufferSize(m_GLFWwindow, &width, &height);
    Global::graphics.setFramebufferSize(glm::ivec2(width, height));

    glfwSwapInterval(1);

    // Set up core now that windowing and opengl are set up
    // Use appId here to decide what app to make
    std::shared_ptr<Application> app;
    app = std::make_shared<App>();
    m_core = new Core(app);
    if(!m_core){
        return -1;
    }
    m_coreAllocated = true;

    // Stores variable in glfw to reference our m_core object. This allows it to be accessed
    // even in static methods such as keyCallback and windowSizeCallback
    glfwSetWindowUserPointer(m_GLFWwindow, m_core);

    glfwSetKeyCallback(m_GLFWwindow, keyCallback);

    glfwSetMouseButtonCallback(m_GLFWwindow, mouseButtonCallback);


    glfwSetCursorPosCallback(m_GLFWwindow, cursorPosCallback);

    glfwSetScrollCallback(m_GLFWwindow, scrollCallback);

    glfwSetWindowSizeCallback(m_GLFWwindow, windowSizeCallback);

    glfwSetFramebufferSizeCallback(m_GLFWwindow, framebufferSizeCallback);

    glfwSetInputMode(m_GLFWwindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    if (glfwRawMouseMotionSupported()){
        glfwSetInputMode(m_GLFWwindow, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
    }

//    glfwSetInputMode(m_GLFWwindow, GLFW_STICKY_KEYS, GLFW_TRUE);
    
    glfwFocusWindow(m_GLFWwindow);

    return 0;
}

int Window::loop(){
    double previous = glfwGetTime();
    while (!glfwWindowShouldClose(m_GLFWwindow))
    {
        glfwPollEvents();
        // calculate deltaTime
        double current = glfwGetTime();
        double deltaTime = current - previous;
        previous = current;
        m_core->update(deltaTime);
        m_core->draw();
        glfwSwapBuffers(m_GLFWwindow);
    }

    return 0;
}

void Window::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods){
//    if(key == GLFW_KEY_ESCAPE){
//        glfwSetWindowShouldClose(window, true);
//    }
    Core* ptr = (Core*)glfwGetWindowUserPointer(window);
    ptr->keyEvent(key, action);
}

void Window::cursorPosCallback(GLFWwindow* window, double xpos, double ypos){
    Core* ptr = (Core*)glfwGetWindowUserPointer(window);
    ptr->mousePosEvent(xpos, ypos);
}

void Window::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods){
    Core* ptr = (Core*)glfwGetWindowUserPointer(window);
    ptr->mouseButtonEvent(button, action);
}

void Window::scrollCallback(GLFWwindow* window, double xoffset, double yoffset){
    Core* ptr = (Core*)glfwGetWindowUserPointer(window);
    ptr->scrollEvent(yoffset);
}

void Window::windowSizeCallback(GLFWwindow* window, int width, int height){
    Core* ptr = (Core*)glfwGetWindowUserPointer(window);
    ptr->windowResizeEvent(width, height);
}

void Window::framebufferSizeCallback(GLFWwindow* window, int width, int height){
    Core* ptr = (Core*)glfwGetWindowUserPointer(window);
    ptr->framebufferResizeEvent(width, height);
}
