#ifndef LOCAL_MODEL_COMMUNICATOR_H
#define LOCAL_MODEL_COMMUNICATOR_H

#include <string>
#include <windows.h>

class LocalModelCommunicator {
public:
    static std::string queryModel(const std::string& prompt);
private:
    static std::string executeOllamaCommand(const std::string& command);
    static bool initialized;
    static void initializeOllama();
};

#endif

