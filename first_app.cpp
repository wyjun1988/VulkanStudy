#include "first_app.hpp"

//std
#include <stdexcept>

namespace lve{

    FirstApp::FirstApp(){
        loadModels();
        createPipelineLayout();
        recreateSwapChain();
        createCommandBuffers();
    }

    FirstApp::~FirstApp(){
        vkDestroyPipelineLayout(lveDevice.device(), pipelineLayout, nullptr);
    }

    void FirstApp::run(){

        while(!lveWindow.shouldClose()){
            glfwPollEvents();
            drawFrame();
        }

        vkDeviceWaitIdle(lveDevice.device());
    }

    void FirstApp::loadModels(){
        std::vector<LveModel::Vertex> vertices{
            {{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
            {{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
            {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}
        };
        lveModel = std::make_unique<LveModel>(lveDevice, vertices);
    }

    void FirstApp::createPipelineLayout(){
        VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = 0;
        pipelineLayoutInfo.pSetLayouts = nullptr;
        pipelineLayoutInfo.pushConstantRangeCount = 0;
        pipelineLayoutInfo.pPushConstantRanges = nullptr;
        if(vkCreatePipelineLayout(lveDevice.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS){
            throw std::runtime_error("failed to create pipeline layout!");
        }
    }
    void FirstApp::createPipeline(){
        auto pipelineConfig = LvePipeline::defaultPipelineConfigInfo(lveSwapChain->width(), lveSwapChain->height());
        pipelineConfig.renderPass = lveSwapChain->getRenderPass();
        pipelineConfig.pipelineLayout = pipelineLayout;
        lvePipeline = std::make_unique<LvePipeline>(
            lveDevice, "shaders/simple_shader.vert.spv", "shaders/simple_shader.frag.spv", pipelineConfig
        );
    }

    void FirstApp::recreateSwapChain(){
        auto extent = lveWindow.getExtent();
        while(extent.width==0 || extent.height==0){ //minimization
            extent = lveWindow.getExtent();
            glfwWaitEvents(); 
        }

        vkDeviceWaitIdle(lveDevice.device());
        lveSwapChain = nullptr;
        lveSwapChain = std::make_unique<LveSwapChain>(lveDevice, extent);
        createPipeline();
    }

    void FirstApp::createCommandBuffers(){
        commandBuffers.resize(lveSwapChain->imageCount());

        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandPool = lveDevice.getCommandPool();
        allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

        if(vkAllocateCommandBuffers(lveDevice.device(), &allocInfo, commandBuffers.data()) != VK_SUCCESS){
            throw std::runtime_error("failed to allocate command buffer");
        }
    }

    void FirstApp::recordCommandBuffer(int imageIndex){
        VkCommandBufferBeginInfo beginInfo{};
            beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

            if(vkBeginCommandBuffer(commandBuffers[imageIndex], &beginInfo) != VK_SUCCESS){
                throw std::runtime_error("failed to begin recording command buffer");
            }

            VkRenderPassBeginInfo renderPassInfo{};
            renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
            renderPassInfo.renderPass = lveSwapChain->getRenderPass();
            renderPassInfo.framebuffer = lveSwapChain->getFrameBuffer(imageIndex);

            renderPassInfo.renderArea.offset = {0, 0};
            renderPassInfo.renderArea.extent = lveSwapChain->getSwapChainExtent();

            std::array<VkClearValue, 2> clearValues{};
            clearValues[0].color = {0.1f, 0.1f, 0.1f, 1.0f};
            clearValues[1].depthStencil = {1.0f, 0};
            renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
            renderPassInfo.pClearValues = clearValues.data();

            vkCmdBeginRenderPass(commandBuffers[imageIndex], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

            lvePipeline->bind(commandBuffers[imageIndex]);
            lveModel->bind(commandBuffers[imageIndex]);
            lveModel->draw(commandBuffers[imageIndex]);

            vkCmdEndRenderPass(commandBuffers[imageIndex]);

            if(vkEndCommandBuffer(commandBuffers[imageIndex]) != VK_SUCCESS){
                throw std::runtime_error("failed to record command buffer");
            }
    }
    void FirstApp::drawFrame(){
        uint32_t imageIndex;
        auto result = lveSwapChain->acquireNextImage(&imageIndex);

        if(result==VK_ERROR_OUT_OF_DATE_KHR){
            recreateSwapChain();
            return;
        }
        if(result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR){
            throw std::runtime_error("failed to acquire swap chain image!");
        }

        recordCommandBuffer(imageIndex);
        result = lveSwapChain->submitCommandBuffers(&commandBuffers[imageIndex], &imageIndex);
        if(result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || lveWindow.wasWindowResized()){
            lveWindow.resetWindowResizedFlag();
            printf("2\n");
            recreateSwapChain();
            return;
        }

        if (result != VK_SUCCESS){
            throw std::runtime_error("failed to present swap chain image!");
        }

    }
}