#include "pch.h"
#include "log.h"

#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/base_sink.h"
#include "spdlog/fmt/ostr.h"
#include "spdlog/details/null_mutex.h"

namespace prev {

	std::vector<std::function<void(std::string, LogLevel)>> s_LogCallbacks;

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
			for (auto & func : s_LogCallbacks) {
				func(fmt::to_string(formatted), level);
			}
		}

		virtual void flush_() override { }
	};

	std::shared_ptr<spdlog::logger> Logger::m_CoreLogger;
	std::shared_ptr<spdlog::logger> Logger::m_ClinetLogger;

	std::shared_ptr<CustomSink<spdlog::details::null_mutex>> s_CustomSink;

	void Logger::Initialize() {
		s_CustomSink = std::make_shared<CustomSink<spdlog::details::null_mutex>>();

		spdlog::set_pattern("%^[%T] %n: %v%$");

		m_CoreLogger = spdlog::stderr_color_st("ENGINE");
		m_CoreLogger->set_level(spdlog::level::trace);
		m_CoreLogger->sinks().push_back(s_CustomSink);

		m_ClinetLogger = spdlog::stderr_color_st("APP");
		m_ClinetLogger->set_level(spdlog::level::trace);
		m_ClinetLogger->sinks().push_back(s_CustomSink);

		LOG_INFO("Logger Initialized Successfully");
	}
	
	void Logger::AddLogCallback(LogCallbackFunc callbackFunction) {
		s_LogCallbacks.push_back(callbackFunction);
	}

}