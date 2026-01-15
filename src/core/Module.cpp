#include "Module.h"
#include "HardwareCfg.h"

//definition of static members
std::vector<std::shared_ptr<InputSocket>> Module::inputSockets;
std::vector<std::shared_ptr<OutputSocket>> Module::outputSockets;

//ctor
Module :: Module(const Address& address) : moduleAddress(address), verbose(false){
}