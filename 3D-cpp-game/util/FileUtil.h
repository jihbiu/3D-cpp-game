#pragma once
#include <string>
#include <iostream>
#include <sstream>

class FileUtil
{
public:
    static std::string readFile(const std::string& filePath)
    {
        std::ifstream file(filePath);
        if (!file.is_open()) {
            throw std::runtime_error("Could not open file: " + filePath);
        }

        std::string contents;
        file.seekg(0, std::ios::end);
        contents.resize(file.tellg());
        file.seekg(0, std::ios::beg);
        file.read(&contents[0], contents.size());
        file.close();
        return contents;       

        // std::stringstream buffer;
        // buffer << file.rdbuf();
        // file.close();
        //
        // std::string fileContent = buffer.str();
        // GLchar* glCharFileContent = new GLchar[fileContent.length() + 1];
        //
        // std::copy(fileContent.begin(), fileContent.end(), glCharFileContent);
        // glCharFileContent[fileContent.length()] = '\0';
        //
        // return glCharFileContent;
    }
};

