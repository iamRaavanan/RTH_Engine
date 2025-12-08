#include "Rthpch.h"
#include "ModuleStack.h"

namespace RTH
{
	ModuleStack::ModuleStack()
	{

	}

	ModuleStack::~ModuleStack()
	{
		for (Module* Module : mModules)
			delete Module;
	}

	void ModuleStack::PushModule(Module* Module)
	{
		mModules.emplace(mModules.begin() + mModuleInsertIndex, Module);
		mModuleInsertIndex++;
	}

	void ModuleStack::PushOverlay(Module* overlay)
	{
		mModules.emplace_back(overlay);
	}

	void ModuleStack::PopModule(Module* Module)
	{
		auto it = std::find(mModules.begin(), mModules.end(), Module);
		if (it != mModules.end())
		{
			mModules.erase(it);
			mModuleInsertIndex--;
		}
	}

	void ModuleStack::PopOverlay(Module* overlay)
	{
		auto it = std::find(mModules.begin(), mModules.end(), overlay);
		if (it != mModules.end())
		{
			mModules.erase(it);
		}
	}

}