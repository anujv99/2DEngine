#include "pch.h"
#include "timer.h"

#include "src/common/log.h"

#if defined(ENGINE_DEBUG) && !defined(LOG_DISABLED)
	#define LOG_TIMER(x) x
#else
	#define LOG_TIMER(x)
#endif

namespace prev {

	std::chrono::duration<float> Timer::s_DeltaTime;
	pv_time_point Timer::m_Time = std::chrono::high_resolution_clock::now();
	pv_time_point Timer::s_StartTime = std::chrono::high_resolution_clock::now();

	unsigned int Timer::s_FPS						= 0;
	unsigned long long int Timer::s_LastTimeSec		= 0;
	bool Timer::s_ShouldShowFPS						= false;

	void Timer::Update() {
		auto currentTime = std::chrono::high_resolution_clock::now();
		s_DeltaTime = currentTime - m_Time;
		m_Time = currentTime;
		s_FPS++;
		if ((unsigned long long int)GetTime() > s_LastTimeSec) {
			s_LastTimeSec++;
			if (s_ShouldShowFPS) {
				LOG_TIMER(
					Logger::Log(User::ENGINE, LOG_LEVEL_INFO, "[FPS = {0}]", s_FPS);
				);
			}
			s_FPS = 0;
		}
	}

	float Timer::GetTime() {
		auto currentTime = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::microseconds>(currentTime - s_StartTime);
		return (float)duration.count() / 1000000;
	}

	float Timer::GetDeltaTime() {
		return s_DeltaTime.count();
	}

	void Timer::FPSCounter(bool isVisible) {
		s_ShouldShowFPS = isVisible;
	}

	TimeThis::TimeThis(const std::string & fileName, const std::string & functionName, const unsigned int & lineNumber, bool timeInMs) : 
		m_FileName(fileName), m_FunctionName(functionName), m_LineNumber(lineNumber), isMS(timeInMs) {
		m_Start = std::chrono::high_resolution_clock::now();

		m_FileName = m_FileName.substr(m_FileName.find_last_of("\\") + 1);
	}

	TimeThis::~TimeThis() {
		std::chrono::duration<float> deltaTime = std::chrono::high_resolution_clock::now() - m_Start;

		LOG_TIMER(
			Logger::Log(User::ENGINE, LOG_LEVEL_INFO, "-----------------------------------");
			Logger::Log(User::ENGINE, LOG_LEVEL_INFO, "FILE NAME : {0}", m_FileName);
			Logger::Log(User::ENGINE, LOG_LEVEL_INFO, "FUNC NAME : {0}", m_FunctionName);
			Logger::Log(User::ENGINE, LOG_LEVEL_INFO, "LINE NUM  : {0}", m_LineNumber);
		);

		if (isMS) {
			LOG_TIMER(
				Logger::Log(User::ENGINE, LOG_LEVEL_INFO, "THIS SCOPE TOOK : {0}ms", deltaTime.count() * 1000);
			);
		} else {
			LOG_TIMER(
				Logger::Log(User::ENGINE, LOG_LEVEL_INFO, "THIS SCOPE TOOK : {0}s", deltaTime.count());
			);
		}

		LOG_TIMER(
			Logger::Log(User::ENGINE, LOG_LEVEL_INFO, "-----------------------------------");
		);
	}

}
