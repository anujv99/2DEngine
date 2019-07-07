#include "gmUtilEx.h"

#include <string>
#include <fstream>

#include "gmMachine.h"
#include "gmTableObject.h"
#include "gmStreamBuffer.h"

#include "utils/datafile.h"
#include "utils/filehandler.h"

#include "common/log.h"

using namespace prev;

#define GM_RESOURCE_GLOBAL_KEY "g_FileData"

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

	return -1;

}

void OutputTableNode(std::ofstream & fh, gmTableObject * table, gmVariable & key, int level) {
	// check not infinite loop
	const int recursionThreshold = 10;
	if (level > recursionThreshold) {
		LOG_ERROR("Saving table node file exceeded {} levels!", recursionThreshold);
		ASSERT(false);
	}

	const char * TAB = "	";

	for (int i = 0; i < level; ++i) fh << TAB;

	gmVariable var = table->Get(key);
	gmType type = var.m_type;

	// output table key
	if (key.IsString()) fh << key.GetCStringSafe() << " = ";

	switch (type) {
	case GM_NULL: fh << "null"; break;
	case GM_INT: fh << var.GetInt(); break;
	case GM_FLOAT: fh << var.GetFloat(); break;
	case GM_VEC2:
	{
		Vec2 vec = var.GetVec2();
		fh << "Vec2(" << vec.x << ", " << vec.y << ")";
		break;
	}
	case GM_VEC2I:
	{
		Vec2i vec = var.GetVec2i();
		fh << "Vec2i(" << vec.x << ", " << vec.y << ")";
		break;
	}
	case GM_VEC3:
	{
		Vec3 vec = var.GetVec3();
		fh << "Vec3(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
		break;
	}
	case GM_VEC4:
	{
		Vec4 vec = var.GetVec4();
		fh << "Vec4(" << vec.x << ", " << vec.y << ", " << vec.z << vec.w << ")";
		break;
	}
	case GM_STRING: fh << '"' << var.GetCStringSafe() << '"'; break;

		// invalid ones
	case GM_FUNCTION: fh << "null, // function"; break;
	case GM_USER: fh << "null, // user-type"; break;

	case GM_TABLE:
	{
		if (key.IsString()) {
			fh << std::endl;
			for (int i = 0; i < level; ++i) fh << TAB;
		}
		fh << "{" << std::endl;

		gmTableObject * childTable = var.GetTableObjectSafe();
		std::vector<gmVariable *> tableKeys;

		gmSortTableKeys(childTable, tableKeys);
		for (size_t i = 0; i < tableKeys.size(); ++i) {
			OutputTableNode(fh, childTable, *tableKeys[i], level + 1);
		}

		break;
	}
	};

	if (type == GM_TABLE) {
		for (int i = 0; i < level; ++i) fh << TAB;
		fh << "}";
	}

	if (type != GM_FUNCTION && type != GM_USER) fh << ",";
	fh << std::endl;
}

int gmSaveTableToFile(gmTableObject * table, const char * file) {
	GM_ASSERT(table);

	std::ofstream outFile(file);
	outFile.precision(6);
	std::showpoint(outFile);

	if (!outFile.is_open()) {
		ERROR_TRACE(ERR_TYPE_UNKNOWN, "Unable to save table to file : " + std::string(file));
		return 0;
	}

	outFile << "//Auto Generated File" << std::endl;
	outFile << "global " GM_RESOURCE_GLOBAL_KEY " = {" << std::endl;
	std::vector<gmVariable *> tableKeys;
	gmSortTableKeys(table, tableKeys);
	for (size_t i = 0; i < tableKeys.size(); i++) {
		OutputTableNode(outFile, table, *tableKeys[i], 1);
	}

	outFile << "};";
	outFile.close();

	LOG_TRACE("Saved Table to file: {}", file);

	return 0;
}

gmTableObject * gmLoadTableFromFile(gmMachine * a_machine, const char * file) {
	ASSERT(a_machine != nullptr);

	int thread = gmCompileStr(a_machine, file);
	gmVariable result_table = a_machine->GetGlobals()->Get(a_machine, GM_RESOURCE_GLOBAL_KEY);

	if (result_table.IsNull()) {
		ERROR_TRACE(ERR_TYPE_UNKNOWN, GM_RESOURCE_GLOBAL_KEY " not found in file : " + std::string(file));
	}

	// null the global value
	gmVariable null_var;
	null_var.Nullify();
	a_machine->GetGlobals()->Set(a_machine, GM_RESOURCE_GLOBAL_KEY, null_var);

	return result_table.GetTableObjectSafe();
}
