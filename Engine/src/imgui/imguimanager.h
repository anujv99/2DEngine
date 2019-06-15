#pragma once

#include "graphics/font.h"
#include "graphics/shadermanager.h"
#include "graphics/uniform.h"

#include "math/cam2d.h"

#include "imguiwindow.h"

namespace prev {

	class ImGuiManager : public Singleton<ImGuiManager> {
		friend class Singleton<ImGuiManager>;
	private:
		ImGuiManager();
		~ImGuiManager();
	public:
		struct State {
			union {
				float			F;
				int				I;
				unsigned int	UInt;
				struct { float	FVec[4]; };
				struct { int	IVec[4]; };
			} Data;
			
			Vec2i	DrawPos;
			Vec2i	DrawPosPrev;
			Vec2i	ScrollOffset;
			int		NumTabs;

			Vec2i MouseRegionStart;
			Vec2i MouseRegionDimensions;

			static const int WIDGET_NULL = -1;
			static const int WIDGET_WINDOW = 1;

			int ActiveWidgetID;
			int WidgetCount;
			bool DoesWidgetConsumeTextInput;

			StrongHandle<ImGuiWindow> ActiveWindow;
			StrongHandle<ImGuiWindow> WorkingWindow;

			void Reset();
		} state;

		void PreUpdate();
		void PostUpdate();
		void ClearActiveWindow();
		void ClearAllWindow();
		void OnResizeScreen(Vec2i prevDimen, Vec2i newDimen);

		void SetActiveWindow(StrongHandle<ImGuiWindow> window);
		void SetActiveWidgetId(int id);
		int GetWindowOrderIndex(const std::string & name) const;
		void BringWindowToFront(StrongHandle<ImGuiWindow> & window);
		bool IsPosInWindow(Vec2i pos, StrongHandle<ImGuiWindow> window) const;

		void DetectConsumeInputs();
		bool DidMouseButtonGoDown(int button);

		Font & GetFont() { return m_Font; }
		StrongHandle<VertexShader> & GetVertexShader() { return m_VertexShader; }
		StrongHandle<PixelShader> & GetPixeShader() { return m_PixelShader; }
		Cam2D & GetCamera() { return m_Camera; }

		StrongHandle<ImGuiWindow> GetWindow(const std::string & name);
		std::unordered_map<uint32_t, StrongHandle<ImGuiWindow>> & GetWindowList() { return m_WindowMap; }
	private:
		void PruneVisibleWindow();
		void UpdateWindowMap();
		void DebugPrintVisibleWindows();
	private:
		std::unordered_map<uint32_t, StrongHandle<ImGuiWindow>> m_WindowMap;
		std::vector <StrongHandle<ImGuiWindow>> m_VisibleWindows;

		Cam2D m_Camera;
		Font m_Font;
		StrongHandle<VertexShader> m_VertexShader;
		StrongHandle<PixelShader> m_PixelShader;

		std::set<int> m_ConsumeMouseButtons;
		std::set<uint32_t> m_ConsumeKeyPress;
	};

}
