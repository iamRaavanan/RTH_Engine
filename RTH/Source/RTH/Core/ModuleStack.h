#pragma once

#include "RTH/Core/Core.h"
#include "Module.h"
#include<vector>

namespace RTH
{
	class RTH_API ModuleStack
	{
	public:
		ModuleStack();
		~ModuleStack();

		void PushModule(Module* Module);
		void PushOverlay(Module* overlay);
		void PopModule(Module* Module);
		void PopOverlay(Module* overlay);

		std::vector<Module*>::iterator begin() { return mModules.begin(); };
		std::vector<Module*>::iterator end() { return mModules.end(); };

	private:
		std::vector<Module*> mModules;
		unsigned int mModuleInsertIndex = 0;
	};
}