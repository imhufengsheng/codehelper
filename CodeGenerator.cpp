#include "CodeGenerator.h"
#include "NetworkRequest.h"
#include "../include/nlohmann/json.hpp"
#include <iostream>
using json = nlohmann::json;

const std::string CodeGenerator::rolePrompt = "only generate code";

std::string CodeGenerator::generateCode(const std::string& prompt)
{
    std::string url = "http://localhost:11434/api/generate";

    json request = {
        {"model", "qwen2.5-coder:0.5b"},
        {"prompt", rolePrompt + "\n" + prompt},
        {"stream", false}
    };

    std::string response;
    if (NetworkRequest::post(url, request.dump(), response))
    {
        try {
            auto responseJson = json::parse(response);
            if (responseJson.contains("response")) {
                return responseJson["response"];
            }
            else {
                std::cerr << "Response does not contain 'response' field" << std::endl;  // Error message
            }
        }
        catch (const json::parse_error& e) {
            std::cerr << "JSON parsing error: " << e.what() << std::endl;  // Error message
        }
    } else {
        std::cerr << "Network request failed" << std::endl;  // Error message
    }
    return "";
}


