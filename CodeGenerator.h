#ifndef CODE_GENERATOR_H
#define CODE_GENERATOR_H

#include "LocalModelCommunicator.h"
#include <string>
#include <map>
#include <vector>

// 代码生成类
class CodeGenerator
{
public:
    // 根据用户的提示词补全代码
    static std::string generateCode(const std::string& prompt);
private:
    static std::map<std::string, std::vector<std::string>> snippets;
    static void initializeSnippets();
    static bool initialized;
};

#endif
