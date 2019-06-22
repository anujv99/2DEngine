#pragma once

#include "math/vec2.h"
#include "math/vec2i.h"
#include "math/vec3.h"
#include "math/vec4.h"
#include "math/cubicspline2d.h"
#include "math/vecconversion.h"

#include "graphics/texture2d.h"
#include "graphics/linegraph.h"
#include "graphics/bargraph.h"

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
		static bool IsMinimized();

		static bool Button(const std::string & name);
		static bool ButtonDown(const std::string & name);
		static bool ButtonHover(const std::string & name);
		static bool ButtonNoPadding(const std::string & name);
		static bool ButtonDownNoPadding(const std::string & name);
		static bool ButtonHoverNoPadding(const std::string & name);

		static bool CheckBox(const std::string & name, bool & val);
		static int Select(const std::string names[], int & val, unsigned int numChoices);
		static int SelectCustom(const std::string names[], int values[], int & val, int numChoices);
		static int DropDown(const std::string & name, bool & val);

		static float SliderFloat(const std::string & name, float & val, float min, float max);
		static int SliderInt(const std::string & name, int & val, int min, int max);
		static Vec2 SliderVec2(const std::string & name, Vec2 & val, Vec2 min, Vec2 max);
		static Vec2i SliderVec2i(const std::string & name, Vec2i & val, Vec2i min, Vec2i max);
		static Vec3 SliderVec3(const std::string & name, Vec3 & val, Vec3 min, Vec3 max);
		static Vec3 SliderRGB(const std::string & name, Vec3 & val);
		static Vec4 SliderRGBA(const std::string & name, Vec4 & val);

		static void FillBarFloat(const std::string & name, float val, float min, float max);
		static void FillBarInt(const std::string & name, int val, int min, int max);
		static void FillBarVec2(const std::string & name, Vec2 val, Vec2 min, Vec2 max);
		static void FillBarVec2i(const std::string & name, Vec2i val, Vec2i min, Vec2i max);
		static void FillBarVec3(const std::string & name, Vec3 val, Vec3 min, Vec3 max);
		static void FillBarVec4(const std::string & name, Vec4 val, Vec4 min, Vec4 max);

		static void ColorBlockRGB(const std::string & name, Vec3 val, Vec2i dimen = Vec2i(64));
		static void ColorBlockRGBA(const std::string & name, Vec4 val, Vec2i dimen = Vec2i(64));

		static int TextInput(const std::string & name, std::string & buffer, int width);
		static void Print(const std::string & text);
		static void PrintParagraph(const std::string & text);

		static void LineGraph(prev::LineGraph & lineGraph);
		static void BarGraph(prev::BarGraph & barGraph);
		static void CubicSpline2D(StrongHandle<prev::CubicSpline2D> spline, Vec2 minPos, Vec2 maxPos, Vec2i dimen = Vec2i(300));

		static void MoveDrawPosBy(Vec2i dimen);
		static void MoveDrawPosNextLine(Vec2i dimen);
		static void SetActiveWidgetId(int Id); 
		static void SetScrollRatioY(float ry);

		static int GetPrevWidgetId();
		static bool IsWorkingWindowNew();
		static bool IsWindowActive();

		static int FONT_WIDTH;
		static int FONT_HEIGHT;
		static int TITLE_BAR_HEIGHT;
	};

}
