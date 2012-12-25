#include <map>
#include <string>

class ArgMapper{
    public:
        ArgMapper();
        void setSimpleArg(char argName, bool value);
        void setCompoundArg(char argName, std::string value);
        bool getSimpleArg(char argName);
        std::string getCompoundArg(char argName);
    private:
        // stores for the two arg types
        std::map<char, std::string> compoundArgsMap;
        std::map<char, bool> simpleArgsMap;
};
