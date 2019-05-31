#pragma once

#define TIME_THIS_SCOPE		prev::TimeThis timer(__FILE__, __FUNCTION__, __LINE__, false);
#define TIME_THIS_SCOPE_MS	prev::TimeThis timer(__FILE__, __FUNCTION__, __LINE__, true);

#if defined _MSC_VER
typedef std::chrono::time_point<std::chrono::steady_clock> pv_time_point;
#elif defined PV_PLATFORM_LINUX
typedef std::chrono::time_point<std::chrono::system_clock> pv_time_point;
#endif

namespace prev {

	class Timer {
	public:
		static void Update();
		static float GetTime();
		static float GetDeltaTime();
		static void FPSCounter(bool isVisible);
		inline static bool IsLoggingFPSCounter() {return s_ShouldShowFPS; }
	private:
		static std::chrono::duration<float> s_DeltaTime;
		static pv_time_point m_Time, s_StartTime;
		static unsigned int s_FPS;
		static unsigned long long int s_LastTimeSec;
		static bool s_ShouldShowFPS;
	};

	//Time any scope
	class TimeThis {
	public:
		TimeThis(const std::string & fileName, const std::string & functionName, const unsigned int & lineNumber, bool timeInMs = false);
		~TimeThis();
	private:
		std::string m_FunctionName, m_FileName;
		unsigned int m_LineNumber;
		pv_time_point m_Start;
		bool isMS;
	};

}
