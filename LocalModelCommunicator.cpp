#include "LocalModelCommunicator.h"
#include <sstream>
#include <memory>

bool LocalModelCommunicator::initialized = false;

void LocalModelCommunicator::initializeOllama() {
    if (!initialized) {
        system("start /b ollama serve");
        Sleep(1000);  // Sleep 是 Windows API 函数，需要 windows.h
        initialized = true;
    }
}

std::string LocalModelCommunicator::queryModel(const std::string& prompt) {
    initializeOllama();
    std::string command = "ollama run deepseek-r1:1.5b \"" + prompt + "\"";
    return executeOllamaCommand(command);
}

std::string LocalModelCommunicator::executeOllamaCommand(const std::string& command) {
    char buffer[128];
    std::string result;

    FILE* pipe = _popen(command.c_str(), "r");
    if (!pipe) {
        return "Error: Failed to start Ollama";
    }

    while (fgets(buffer, sizeof(buffer), pipe) != NULL) {
        result += buffer;
    }

    _pclose(pipe);
    return result;
}
