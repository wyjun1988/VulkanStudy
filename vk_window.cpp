#include "vk_window.hpp"

namespace vk{

    VkWindow::VkWindow(int w, int h, std::string name) : width{w}, height{h}, windowName{name} {
        initWindow();
    }

    VkWindow::~VkWindow(){
        glfwDestroyWindow(window);
        glfwTerminate();
    }

    void VkWindow::initWindow(){
        glfwInit();
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

        window = glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);
    }

}