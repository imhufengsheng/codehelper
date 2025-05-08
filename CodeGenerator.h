#include <string>

// Provides static methods for generating code completion suggestions
class CodeGenerator
{
public:
    //Generate code completion suggestions based on prompt
    
    
    static std::string generateCode(const std::string& prompt);

private:
    static const std::string rolePrompt; ///Role prompt text
};
