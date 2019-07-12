#pragma once

#include <string>

#include "sprite.h"

namespace prev {

	struct Label : public Sprite {
		inline void operator=(const std::string & text) {
			m_Text = text;
			m_IsChanged = true;
		}

		inline void SetText(const std::string & text) {	m_Text = text; }
		inline std::string GetText() const { return m_Text;	}

		inline void SetDirty(bool dirty) const { m_IsChanged = dirty; }
		inline bool GetDirty() const { return m_IsChanged; }

		inline void SetPosition(Vec2 pos) { m_Position = pos; }
		inline Vec2 GetPosition() const { return m_Position; }

		inline void SetColor(Vec4 col) { m_Color = col; }
		inline SpriteColor GetColor() const { return m_Color; }
	private:
		std::string m_Text;
		mutable bool m_IsChanged = false;

		Vec2 m_Position;
		SpriteColor m_Color;
	};

}
