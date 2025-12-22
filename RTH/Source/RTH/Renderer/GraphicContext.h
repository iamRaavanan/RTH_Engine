#pragma once

namespace RTH
{
	class GraphicContex
	{
	public:
		virtual ~GraphicContex() = default;
		void virtual Init() = 0;
		void virtual SwapBuffers() = 0;
	};
}