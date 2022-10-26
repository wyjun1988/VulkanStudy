#pragma once

#include "vk_window.hpp"
#include "vk_pipline.hpp"

namespace vk{
    class FirstApp
    {
    public:

        static constexpr int WIDTH = 800;
        static constexpr int HEIGHT = 600;

        void run();
    private:
        VkWindow vkWindow{WIDTH, HEIGHT, "hello vulkan~"};
        VkPipeline vkPipeline{"shaders/simple_shader.vert.spv", "shaders/simple_shader.frag.spv"};

    };
    
    
}