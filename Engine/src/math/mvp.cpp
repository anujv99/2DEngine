#include "pch.h"
#include "mvp.h"

#include "graphics/window.h"
#include "vecconversion.h"

namespace prev {

	MVP::MVP() {}

	MVP::~MVP() {}

	prev::Mat4 MVP::Get() {
		if (Model().IsDirty() || View().IsDirty() || Projection().IsDirty()) {
			Mat4 viewInv = prev::inverseRT(View().Get());
			m_MVPCache = Projection().Get() * View().Get() * Model().Get();
			Model().SetDirtyOff();
			Projection().SetDirtyOff();
			View().SetDirtyOff();
		}

		return m_MVPCache;
	}

	bool MVP::IsDirty() {
		return Model().IsDirty() || View().IsDirty() || Projection().IsDirty();
	}

}