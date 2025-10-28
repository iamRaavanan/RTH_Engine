#pragma once

namespace RTH
{
	class GraphicContex
	{
	public:
		void virtual Init() = 0;
		void virtual SwapBuffers() = 0;
	};
}