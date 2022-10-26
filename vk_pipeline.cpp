#include "vk_pipline.hpp"


//std
#include <fstream>
#include <stdexcept>
#include <iostream>

namespace vk{

    VkPipeline::VkPipeline(const std::string& vertFilePath, const std::string& fragFilePath){
        createGraphicsPipeline(vertFilePath, fragFilePath);
    }

    std::vector<char> VkPipeline::readFile(const std::string& filepath){
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

    void VkPipeline::createGraphicsPipeline(const std::string& vertFilePath, const std::string& fragFilePath){
        auto vertCode = readFile(vertFilePath);
        auto fragCode = readFile(fragFilePath);

        std::cout<<"vertex shader code size: " << vertCode.size() << std::endl;
        std::cout<<"fragment shader code size: " << fragCode.size() << std::endl;

    }
}
