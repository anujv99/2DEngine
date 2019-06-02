#pragma once

#include <string>
#include <sstream>
#include <vector>
#include <functional>

#define ERROR_TRACE(e, m) { prev::Error automaticError(prev::e, m, __FILE__, std::to_string(__LINE__)); }

namespace prev {

	enum ErrorType {
		ERR_WINDOW_INTERNAL_ERROR,
		ERR_WINDOW_CREATION_FAILED,
		ERR_ASSERTION_FAILED,
		ERR_TYPE_INVALID_FUNCTION,
		ERR_TYPE_UNKNOWN
	};

	struct Error {
		friend class ErrorDispatcher;
	public:
		Error(const ErrorType & errorType, const std::string & message);
		Error(const ErrorType & errorType, const std::string & message, const std::string & file, const std::string & line);
		~Error();

		static void AddErrorFunction(std::function<void(const Error &)> func);
	private:
		void LogError();
	private:
		ErrorType m_ErrorLevel;
		std::string m_Message;
		std::string m_File;
		std::string m_Line;

		std::string m_ErrorType;
		std::string m_ErrorCode;
	private:
		class ErrorDispatcher {
			friend struct Error;
			using ErrorFunc = std::function<void(const Error &)>;
		public:
			static void Dispatch(Error * error);
		private:
			static std::vector<ErrorFunc> m_DispatchFunctions;
		};
	};

}