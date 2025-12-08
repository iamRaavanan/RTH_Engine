#pragma once

#include "RTH/Core/Core.h"
#include "RTH/Core/Timestep.h"
#include "RTH/Event/Event.h"
namespace RTH
{
	class RTH_API Module
	{
	public:
		Module(const std::string& name = "Module");
		virtual ~Module();

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate(Timestep deltaTime) {}
		virtual void OnImGuiRender() {}
		virtual void OnEvent(Event& e) {}
		inline const std::string& GetName() const { return mName; }

	private:
		std::string mName;
	};
}