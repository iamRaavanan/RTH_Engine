#pragma once
#include "RTH/Core/Module.h"
#include "RTH/Event/ApplicationEvent.h"
#include "RTH/Event/KeyEvent.h"
#include "RTH/Event/MouseEvent.h"
namespace RTH
{
	class RTH_API ImGuiModule : public Module
	{
	public:
		ImGuiModule();
		~ImGuiModule();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnImGuiRender() override;

		void Begin();
		void End();
	private:
		float mTime = 0.0f;
	};
}