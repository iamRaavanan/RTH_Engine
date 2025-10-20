#pragma once
#include "RTH/Layer.h"

namespace RTH
{
	class RTH_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		void OnAttach();
		void OnDetach();
		void OnUpdate();
		void OnEvent(Event& e);
	private:
		float mTime = 0.0f;
	};
}