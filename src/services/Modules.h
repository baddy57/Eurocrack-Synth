#pragma once

#include <vector>

class Module;

class Modules {
public:
	static std::vector<Module*> active;

	static void updateAll();
};
