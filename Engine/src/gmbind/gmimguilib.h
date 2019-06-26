#pragma once

class gmMachine;
class gmTableObject;
class gmUserArray;

namespace prev {

	void gmBindImGuiLib(gmMachine * a_machine);

	void ImGuiOutputTable(gmTableObject * table, gmUserArray * selectArray, bool showFunctions);
	void ImGuiSafeEnd();

}
