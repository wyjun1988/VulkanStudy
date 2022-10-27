#pragma once

#include "lve_device.hpp"

#include <string>
#include <vector>


namespace lve{

    struct  PipelineConfigInfo{};
    
    class LvePipeline{
        public:
            LvePipeline(LveDevice& device, const std::string& vertFilePath, const std::string& fragFilePath, const PipelineConfigInfo& configInfo);
            ~LvePipeline(){}

            LvePipeline(const LvePipeline&) = delete;
            void operator=(const LvePipeline&) = delete;

            static PipelineConfigInfo defaultPipelineConfigInfo(uint32_t width, uint32_t height);


        private:
            static std::vector<char> readFile(const std::string& filepath);

            void createGraphicsPipeline(const std::string& vertFilePath, const std::string& fragFilePath, const PipelineConfigInfo& configInfo);

            void createShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule);

            LveDevice& lveDevice; // de reference, aggregation UML
            VkPipeline graphicsPipeline;
            VkShaderModule vertShaderModule;
            VkShaderModule fragShaderModule;

    };
}