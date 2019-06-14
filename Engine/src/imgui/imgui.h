#pragma once

#include "math/vec2.h"
#include "math/vec2i.h"
#include "math/vec3.h"
#include "math/vec4.h"
#include "math/vecconversion.h"
#include "graphics/texture2d.h"
#include "graphics/linegraph.h"

namespace prev {

	struct ImGui {

		static const int AUTOPOS	= -1;
		static const int AUTOSIZE	= -1;

		static void Begin(const std::string & name, Vec2i pos = Vec2i(ImGui::AUTOPOS), Vec2i dimension = Vec2i(ImGui::AUTOSIZE));
		static void End();

		static void Header(const std::string & name);
		static void SameLine();
		static void Separator();
		static void Tab();
		static void UnTab();
		static void Minimize();
		static void IsMinimized();

		static bool Button(const std::string & name);
		static bool ButtonDown(const std::string & name);
		static bool ButtonHover(const std::string & name);
		static bool ButtonNoPadding(const std::string & name);
		static bool ButtonDownNoPadding(const std::string & name);
		static bool ButtonHoverNoPadding(const std::string & name);

		static bool CheckBox(const std::string & name, bool & val);
		static int Select(const std::string names[], int & val, int numChoices);
		static int SelectCustom(const std::string names[], int values[], int & val, int numChoices);
		static int DropDown(const std::string & name, bool & val);

		static float SliderFloat(const std::string & name, float & val, float min, float max);
		static int SliderInt(const std::string & name, int & val, int min, int max);

		static void Print(const std::string & text);
		static void PrintParagraph(const std::string & text);

		static void LineGraph(prev::LineGraph & lineGraph);
		static void BarGraph(float * data, int num_bins, float minVal = 0.0f, float maxVal = 1.0f, int hightlightBinIndex = -1);

		static void MoveDrawPosBy(Vec2i dimen);
		static void MoveDrawPosNextLine(Vec2i dimen);

		static int FONT_WIDTH;
		static int FONT_HEIGHT;
		static int TITLE_BAR_HEIGHT;
	};

}
