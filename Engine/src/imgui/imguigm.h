#pragma once

class gmMachine;
class gmUserArray;
class gmTableObject;

namespace prev {

	void gmBindImGuiLib(gmMachine * a_machine);
	void ImGuiOutputTable(gmTableObject * table, gmUserArray * selectArray, bool showFunctions);

}
