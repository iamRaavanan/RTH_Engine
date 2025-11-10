#pragma once

namespace RTH
{
	class Timestep
	{
	public:
		Timestep(float time = 0.0f)
			:mTime(time)
		{
		}

		operator float() const { return mTime; }
		float GetSeconds() const { return mTime; }
		float GetMilliSeconds() const { return mTime * 1000; }
	private:
		float mTime;
	};
}
