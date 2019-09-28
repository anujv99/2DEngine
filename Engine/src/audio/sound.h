#pragma once

namespace prev {

	enum LoopCount {
		LOOP_FOREVER	= -1,
		LOOP_NEVER		= 0,
		LOOP_ONCE		= 1,
	};

	class Sound : public HandledObject<Sound> {
	protected:
		Sound() {}
	public:
		virtual ~Sound() {}
	public:
		virtual void Play() = 0;
		virtual void Pause() = 0;
		virtual void Stop() = 0;
		virtual void SetLoopCount(LoopCount loop) = 0;
	};

}
