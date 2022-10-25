#pragma once

#include "vk_window.hpp"

namespace vk{
    class FirstApp
    {
    private:
        VkWindow vkWindow{WIDTH, HEIGHT, "hello vulkan~"};

    public:

        static constexpr int WIDTH = 800;
        static constexpr int HEIGHT = 600;

        void run();

    };
    
}