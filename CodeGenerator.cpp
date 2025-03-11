#include "CodeGenerator.h"
#include "LocalModelCommunicator.h"
#include <algorithm>

std::map<std::string, std::vector<std::string>> CodeGenerator::snippets;
bool CodeGenerator::initialized = false;

void CodeGenerator::initializeSnippets() {
    if (initialized) return;

    // 添加更多的代码片段
    snippets["for"] = {
        "for(int i = 0; i < n; i++) {\n    \n}",
        "for(auto& item : container) {\n    \n}"
    };

    snippets["if"] = {
        "if() {\n    \n}",
        "if() {\n    \n} else {\n    \n}"
    };

    snippets["class"] = {
        "class ClassName {\npublic:\n    ClassName();\n    ~ClassName();\n};"
    };

    snippets["switch"] = {
        "switch() {\n    case :\n        break;\n    default:\n        break;\n}"
    };

    snippets["while"] = {
        "while() {\n    \n}"
    };

    snippets["do"] = {
        "do {\n    \n} while();"
    };

    initialized = true;
}

std::string CodeGenerator::generateCode(const std::string& prompt)
{
    initializeSnippets();

    // 首先尝试从预定义片段中匹配
    for (const auto& pair : snippets) {
        if (prompt.find(pair.first) != std::string::npos) {
            return !pair.second.empty() ? pair.second[0] : "";
        }
    }

    // 如果没有匹配的预定义片段，调用本地模型
    return LocalModelCommunicator::queryModel(prompt);
}
