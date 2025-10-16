#pragma once

#include "RTH/Core.h"
#include "RTH/Event/Event.h"

namespace RTH
{
	class RTH_API Layer
	{
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer();

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate() {}
		virtual void OnEvent(Event& e) {}

		inline const std::string& GetName() const { return mName; }

	private:
		std::string mName;
	};
}