#include "pch.h"
#include "error.h"

#include "log.h"

#define CONVERT_TO_STRING(x) #x

namespace prev {

	std::vector<Error::ErrorDispatcher::ErrorFunc> Error::ErrorDispatcher::m_DispatchFunctions;

	Error::Error(const ErrorType & errorType, const std::string & message) : 
		m_ErrorLevel(errorType), m_Message(message) {
		ErrorDispatcher::Dispatch(this);
	}

	Error::Error(const ErrorType & errorType, const std::string & message, const std::string & file, const std::string & line) : 
		m_ErrorLevel(errorType), m_Message(message), m_File(file.substr(file.find_last_of("\\") + 1)), m_Line(line) {
		ErrorDispatcher::Dispatch(this);
	}

	Error::~Error() {
	}

	void Error::AddErrorFunction(std::function<void(const Error &)> func) {
		if (func) {
			ErrorDispatcher::m_DispatchFunctions.push_back(func);
		}
	}

	void Error::LogError() {
		Logger::Ref().Log(User::ENGINE, LOG_LEVEL_ERROR, "------------------------------------------");
		if (m_File.length() != 0) {
			Logger::Ref().Log(User::ENGINE, LOG_LEVEL_ERROR, ("FILE NAME  : " + m_File).c_str());
		}
		if (m_Line.length() != 0) { 
			Logger::Ref().Log(User::ENGINE, LOG_LEVEL_ERROR, ("LINE NUM   : " + m_Line).c_str());
		}
		Logger::Ref().Log(User::ENGINE, LOG_LEVEL_ERROR, ("ERROR CODE : " + m_ErrorCode).c_str());
		Logger::Ref().Log(User::ENGINE, LOG_LEVEL_ERROR, ("ERROR INFO : " + m_ErrorType).c_str());
		Logger::Ref().Log(User::ENGINE, LOG_LEVEL_ERROR, ("ERROR MSG  : " + m_Message).c_str());
		Logger::Ref().Log(User::ENGINE, LOG_LEVEL_ERROR, "------------------------------------------");
	}

	void Error::ErrorDispatcher::Dispatch(Error * error) {
		switch (error->m_ErrorLevel) {
		case ERR_FILE_NOT_FOUND:
			error->m_ErrorType = "File not found";
			error->m_ErrorCode = CONVERT_TO_STRING(ERR_FILE_NOT_FOUND);
			break;
		case ERR_SHADER_ALREADY_PRESENT:
			error->m_ErrorType = "Shader already present in the map";
			error->m_ErrorCode = CONVERT_TO_STRING(ERR_SHADER_ALREADY_PRESENT);
			break;
		case ERR_SHADER_NOT_FOUND:
			error->m_ErrorType = "Shader not found";
			error->m_ErrorCode = CONVERT_TO_STRING(ERR_SHADER_NOT_FOUND);
			break;
		case ERR_D3D11_INTERNAL_ERROR:
			error->m_ErrorType = "An internal d3d11 error has occurred";
			error->m_ErrorCode = CONVERT_TO_STRING(ERR_D3D11_INTERNAL_ERROR);
			break;
		case ERR_GRAPHICS_CONTEXT_CREATION_FAILED:
			error->m_ErrorType = "Unable to create graphics context";
			error->m_ErrorCode = CONVERT_TO_STRING(ERR_GRAPHICS_CONTEXT_CREATION_FAILED);
			break;
		case ERR_WINDOW_INTERNAL_ERROR:
			error->m_ErrorType = "An internal window error has occurred";
			error->m_ErrorCode = CONVERT_TO_STRING(ERR_WINDOW_INTERNAL_ERROR);
			break;
		case ERR_TYPE_INVALID_FUNCTION:
			error->m_ErrorType = "Invalid function pointer passed";
			error->m_ErrorCode = CONVERT_TO_STRING(ERR_TYPE_INVALID_FUNCTION);
			break;
		case ERR_WINDOW_CREATION_FAILED:
			error->m_ErrorType = "Unable to create window";
			error->m_ErrorCode = CONVERT_TO_STRING(ERR_WINDOW_CREATION_FAILED);
			break;	
		case ERR_ASSERTION_FAILED:
			error->m_ErrorType = "Assertion failed";
			error->m_ErrorCode = CONVERT_TO_STRING(ERR_ASSERTION_FAILED);
			break;
		case ERR_TYPE_UNKNOWN:
			error->m_ErrorType = "An unknown error has occurred";
			error->m_ErrorCode = CONVERT_TO_STRING(ERR_TYPE_UNKNOWN);
			break;
		default:
			break;
		}

		error->LogError();
	}

}

//////////////////////////////////////TEMPORARY//////////////////////////////////////



//////////////////////////////////////TEMPORARY//////////////////////////////////////