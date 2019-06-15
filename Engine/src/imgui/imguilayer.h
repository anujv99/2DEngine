#pragma once

#include "utils/layer.h"

namespace prev {

	class ImGuiLayer : public Singleton<ImGuiLayer> {
	public:
		ImGuiLayer();
		~ImGuiLayer();

		void StartFrame();
		void EndFrame();
	private:
		void OnImGuiUpdate();

	};

}
