#include "ArgMapper.h"

ArgMapper::ArgMapper(){

}

void ArgMapper::setSimpleArg(char argName, bool value){
    simpleArgsMap[argName] = value;
}

void ArgMapper::setCompoundArg(char argName, std::string value){
    compoundArgsMap[argName] = value;
}

bool ArgMapper::getSimpleArg(char argName){
    return simpleArgsMap[argName];
}

std::string ArgMapper::getCompoundArg(char argName){
    return compoundArgsMap[argName];
}
