#include "pch.h"
#include "imgui.h"

#include "imguimanager.h"
#include "utils/input.h"
#include "graphics/window.h"
#include "graphics/immediategfx.h"

#define CHECK_LOG_ASSERT(c, m, ...) { if (!(c)) { LOG_ERROR(m, __VA_ARGS__); ASSERT(false); } }

namespace prev {

	int ImGui::FONT_WIDTH								= 6;
	int ImGui::FONT_HEIGHT								= 12;

	const int BUTTON_INSIDE_PADDING						= 3;
	const int IMAGE_BUTTON_INSIDE_PADDING				= 6;
	const int TAB_WIDTH									= ImGui::FONT_WIDTH * 2;

	const int WIDGET_PADDING							= 4;
	const int WINDOW_INSIDE_PADDING						= 6;
	const int TITLE_BAR_PADDING							= 7;
	const int FONT_Y_OFFSET								= 8;
	int ImGui::TITLE_BAR_HEIGHT							= ImGui::FONT_HEIGHT + TITLE_BAR_PADDING * 2;

	const int SCROLL_BAR_SIZE							= 15;

	const int FILLBAR_WIDTH								= 150;
	const int FILLBAR_TEXT_BUFFER						= 2;
	const int FILLBAR_HEIGHT							= ImGui::FONT_HEIGHT + FILLBAR_TEXT_BUFFER * 2;
	const float ROUNDED_RADIUS							= 4.0f;

	const Vec3 COLOR_WINDOW_BG							= Vec3(0.15f);
	const Vec3 COLOR_SLIDER_BTN_BORDER					= Vec3(0.0f);
	const Vec3 COLOR_SLIDER_BG_BORDER					= Vec3(0.08f);
	const Vec3 COLOR_SLIDER								= Vec3(0.25f);
	const Vec3 COLOR_SLIDER_ACTIVE						= Vec3(0.55f);
	const Vec3 COLOR_BUTTON								= Vec3(0.35f);
	const Vec3 COLOR_BUTTON_HOVER						= Vec3(0.45f);
	const Vec3 COLOR_BUTTON_PRESS						= Vec3(0.25f);
	const Vec3 COLOR_BAR								= Vec3(0.15f);
	const Vec3 COLOR_BAR_HOVER							= Vec3(0.30f);
	const Vec3 COLOR_FILLBAR							= Vec3(0.6f, 0.6f, 0.0f);
	const Vec3 COLOR_SEPARATOR							= Vec3(0.35f);
	const Vec3 COLOR_WHITE								= Vec3(1.0f);
	const Vec3 COLOR_BLACK								= Vec3(0.0f);

	const int IMGUI_MOUSE_PRESS_BTN						= 0;

	const int MOUSEWHEEL_SCROLL_DELTA					= 100;

	enum ButtonState {
		BUTTON_NONE,
		BUTTON_HOVER,
		BUTTON_PRESS,
		BUTTON_DOWN
	};

	enum SpecialWidgetId {
		ID_SCROLL_X = (1 << 16) + 0,
		ID_SCROLL_Y = (1 << 16) + 1,
		ID_RESIZE_BUTTON = (1 << 16) + 2,
	};

	inline StrongHandle<ImGuiWindow> & ImGuiWorkingWindow() { return ImGuiManager::Get()->state.WorkingWindow; }
	inline ImGuiManager::State & ImGuiState() { return ImGuiManager::Get()->state; }
	inline int ImGuiTextWidth(const std::string & c) { return ImGui::FONT_WIDTH * (int)c.size(); }
	inline bool ImGuiIsMinimized() { ASSERT(ImGuiWorkingWindow()); return ImGuiWorkingWindow()->Minimized || ImGuiWorkingWindow()->Locked; }

	inline void ImmGfxBegin() {
		ImmediateGFX::Ref().BeginDefaultShaders();
		ImmediateGFX::Ref().BeginDraw();
	}

	inline int ImGuiGenWidgetId() {
		return ++ImGuiState().WidgetCount;
	}

	inline Vec2i ImGuiGetMousePos() {
		return Input::Ref().GetMousePosition();
	}

	inline void ImGuiColor(Vec3 color, float alpha = 1.0f) {
		ImmediateGFX::Ref().Color(color, alpha);
	}

	inline void ImGuiDrawRectRounded(Vec2i pos, Vec2i dimen, float radius) {
		ImmGfxBegin();
		ImmediateGFX::Ref().DrawRectRounded(ToVec2(pos - Vec2i(0, dimen.y)), ToVec2(dimen), radius);
	}

	inline void ImGuiDrawRect(Vec2i pos, Vec2i dimen) {
		ImmGfxBegin();
		ImmediateGFX::Ref().DrawRect(ToVec2(pos - Vec2i(0, dimen.y)), ToVec2(dimen));
	}

	inline void ImGuiDrawRectWire(Vec2i pos, Vec2i dimen) {
		ImmGfxBegin();
		ImmediateGFX::Ref().DrawRectWire(ToVec2(pos - Vec2i(0, dimen.y)), ToVec2(dimen));
	}

	inline void ImGuiDrawRectRoundedTop(Vec2i pos, Vec2i dimen, float radius) {
		ImmGfxBegin();
		ImmediateGFX::Ref().DrawRectRoundedTop(ToVec2(pos - Vec2i(0, dimen.y)), ToVec2(dimen), radius);
	}

	inline void ImGuiDrawRectRoundedBot(Vec2i pos, Vec2i dimen, float radius) {
		ImmGfxBegin();
		ImmediateGFX::Ref().DrawRectRoundedBottom(ToVec2(pos - Vec2i(0, dimen.y)), ToVec2(dimen), radius);
	}

	inline void ImGuiDrawRectRoundedWire(Vec2i pos, Vec2i dimen, float radius) {
		ImmGfxBegin();
		ImmediateGFX::Ref().DrawRectRoundedWire(ToVec2(pos - Vec2i(0, dimen.y)), ToVec2(dimen), radius);
	}

	inline void ImGuiDrawLine(Vec2i start, Vec2i end) {
		ImmGfxBegin();
		ImmediateGFX::Ref().DrawLine(ToVec2(start), ToVec2(end));
	}

	inline void ImGuiPrint(const std::string & text, Vec2i pos) {
		Font & font = ImGuiManager::Ref().GetFont();

		font.Begin(0);

		auto fontVShader = ImGuiManager::Get()->GetVertexShader();
		auto fontPShader = ImGuiManager::Get()->GetPixeShader();
		fontVShader->UpdateMVP();

		fontVShader->Bind();
		fontPShader->Bind();

		pos.y -= FONT_Y_OFFSET;

		font.Print(text, ToVec2(pos));

		font.End();
	}

	inline bool ImGuiMouseOver(Vec2i pos, Vec2i dimen) {
		if (ImGuiWorkingWindow()->Locked) return false;

		const Vec2i mousePos = ImGuiGetMousePos();
		int mX = (int)mousePos.x;
		int mY = (int)mousePos.y;

		// mouse regions
		Vec2i regionPos = ImGuiState().MouseRegionStart + ImGuiState().ScrollOffset;
		Vec2i regionDimen = ImGuiState().MouseRegionDimensions;
		
		return (mX >= pos.x) && (mX <= pos.x + dimen.x) && (mY <= pos.y) && (mY >= pos.y - dimen.y)
			&& (mX >= regionPos.x) && (mX <= regionPos.x + regionDimen.x) && (mY <= regionPos.y) && (mY >= regionPos.y - regionDimen.y);
	}

	inline bool ImGuiIsWindowActive() {
		return ImGuiWorkingWindow() == ImGuiState().ActiveWindow;
	}

	inline void ImGuiSetActiveWidgetId(int id) {
		ImGuiManager::Get()->SetActiveWidgetId(id);
	}

	inline bool ImGuiDidMouseJustGoDown() {
		return Input::Ref().IsMouseButtonPressed(IMGUI_MOUSE_PRESS_BTN);
	}

	inline bool ImGuiDidMouseJustGoUp() {
		return Input::Ref().IsMouseButtonReleased(IMGUI_MOUSE_PRESS_BTN);
	}

	inline bool ImGuiIsWidgetActive(int id) {
		return ImGuiWorkingWindow() && id == ImGuiState().ActiveWidgetID;
	}

	inline Vec2i ImGuiGetScrollBarDimenOffset(StrongHandle<ImGuiWindow> & window) {
		int dimen_x = (window->Dimension.y < window->DimensionAutoSize.y && !window->AutoSize) ? SCROLL_BAR_SIZE : 0;
		int dimen_y = (window->Dimension.x < window->DimensionAutoSize.x && !window->AutoSize) ? SCROLL_BAR_SIZE + 1 : 0;

		return Vec2i(dimen_x, dimen_y);
	}

	inline int ImGuiGetActiveWidgetId() {
		return ImGuiManager::Get()->state.ActiveWidgetID;
	}

	void ImGuiWindowBG(StrongHandle<ImGuiWindow> & window) {
		static const float ALPHA_INACTIVE = 0.6f;
		static const float ALPHA_HOVER = 0.8f;
		static const float ALPHA_ACTIVE = 1.0f;

		float bgAlpha = ALPHA_INACTIVE;

		Vec2i dimen = window->Dimension;
		if (window->Minimized) dimen.y = ImGui::TITLE_BAR_HEIGHT;

		if (ImGuiMouseOver(window->Position, dimen)) {
			bgAlpha = ALPHA_HOVER;

			StrongHandle<ImGuiWindow> & prevActiveWindow = ImGuiState().ActiveWindow;

			if (!window->AutoSize) {
				const int scrollDelta = MOUSEWHEEL_SCROLL_DELTA;
				LOG_ERROR("WINDOW SCROLL NOT IMLEMENTED");
			}
		}

		if (ImGuiIsWindowActive()) {
			bgAlpha = ALPHA_ACTIVE;
		}

		window->BackgroundAlpha = Lerp(window->BackgroundAlpha, bgAlpha, ImGuiIsWindowActive() ? 1.0f : 0.17f);

		if (!ImGuiIsMinimized()) {
			ImGuiColor(COLOR_WINDOW_BG, window->BackgroundAlpha);
			ImGuiDrawRectRoundedBot(window->Position - Vec2i(0, ImGui::TITLE_BAR_HEIGHT), window->Dimension - Vec2i(0, ImGui::TITLE_BAR_HEIGHT), ROUNDED_RADIUS);
		}

	}

	ButtonState ImGuiButton(const std::string & name, int padding = BUTTON_INSIDE_PADDING) {
		if (ImGuiIsMinimized()) return BUTTON_NONE;

		const Vec3 COLOR_BORDER = Vec3(0.1f);
		const Vec3 COLOR_TEXT = COLOR_WHITE;

		ButtonState result = BUTTON_NONE;
		const int id = ImGuiGenWidgetId();
		const int buttonHeight = ImGui::FONT_HEIGHT + padding * 2;

		const int fontWidth = ImGuiTextWidth(name);
		const Vec2i pos = ImGuiState().DrawPos;
		const Vec2i dimen = Vec2i(fontWidth + padding * 2, buttonHeight);
		Vec3 buttonColor = COLOR_BUTTON;

		// mouse go down
		if (ImGuiMouseOver(pos, dimen)) {
			buttonColor = COLOR_BUTTON_HOVER;
			result = BUTTON_HOVER;

			// mouse goes down on button
			if (ImGuiDidMouseJustGoDown()) {
				if (ImGuiIsWindowActive()) {
					ImGuiSetActiveWidgetId(id);
				}
			}
			

			// fully clicked on button!
			else if (ImGuiDidMouseJustGoUp() && ImGuiIsWidgetActive(id)) {
				result = BUTTON_PRESS;
				ImGuiSetActiveWidgetId(ImGuiManager::State::WIDGET_NULL);
			}
		}

		// released mouse
		else if (ImGuiDidMouseJustGoUp()) {
			if (ImGuiIsWidgetActive(id)) {
				ImGuiSetActiveWidgetId(ImGuiManager::State::WIDGET_NULL);
			}
		}

		// held down
		if (ImGuiIsWidgetActive(id)) {
			result = BUTTON_DOWN;
			buttonColor = COLOR_BUTTON_PRESS;
		}

		// draw button
		ImGuiColor(buttonColor);
		ImGuiDrawRect(pos, dimen);
		ImGuiColor(COLOR_BORDER);
		ImGuiDrawRectWire(pos, dimen);
		ImGuiColor(COLOR_TEXT);
		ImGuiPrint(name, pos + Vec2i(padding, -padding));

		ImGui::MoveDrawPosNextLine(dimen);

		return result;
	}

	void ImGuiTitleBar(StrongHandle<ImGuiWindow> & window, const std::string & name, int id) {
		const int MINIZE_BUTTON_PADDING = WIDGET_PADDING;
		const Vec3 COLOR_WINDOW_TITLE = Vec3(0.1f, 0.7f, 0.4f);
		const int BUTTON_HEIGHT = ImGui::FONT_HEIGHT + BUTTON_INSIDE_PADDING * 2;

		bool isWindowActive = ImGuiIsWindowActive();
		const Vec2i mousePos = ImGuiGetMousePos();

		// draw title bar background
		Vec2i titleBarDimen = Vec2i(window->Dimension.x, ImGui::TITLE_BAR_HEIGHT);
		float titleBarAlpha = 1.0f;
		Vec3 titleBarColor = COLOR_WINDOW_TITLE * window->BackgroundAlpha;
		ImGuiColor(titleBarColor, titleBarAlpha);

		// draw the rect. full rounded if minimized
		if (window->Minimized) {
			ImGuiDrawRectRounded(window->Position, titleBarDimen, ROUNDED_RADIUS);
		} else {
			ImGuiDrawRectRoundedTop(window->Position, titleBarDimen, ROUNDED_RADIUS);

			// draw line under title bar
			ImGuiColor(COLOR_WINDOW_BG);
			ImGuiDrawLine(window->Position + Vec2i(0, -ImGui::TITLE_BAR_HEIGHT), window->Position + Vec2i(window->Dimension.x, -ImGui::TITLE_BAR_HEIGHT));
		}

		// draw title font
		int fontWidth = ImGuiTextWidth(name);
		float fontAlpha = isWindowActive ? 1.0f : 0.6f;
		ImGuiColor(COLOR_WHITE, fontAlpha);
		ImGuiPrint(name, window->Position + Vec2i(TITLE_BAR_PADDING, -TITLE_BAR_PADDING));

		// find position for button
		int buttonWidth = ImGui::FONT_WIDTH + BUTTON_INSIDE_PADDING * 2;
		ImGuiState().DrawPos = window->Position + Vec2i(window->Dimension.x - buttonWidth - WIDGET_PADDING / 2 - 2, -(ImGui::TITLE_BAR_HEIGHT - BUTTON_HEIGHT) / 2);

		// calculate how big min title bar
		window->TitleBarMinWidth = fontWidth + TITLE_BAR_PADDING + buttonWidth + MINIZE_BUTTON_PADDING;

		// handle minimized (need to unminimized to draw the button)
		bool minimized = window->Minimized;
		window->Minimized = false;
		ButtonState minBtn = ImGuiButton(minimized ? "+" : "-");
		minimized = minimized ^ (minBtn == BUTTON_PRESS);
		window->Minimized = minimized;

		// Handle title bar drag
		if (minBtn == BUTTON_NONE) {
			if (ImGuiMouseOver(window->Position, titleBarDimen)) {
				if (ImGuiDidMouseJustGoDown() && ImGuiIsWindowActive()) {
					// double click
					if (true) {
						LOG_ERROR("Mouse double click not implemented");
					}

					ImGuiSetActiveWidgetId(id);
					ImGuiState().Data.IVec[0] = mousePos.x - window->Position.x;
					ImGuiState().Data.IVec[1] = mousePos.y - window->Position.y;
				}
			}
		}
	}

	bool ImGuiHorizontalScrollBar(StrongHandle<ImGuiWindow> & window) {
		if (ImGuiIsMinimized()) return false;
		const int id = ID_SCROLL_X;

		// calculate how much bar is shown
		int totalWidthAllWidgets = window->DimensionAutoSize.x;
		if (totalWidthAllWidgets <= 0) totalWidthAllWidgets = 1;
		const int barWidth = window->Dimension.x - 1;
		const float percentShown = (float)barWidth / totalWidthAllWidgets;

		// don't show bar if not needed
		if (percentShown >= 1.0f) {
			window->ScrollPos.x = 0;
			return false;
		}

		// bar
		Vec3  barColor = COLOR_BAR;
		Vec2i barPosition = Vec2i(window->Position.x + 1, window->Position.y - window->Dimension.y + SCROLL_BAR_SIZE);
		Vec2i barDimen = Vec2i(barWidth - SCROLL_BAR_SIZE, SCROLL_BAR_SIZE);

		// slider
		Vec3 sliderColor = COLOR_SLIDER;
		Vec2i sliderDimen = Vec2i((int)(barDimen.x * percentShown), SCROLL_BAR_SIZE);
		int sliderPositionMinX = barPosition.x;
		int sliderPositionMaxX = barPosition.x + barDimen.x - sliderDimen.x;
		float scrollRatioX = Saturate((float)window->ScrollPos.x / 
			(std::max(window->DimensionAutoSize.x, window->DimensionAutoSizePrev.x) - window->Dimension.x));
		Vec2i sliderPosition = Vec2i((int)Lerp((float)sliderPositionMinX, (float)sliderPositionMaxX, scrollRatioX), barPosition.y);

		const int mouseX = ImGuiGetMousePos().x;

		// mouse over
		if (ImGuiMouseOver(barPosition, barDimen)) {
			barColor = COLOR_BAR_HOVER;

			if (ImGuiDidMouseJustGoDown()) {
				ImGuiSetActiveWidgetId(id);

				if (ImGuiMouseOver(sliderPosition, sliderDimen)) // over slider
				{
					ImGuiState().Data.I = mouseX - sliderPosition.x;
				} else // not over slider, chose half the Positionition
				{
					ImGuiState().Data.I = sliderDimen.x >> 1;
				}
			}
		}

		if (ImGuiIsWidgetActive(id)) {
			// release mouse
			if (ImGuiDidMouseJustGoUp()) {
				ImGuiSetActiveWidgetId(ImGuiManager::State::WIDGET_NULL);
			}

			// bar Position based on mouse Position
			else {
				int xMin = barPosition.x + ImGuiState().Data.I;
				int xMax = barPosition.x + barDimen.x - sliderDimen.x + ImGuiState().Data.I;

				scrollRatioX = Saturate(((float)mouseX - xMin) / (xMax - xMin));
				window->ScrollPos.x = (int)(scrollRatioX * (std::max(window->DimensionAutoSize.x, window->DimensionAutoSizePrev.x) - window->Dimension.x));
			}
		}

		if (ImGuiIsWidgetActive(id)) sliderColor = COLOR_SLIDER_ACTIVE;

		// draw background
		ImGuiColor(barColor);
		ImGuiDrawRect(barPosition, barDimen);
		ImGuiColor(COLOR_SLIDER_BG_BORDER);
		ImGuiDrawRectWire(barPosition, barDimen);

		// draw slider
		ImGuiColor(sliderColor);
		ImGuiDrawRectRounded(sliderPosition, sliderDimen, ROUNDED_RADIUS);
		ImGuiColor(COLOR_SLIDER_BTN_BORDER);
		ImGuiDrawRectRoundedWire(sliderPosition, sliderDimen, ROUNDED_RADIUS);

		return true;
	}

	bool ImGuiVerticalScrollBar(StrongHandle<ImGuiWindow> & window) {
		if (ImGuiIsMinimized()) return false;
		const int id = ID_SCROLL_Y;

		// calculate how much bar is shown
		int totalHeightAllWidgets = window->DimensionAutoSize.y - ImGui::TITLE_BAR_HEIGHT;
		if (totalHeightAllWidgets <= 0) totalHeightAllWidgets = 1;
		const int barHeight = window->Dimension.y - ImGui::TITLE_BAR_HEIGHT;
		const float percentShown = (float)barHeight / totalHeightAllWidgets;

		// don't show bar if not needed
		if (percentShown >= 1.0f) {
			window->ScrollPos.y = 0;
			return false;
		}

		// bar
		Vec3  barColor = COLOR_BAR;
		Vec2i barPos = Vec2i(window->Position.x + window->Dimension.x - SCROLL_BAR_SIZE, window->Position.y - ImGui::TITLE_BAR_HEIGHT);
		Vec2i barDimen = Vec2i(SCROLL_BAR_SIZE, barHeight - SCROLL_BAR_SIZE);

		// slider
		Vec3	sliderColor = COLOR_SLIDER;
		Vec2i sliderDimen = Vec2i(SCROLL_BAR_SIZE, (int)(barDimen.y * percentShown));
		int sliderPosMinY = barPos.y;
		int sliderPosMaxY = barPos.y - barDimen.y + sliderDimen.y;
		float scrollRatioY = Saturate((float)window->ScrollPos.y / (std::max(window->DimensionAutoSize.y, window->DimensionAutoSizePrev.y) - window->Dimension.y));
		Vec2i sliderPos = Vec2i(barPos.x, (int)Lerp((float)sliderPosMinY, (float)sliderPosMaxY, scrollRatioY));

		const int mouseY = ImGuiGetMousePos().y;

		// mouse over
		if (ImGuiMouseOver(barPos, barDimen)) {
			barColor = COLOR_BAR_HOVER;

			if (ImGuiDidMouseJustGoDown()) {
				ImGuiSetActiveWidgetId(id);

				if (ImGuiMouseOver(sliderPos, sliderDimen)) // over slider
				{
					ImGuiState().Data.I = sliderPos.y - mouseY;
				} else // not over slider, chose half the position
				{
					ImGuiState().Data.I = sliderDimen.y >> 1;
				}
			}
		}

		if (ImGuiIsWidgetActive(id)) {
			// release mouse
			if (ImGuiDidMouseJustGoUp()) {
				ImGuiSetActiveWidgetId(ImGuiManager::State::WIDGET_NULL);
			}

			// bar position based on mouse pos
			else {
				int yMin = barPos.y - ImGuiState().Data.I;
				int yMax = barPos.y - barDimen.y + sliderDimen.y - ImGuiState().Data.I;

				scrollRatioY = Saturate(((float)mouseY - yMin) / (yMax - yMin));
				window->ScrollPos.y = (int)(scrollRatioY * (std::max(window->DimensionAutoSize.y, window->DimensionAutoSizePrev.y) - window->Dimension.y));
			}
		}

		if (ImGuiIsWidgetActive(id)) sliderColor = COLOR_SLIDER_ACTIVE;

		// draw background
		ImGuiColor(barColor);
		ImGuiDrawRect(barPos, barDimen);
		ImGuiColor(COLOR_SLIDER_BG_BORDER);
		ImGuiDrawRectWire(barPos, barDimen);

		// draw slider
		ImGuiColor(sliderColor);
		ImGuiDrawRectRounded(sliderPos, sliderDimen, ROUNDED_RADIUS);
		ImGuiColor(COLOR_SLIDER_BTN_BORDER);
		ImGuiDrawRectRoundedWire(sliderPos, sliderDimen, ROUNDED_RADIUS);

		return true;
	}

	void ImGuiResizeButton(StrongHandle<ImGuiWindow> & window) {
		if (ImGuiIsMinimized()) return;
		const int id = ID_RESIZE_BUTTON;

		const Vec2i Position = window->Position + Vec2i(window->Dimension.x, -window->Dimension.y) + Vec2i(-SCROLL_BAR_SIZE, SCROLL_BAR_SIZE);
		const Vec2i Dimension = Vec2i(SCROLL_BAR_SIZE);
		const Vec2i mousePosition = ImGuiGetMousePos();

		Vec3 color = COLOR_SLIDER;

		// mouse over
		if (ImGuiMouseOver(Position, Dimension)) {
			color = COLOR_BUTTON_HOVER;

			// clicked on button
			if (ImGuiDidMouseJustGoDown()) {
				ImGuiSetActiveWidgetId(id);

				// cache mouse Position and offset
				ImGuiState().Data.IVec[0] = mousePosition.x;
				ImGuiState().Data.IVec[1] = mousePosition.y;
				ImGuiState().Data.IVec[2] = mousePosition.x - Position.x;
				ImGuiState().Data.IVec[3] = mousePosition.y - Position.y;
			}
		}

		if (ImGuiIsWidgetActive(id)) {
			color = COLOR_BUTTON_PRESS;

			// release
			if (ImGuiDidMouseJustGoUp()) {
				ImGuiSetActiveWidgetId(ImGuiManager::State::WIDGET_NULL);
			} else {
				// resize window
				window->Dimension.x = std::max(window->TitleBarMinWidth + SCROLL_BAR_SIZE, mousePosition.x - window->Position.x - ImGuiState().Data.IVec[2] + SCROLL_BAR_SIZE);
				window->Dimension.y = std::max(ImGui::TITLE_BAR_HEIGHT + SCROLL_BAR_SIZE, window->Position.y - mousePosition.y + ImGuiState().Data.IVec[3] + SCROLL_BAR_SIZE);
			}
		}

		// background
		ImGuiColor(color);
		ImGuiDrawRect(Position, Dimension);
		ImGuiColor(COLOR_BLACK);
		ImGuiDrawRectWire(Position, Dimension);

		// line
		const int LINE_PADDING = 2;
		Vec2i linePositionStart = Position + Vec2i(LINE_PADDING, -SCROLL_BAR_SIZE + LINE_PADDING);
		Vec2i linePositionEnd = Position + Vec2i(SCROLL_BAR_SIZE - LINE_PADDING, -LINE_PADDING);
		ImGuiColor(COLOR_WHITE, ImGuiIsWidgetActive(id) ? 0.85f : 0.5f);
		ImGuiDrawLine(linePositionStart, linePositionEnd);
	}

	float ImGuiSlider(int id, float percent) {
		const int SLIDER_WIDTH = 150;
		const int SLIDER_HEIGHT = ImGui::FONT_HEIGHT;
		const int BUTTON_WIDTH = 10;

		Vec2i pos = ImGuiState().DrawPos;
		Vec2i dimen = Vec2i(SLIDER_WIDTH, SLIDER_HEIGHT);

		// extend size if not autosized
		if (!ImGuiWorkingWindow()->AutoSize) {
			dimen.x = std::max(dimen.x, ImGuiWorkingWindow()->Dimension.x - WIDGET_PADDING * 2 - WINDOW_INSIDE_PADDING * 2 - ImGui::FONT_WIDTH * 15);
		}

		Vec3 colorBar = COLOR_BAR;
		Vec3 colorButton = COLOR_SLIDER;

		// button position
		int xMin = pos.x + BUTTON_WIDTH / 2;
		int xMax = pos.x + dimen.x - BUTTON_WIDTH / 2;

		if (ImGuiMouseOver(pos, dimen)) {
			colorBar = COLOR_BAR_HOVER;

			// clicked down
			if (ImGuiDidMouseJustGoDown() && ImGuiIsWindowActive()) {
				ImGuiSetActiveWidgetId(id);
				ImGuiState().Data.F = 0.0f;
			}
		}

		if (ImGuiIsWidgetActive(id)) {
			// released
			if (ImGuiDidMouseJustGoUp()) {
				ImGuiSetActiveWidgetId(ImGuiManager::State::WIDGET_NULL);
			}

			int mouseX = ImGuiGetMousePos().x;
			percent = Saturate(float(mouseX - xMin) / (xMax - xMin));

			colorButton = COLOR_SLIDER_ACTIVE;
		}

		// background
		ImGuiColor(colorBar);
		ImGuiDrawRect(pos, dimen);
		ImGuiColor(COLOR_SLIDER_BG_BORDER);
		ImGuiDrawRectWire(pos, dimen);

		// draw button
		int xPos = (int)Lerp((float)xMin, (float)xMax, Saturate(percent));
		Vec2i buttonDimen = Vec2i(BUTTON_WIDTH, SLIDER_HEIGHT - 1);
		Vec2i buttonPos = Vec2i(xPos - BUTTON_WIDTH / 2, pos.y);
		ImGuiColor(colorButton);
		ImGuiDrawRect(buttonPos, buttonDimen);
		ImGuiColor(COLOR_SLIDER_BTN_BORDER);
		ImGuiDrawRectWire(buttonPos, buttonDimen);

		// move draw cursor
		ImGui::MoveDrawPosNextLine(dimen);

		return percent;
	}

	void ImGuiRefreshScrollOffset(bool begin = false) {
		StrongHandle<ImGuiWindow> window = ImGuiWorkingWindow();

		// figure out translation offset
		int translateX = -window->ScrollPos.x;
		int translateY = window->ScrollPos.y;
		ImGuiState().ScrollOffset = -Vec2i(translateX, translateY);

		if (ImGuiState().ScrollOffset != Vec2i(0, 0))
			int a = 0;

		if (!begin)
			MVP::Ref().Model().Pop();
		MVP::Ref().Model().Push();
		MVP::Ref().Model().Translate(Vec2((float)translateX, (float)translateY));
	}

	void ImGui::Begin(const std::string & name, Vec2i pos /*= Vec2i(ImGui::AUTOPOS)*/, Vec2i dimension /*= Vec2i(ImGui::AUTOSIZE)*/) {
		CHECK_LOG_ASSERT(ImGuiWorkingWindow() == nullptr, "ImGui::Begin('{}') Error - Forgot to call ImGui::End() on window '{}'", name
		,ImGuiState().WorkingWindow->Name);

		const int id = ImGuiGenWidgetId();
		ASSERT(id == ImGuiManager::State::WIDGET_WINDOW);
		const Vec2i mousePos = ImGuiGetMousePos();
		const Vec2i windowSize = Window::Ref().GetDisplayMode().GetWindowSize();

		ImGuiManager::Get()->GetCamera().Begin();
		//Set Viewport
		//Set Blend Function
		
		//get window
		ImGuiManager * manager = ImGuiManager::Get();
		StrongHandle<ImGuiWindow> window = manager->GetWindow(name);
		window->FramesSinceUpdate = 0;
		ImGuiState().WorkingWindow = window;

		// remove any tabbing
		ImGuiState().NumTabs = 0;

		//on first creation of window
		if (window->IsNewWindow) {
			window->AutoSize = (dimension == Vec2i(ImGui::AUTOSIZE));

			if (window->AutoSize) {
				window->Dimension = Vec2i(0);
			} else if (window->Dimension == Vec2i(ImGui::AUTOSIZE) || window->Dimension == Vec2i(0)) {
				window->Dimension = dimension;
			}

			if (window->Position == Vec2i(-1)) {
				window->Position = pos;
			}

			if (window->Position == Vec2i(ImGui::AUTOPOS)) {
				const int windowMargin = 150;
				static Vec2i autoPos = Vec2i(windowMargin, windowSize.y - 10);

				window->Position = autoPos;

				autoPos += Vec2i(windowMargin, 0);

				if (autoPos.x > windowSize.x - windowMargin * 2)
					autoPos = Vec2i(windowMargin, autoPos.y - windowMargin * 2);
			}
		}

		if (ImGuiIsWidgetActive(id)) {
			if (ImGuiDidMouseJustGoUp()) {
				ImGuiSetActiveWidgetId(ImGuiManager::State::WIDGET_NULL);
			} else {
				Vec2i delta = Vec2i(ImGuiState().Data.IVec[0], ImGuiState().Data.IVec[1]);
				window->Position = mousePos - delta;
			}
		}

		Vec2i borderDimen = window->Dimension;
		if (window->Minimized || window->Locked) borderDimen.y = ImGui::TITLE_BAR_HEIGHT;

		const int shadowOffset = 1;
		const int shadowSize = 4;
		ImGuiColor(COLOR_BLACK, 0.2f);
		ImGuiDrawRectRounded(window->Position + Vec2i(shadowOffset, -shadowOffset), borderDimen + Vec2i(shadowSize), ROUNDED_RADIUS);

		ImGuiWindowBG(window);
		ImGuiTitleBar(window, name, id);

		ImGuiColor(COLOR_WINDOW_BG * 0.75f);
		ImGuiDrawRectRoundedWire(window->Position, borderDimen, ROUNDED_RADIUS);

		if (!window->AutoSize) {
			bool showHorizScroll = ImGuiHorizontalScrollBar(window);
			bool showVertScroll = ImGuiVerticalScrollBar(window);

			// only show resize button if any scrollbars are visible
			if (showHorizScroll || showVertScroll) {
			}
			ImGuiResizeButton(window);
		}

		// scissor around box (not the title bar and scrollers)
		const Vec2i scroll_bar_dimen_offset = ImGuiGetScrollBarDimenOffset(ImGuiWorkingWindow());
		ScissorBox box;
		
		box.Left = window->Position.x;
		box.Right = box.Left + std::max(0, window->Dimension.x - 1 - scroll_bar_dimen_offset.x);
		box.Bottom = window->Position.y - window->Dimension.y + scroll_bar_dimen_offset.y;
		box.Top = box.Bottom + std::max(0, window->Dimension.y - ImGui::TITLE_BAR_HEIGHT - 1 - scroll_bar_dimen_offset.y);
		
		RenderState::Ref().SetScissorBox(box);

		ImGuiRefreshScrollOffset(true);

		// where the mouse can interact
		ImGuiState().MouseRegionStart = window->Position - Vec2i(0, ImGui::TITLE_BAR_HEIGHT);
		ImGuiState().MouseRegionDimensions = window->Dimension - Vec2i(0, ImGui::TITLE_BAR_HEIGHT) - scroll_bar_dimen_offset;

		// determine size through ImGui calls
		window->DimensionAutoSizePrev = window->DimensionAutoSize;
		window->DimensionAutoSize = Vec2i(0);
		MoveDrawPosNextLine(Vec2i(0, WINDOW_INSIDE_PADDING - WIDGET_PADDING));
	}

	void ImGui::End() {
		CHECK_LOG_ASSERT(ImGuiWorkingWindow() != NULL, "ImGui::End() called without existing working window");

		// editable region
		const Vec2i windowSize = Window::Get()->GetDisplayMode().GetWindowSize();
		ImGuiState().MouseRegionStart = Vec2i(0, windowSize.y);
		ImGuiState().MouseRegionDimensions = windowSize;
		ImGuiState().ScrollOffset = Vec2i(0);

		MVP::Ref().Model().Pop();

		// end render state
		RenderState::Ref().DisableScissors();
		ImGuiManager::Get()->GetCamera().End();

		// disable working window
		StrongHandle<ImGuiWindow> window = ImGuiWorkingWindow();
		window->IsNewWindow = false;

		// will use title bar width if that is bigger
		window->DimensionAutoSize.x = std::max(window->DimensionAutoSize.x, window->TitleBarMinWidth);

		// add window padding
		window->DimensionAutoSize += Vec2i(WINDOW_INSIDE_PADDING, WINDOW_INSIDE_PADDING - WIDGET_PADDING);

		// if auto sized
		if (window->AutoSize) window->Dimension = window->DimensionAutoSize;

		// did use click anywhere in box
		const bool is_mouse_in_window = ImGuiMouseOver(window->Position, window->Dimension);
		const bool mouse_clicked = ImGuiDidMouseJustGoDown();

		// if currently active window
		if (ImGuiIsWindowActive()) {
			// if no widget is selected and use clicked on the widget, set it to drag
			if (ImGuiGetActiveWidgetId() == ImGuiManager::State::WIDGET_NULL
				&& is_mouse_in_window && mouse_clicked && !window->Minimized
				) {
				Vec2i mousePosition = ImGuiGetMousePos();
				ImGuiSetActiveWidgetId(ImGuiManager::State::WIDGET_WINDOW);
				ImGuiState().Data.IVec[0] = mousePosition.x - window->Position.x;
				ImGuiState().Data.IVec[1] = mousePosition.y - window->Position.y;
			}
		}

		ImGuiState().WorkingWindow = nullptr;
		ImGuiState().WidgetCount = 0;
	}

	void ImGui::Header(const std::string & name) {
		if (ImGuiIsMinimized()) return;

		const int HEADER_INNER_PADDING = 2;
		const int HEADER_PADDING = 3;
		const int HEADER_HEIGHT = ImGui::FONT_HEIGHT + HEADER_PADDING * 3;
		//const int HEADER_WIDTH = FILLBAR_WIDTH;
		const int HEADER_TOP_PADDING = 7;

		ImGuiState().DrawPos.x = ImGuiWorkingWindow()->Position.x + HEADER_INNER_PADDING;
		ImGuiState().DrawPos.y -= HEADER_TOP_PADDING;

		Vec2i pos = ImGuiState().DrawPos;
		Vec2i dimen = Vec2i(ImGuiWorkingWindow()->Dimension.x, HEADER_HEIGHT);

		//const v3 HEADER_COLOR = v3(0.1f, 0.25f, 0.29f );
		ImGuiColor(COLOR_BAR_HOVER, ImGuiIsWindowActive() ? 1.0f : 0.7f);
		ImGuiDrawRect(pos, dimen);
		ImGuiColor(COLOR_WHITE);
		ImGuiPrint(name, pos + Vec2i(HEADER_PADDING * 2, -HEADER_PADDING));

		// border around
		ImGuiColor(COLOR_WHITE, 0.2f);
		ImGuiDrawRectWire(pos, dimen - Vec2i(HEADER_INNER_PADDING + 2, 1));

		MoveDrawPosNextLine(dimen + Vec2i(-WINDOW_INSIDE_PADDING - WIDGET_PADDING, HEADER_PADDING));
	}

	void ImGui::SameLine() {
		if (ImGuiIsMinimized()) return;

		ImGuiState().DrawPos = ImGuiState().DrawPosPrev;
	}

	void ImGui::Separator() {
		if (ImGuiIsMinimized()) return;

		StrongHandle<ImGuiWindow> window = ImGuiWorkingWindow();

		Vec2i pos = ImGuiState().DrawPos - Vec2i(0, WIDGET_PADDING);
		Vec2i dimen = Vec2i(window->Dimension.x, 0);

		ImGuiColor(COLOR_BLACK, 0.6f);
		ImGuiDrawLine(pos, pos + Vec2i(window->Dimension.x - WINDOW_INSIDE_PADDING * 2, 0));

		ImGuiColor(COLOR_SEPARATOR);
		ImGuiDrawLine(pos - Vec2i(0, 1), pos - Vec2i(0, 1) + Vec2i(window->Dimension.x - WINDOW_INSIDE_PADDING * 2, 0));

		MoveDrawPosNextLine(Vec2i(1, dimen.y + WIDGET_PADDING * 2 + 1));
	}

	void ImGui::Tab() {
		if (ImGuiIsMinimized()) return;

		ImGuiState().DrawPos.x += TAB_WIDTH;
		ImGuiState().NumTabs++;
	}

	void ImGui::UnTab() {
		if (ImGuiIsMinimized()) return;

		if (ImGuiState().NumTabs > 0) {
			ImGuiState().DrawPos.x -= TAB_WIDTH;
			ImGuiState().NumTabs--;
		}
	}

	void ImGui::Minimize() {

	}

	void ImGui::IsMinimized() {

	}

	bool ImGui::Button(const std::string & name) {
		return ImGuiButton(name) == BUTTON_PRESS;
	}

	bool ImGui::ButtonDown(const std::string & name) {
		return false;
	}

	bool ImGui::ButtonHover(const std::string & name) {
		return false;
	}

	bool ImGui::ButtonNoPadding(const std::string & name) {
		return false;
	}

	bool ImGui::ButtonDownNoPadding(const std::string & name) {
		return false;
	}

	bool ImGui::ButtonHoverNoPadding(const std::string & name) {
		return false;
	}

	bool ImGui::CheckBox(const std::string & name, bool & val) {
		return false;
	}

	int ImGui::Select(const std::string names[], int & val, int numChoices) {
		return 0;
	}

	int ImGui::SelectCustom(const std::string names[], int values[], int & val, int numChoices) {
		return 0;
	}

	int ImGui::DropDown(const std::string & name, bool & val) {
		return 0;
	}

	float ImGui::SliderFloat(const std::string & name, float & val, float min, float max) {
		if (ImGuiIsMinimized()) return val;
		const int id = ImGuiGenWidgetId();

		Print(name);

		float sliderVal = min + ImGuiSlider(id, (val - min) / (max - min)) * (max - min);
		val = sliderVal;

		SameLine();
		Print(std::to_string(val));

		return val;
	}

	int ImGui::SliderInt(const std::string & name, int & val, int min, int max) {
		if (ImGuiIsMinimized()) return val;
		const int id = ImGuiGenWidgetId();

		Print(name);

		int width = max - min;
		int sliderVal = min + (int)(ImGuiSlider(id, float(val - min) / (width)) * (width));
		val = sliderVal;

		SameLine();
		Print(std::to_string(val));

		return val;
	}

	void ImGui::Print(const std::string & text) {
		if (ImGuiIsMinimized() || text.size() == 0) return;

		int textWidth = ImGuiTextWidth(text);
		ImGuiColor(COLOR_WHITE);
		ImGuiPrint(text, ImGuiState().DrawPos);
		MoveDrawPosNextLine(Vec2i(textWidth, ImGui::FONT_HEIGHT));
	}

	void ImGui::PrintParagraph(const std::string & text) {
		
	}

	void ImGui::LineGraph(prev::LineGraph & lineGraph) {
		if (ImGuiIsMinimized()) return;

		const Vec2i pos = ImGuiState().DrawPos;
		const Vec2i dimen = lineGraph.GetDimension();

		lineGraph.DrawBackground(ToVec2(pos));
		lineGraph.Draw(ToVec2(pos));

		MoveDrawPosNextLine(dimen);
	}

	void ImGui::BarGraph(float * data, int num_bins, float minVal /*= 0.0f*/, float maxVal /*= 1.0f*/, int hightlightBinIndex /*= -1*/) {
		if (ImGuiIsMinimized()) return;

		const Vec2i pos = ImGuiState().DrawPos;
		int drawAreaWidth = ImGuiWorkingWindow()->Dimension.x;
		const Vec2i dimen = Vec2i(250, 150);

		const float inv_range = 1.0f / (maxVal - minVal);
		float deltaX = dimen.x / (float)num_bins;

		// draw background
		ImGuiColor(Vec3(1.0f), 0.035f);
		ImGuiDrawRect(pos, dimen);
		ImGuiColor(Vec3(1.0f), 0.1f);
		ImGuiDrawRectWire(pos, dimen);

		// background grid lines
		const int num_lines = 3;
		const int deltaXLines = dimen.x >> 2;
		const int deltaYLines = dimen.y >> 2;
		for (int i = 1; i <= num_lines; ++i) {
			const int offsetX = deltaXLines * i;
			const int offsetY = -deltaYLines * i;
			ImGuiDrawLine(pos + Vec2i(0, offsetY), pos + Vec2i(dimen.x, offsetY));
			ImGuiDrawLine(pos + Vec2i(offsetX, 0), pos + Vec2i(offsetX, -dimen.y));
		}

		// draw bars
		for (int i = 0; i < num_bins; ++i) {
			float currVal = data[i];
			float percent = Saturate((currVal - minVal) * inv_range);

			float x0 = (float)pos.x + deltaX * i;
			float x1 = x0 + deltaX;
			float y0 = (float)pos.y - dimen.y;
			float y1 = y0 + percent * dimen.y;

			Vec2 barPos = Vec2(x0, y1);
			Vec2 barDimen = Vec2(x1 - x0, y1 - y0);

			ImGuiColor(Vec3(0.0f, 0.8f, 0.9f), 0.4f);
			if (i == hightlightBinIndex) {
				ImGuiColor(Vec3(1.0f), 0.1f);
				ImGuiDrawRectWire(ToVec2i(barPos + Vec2(0, dimen.y - barDimen.y)), ToVec2i(Vec2(barDimen.x, dimen.y)));
				ImGuiColor(Vec3(1.0f, 1.0f, 0.0f), 0.6f);
			}

			ImGuiDrawRect(ToVec2i(barPos) + Vec2i(1, 0), ToVec2i(barDimen) - Vec2i(1, 0));
		}

		MoveDrawPosNextLine(dimen);
	}

	void ImGui::MoveDrawPosBy(Vec2i dimen) {
		StrongHandle<ImGuiWindow> & window = ImGuiWorkingWindow();

		ImGuiState().DrawPosPrev = ImGuiState().DrawPos;
		ImGuiState().DrawPosPrev.x += dimen.x + WIDGET_PADDING;
		ImGuiState().DrawPos += Vec2i(dimen.x, -dimen.y);

		window->DimensionAutoSize.x = std::max(window->DimensionAutoSize.x, ImGuiState().DrawPos.x - window->Position.x);
		window->DimensionAutoSize.y = std::max(window->DimensionAutoSize.y, window->Position.y - ImGuiState().DrawPos.y);
	}

	void ImGui::MoveDrawPosNextLine(Vec2i dimen) {
		StrongHandle<ImGuiWindow> & window = ImGuiWorkingWindow();

		ImGui::MoveDrawPosBy(dimen + Vec2i(0, WIDGET_PADDING));
		ImGuiState().DrawPos.x = window->Position.x + WINDOW_INSIDE_PADDING + TAB_WIDTH * ImGuiState().NumTabs;
	}

}