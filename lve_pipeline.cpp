#include "lve_pipeline.hpp"


//std
#include <fstream>
#include <stdexcept>
#include <iostream>

namespace lve{

    LvePipeline::LvePipeline(LveDevice& device, const std::string& vertFilePath, 
                            const std::string& fragFilePath, const PipelineConfigInfo& configInfo) : lveDevice(device){
        createGraphicsPipeline(vertFilePath, fragFilePath, configInfo);
    }

    std::vector<char> LvePipeline::readFile(const std::string& filepath){
        std::ifstream file{filepath, std::ios::ate | std::ios::binary};//atd:  when the file opened we seek to the end immediately to get file size, binary to avoid unwanted text transformation 

        if(!file.is_open()){
            throw std::runtime_error ("failed to open file: " + filepath);
        }

        size_t fileSize = static_cast<size_t>(file.tellg()); //tellg: get the position. because of ate flag, we can get the last position
        std::vector<char> buffer(fileSize);

        file.seekg(0);
        file.read(buffer.data(), fileSize);

        file.close();
        return buffer; 

    }

    void LvePipeline::createGraphicsPipeline(const std::string& vertFilePath, const std::string& fragFilePath, const PipelineConfigInfo& configInfo){
        auto vertCode = readFile(vertFilePath);
        auto fragCode = readFile(fragFilePath);

        std::cout<<"vertex shader code size: " << vertCode.size() << std::endl;
        std::cout<<"fragment shader code size: " << fragCode.size() << std::endl;
    }

    void LvePipeline::createShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule){
        VkShaderModuleCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createInfo.codeSize = code.size();
        createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

        if (vkCreateShaderModule(lveDevice.device(), &createInfo, nullptr, shaderModule) != VK_SUCCESS){
            throw std::runtime_error("failed to create shader module");
        }
    }

    PipelineConfigInfo LvePipeline::defaultPipelineConfigInfo(uint32_t width, uint32_t height){
        PipelineConfigInfo configInfo{};

        return configInfo;
    }
}
