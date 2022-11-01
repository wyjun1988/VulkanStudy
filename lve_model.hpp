#pragma once

// read the vertex data from cpu. allocate gpu memory and upload it

#include "lve_device.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

namespace lve{
    class LveModel{
        public:

            struct Vertex{
                glm::vec2 position;

                static std::vector<VkVertexInputBindingDescription> getBindingDescriptions();
                static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();
            };
            LveModel(LveDevice &device, const std::vector<Vertex> &vertices);
            ~LveModel();

            LveModel(const LveModel &) = delete;
            LveModel &operator = (const LveModel &) = delete;

            void bind(VkCommandBuffer commandBuffer);
            void draw(VkCommandBuffer commandBuffer);


        private:
            void createVertexBuffers(const std::vector<Vertex> &vertices);

            LveDevice &lveDevice;
            VkBuffer vertexBuffer;
            VkDeviceMemory vertexBufferMemory; // Vulkan directly manage grpahic memory
            uint32_t vertexCount;
    };
}