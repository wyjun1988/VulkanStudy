#include "first_app.hpp"

namespace vk{

    void FirstApp::run(){

        while(!vkWindow.shouldClose()){
            glfwPollEvents();
        }
    }
}