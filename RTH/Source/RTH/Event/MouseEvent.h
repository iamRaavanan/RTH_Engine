#pragma once
#include "Event.h"
#include<sstream>

namespace RTH {
	class RTH_API MouseMovedEvent : public Event
	{
	public:
		MouseMovedEvent(float x, float y) : mMouseX(x), mMouseY(y) {}

		inline float GetX() const { return mMouseX; }
		inline float GetY() const { return mMouseY; }

		std::string ToString() cosnt override
		{
			std::stringstream ss;
			ss << "MouseMovedEvent: " << mMouseX << ", " << mMouseY;
			return ss.str();
		}
		EVENT_CLASS_TYPE(MouseMoved)
		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
	private:
		float mMouseX, mMouseY;
	};

	class RTH_API MouseButtonEvent : public Event
	{
	public:
		inline int GetMouseButton() const { return mButton; }
		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)

	protected:
		MouseButtonEvent(int button) : mButton(button) {}
		int mButton;
	};

	class RTH_API MouseButtonPressedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonPressedEvent(int button) : MouseButtonEvent(button) {}
		
		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseButtonPressedEvent: " << mButton;
			return ss.str();
		}
		EVENT_CLASS_TYPE(MouseButtonPressed)
	};

	class RTH_API MouseButtonReleasedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonReleasedEvent(int button) : MouseButtonEvent(button) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseButtonReleasedEvent: " << mButton;
			return ss.str();
		}
		EVENT_CLASS_TYPE(MouseButtonReleased)
	};
}