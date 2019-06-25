#include "pch.h"
#include "log.h"

#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/base_sink.h"
#include "spdlog/fmt/ostr.h"
#include "spdlog/details/null_mutex.h"

namespace prev {

	static std::vector<std::function<void(std::string, LogLevel)>> g_LogCallbacks;

	template<typename Mutex>
	class CustomSink : public spdlog::sinks::base_sink<Mutex> {
	protected:
		virtual void sink_it_(const spdlog::details::log_msg & msg) override {
			fmt::memory_buffer formatted;
			sink::formatter_->format(msg, formatted);
			LogLevel level = LOG_LEVEL_UNKNOWN;
			switch (msg.level) {
			case spdlog::level::critical:
				level = LOG_LEVEL_CRITICAL;
				break;
			case spdlog::level::err:
				level = LOG_LEVEL_ERROR;
				break;
			case spdlog::level::warn:
				level = LOG_LEVEL_WARN;
				break;
			case spdlog::level::info:
				level = LOG_LEVEL_INFO;
				break;
			case spdlog::level::trace:
				level = LOG_LEVEL_TRACE;
				break;
			default:
				LOG_WARN("Unknown Log Level");
				break;
			}
			for (auto & func : g_LogCallbacks) {
				func(fmt::to_string(formatted), level);
			}
		}

		virtual void flush_() override { }
	};

	CustomSink<spdlog::details::null_mutex> * s_CustomSink;

	void Logger::Initialize() {
		s_CustomSink = new CustomSink<spdlog::details::null_mutex>();
		std::shared_ptr<CustomSink<spdlog::details::null_mutex>> sinkPtr(s_CustomSink);

		spdlog::set_pattern("%^[%T] %n: %v%$");

		s_CoreLogger = spdlog::stderr_color_st("ENGINE");
		s_CoreLogger->set_level(spdlog::level::trace);
		s_CoreLogger->sinks().push_back(sinkPtr);

		s_ClinetLogger = spdlog::stderr_color_st("APP");
		s_ClinetLogger->set_level(spdlog::level::trace);
		s_ClinetLogger->sinks().push_back(sinkPtr);
	}
	
	void Logger::AddLogCallback(LogCallbackFunc callbackFunction) {
		g_LogCallbacks.push_back(callbackFunction);
	}

	Logger::Logger() {
		Initialize();
	}

	Logger::~Logger() {
	}

}