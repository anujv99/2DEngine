#include "gmUtilEx.h"

#include <string>

#include "gmMachine.h"
#include "gmTableObject.h"
#include "gmStreamBuffer.h"

#include "utils/datafile.h"
#include "utils/filehandler.h"

#include "common/log.h"

using namespace prev;

bool gmFileExists(const char * file) {
	return FileExists(file);
}

int gmCompileStr(gmMachine * a_machine, const char * file) {

	while (true) {
		int threadID;
		DataFile dataFile;
		dataFile.Load(file);

		const std::string & code = dataFile.GetDataStr();

		if (code.size() == 0) return 0;

		int err = a_machine->CheckSyntax(code.c_str());

		if (!err) {
			LOG_TRACE("Compiled '{}'", file);
			a_machine->ExecuteString(code.c_str(), &threadID);

			return threadID;
		}

		gmLog & compileLog = a_machine->GetLog();
		bool first = true;
		const char * msg = compileLog.GetEntry(first);

		while (msg) {
			LOG_TRACE("GM Compile error '{}': {}", file, msg);
			msg = compileLog.GetEntry(first);
		}

		break;
	}

}
