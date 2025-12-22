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
		virtual void OnEvent(Event& e) override;

		void Begin();
		void End();

		void AllowImGuiEvent(bool value) { mAllowImGuiEvents = value; }
	private:
		bool mAllowImGuiEvents = true;
		float mTime = 0.0f;
	};
}