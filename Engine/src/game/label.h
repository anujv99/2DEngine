#pragma once

#include <string>

#include "sprite.h"

namespace prev {

	enum LabelAlignment {
		PV_LABEL_ALIGNMENT_LEFT,
		PV_LABEL_ALIGNMENT_RIGHT,
		PV_LABEL_ALIGNMENT_MIDDLE,
	};

	struct Label : public Sprite {
		inline void operator=(const std::string & text) {
			m_Text = text;
			m_IsChanged = true;
		}

		inline void SetText(const std::string & text) {	m_Text = text; }
		inline std::string GetText() const { return m_Text;	}

		inline void SetDirty(bool dirty) const { m_IsChanged = dirty; }
		inline bool GetDirty() const { return m_IsChanged; }
	public:
		LabelAlignment Alignment = PV_LABEL_ALIGNMENT_RIGHT;
	private:
		std::string m_Text;
		mutable bool m_IsChanged = false;
	};

}
