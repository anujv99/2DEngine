#include "pch.h"
#include "event.h"

#include "src/common/error.h"

namespace prev {

	void EventHandler::RegisterEventFunction(EventFunc func) {
		if (func) {
			s_EventFunctions.push_back(func);
			return;
		}
		ERROR_TRACE(ERR_TYPE_INVALID_FUNCTION, "Trying to bind invalid event function");
	}

	void EventHandler::FlushEventQueue() {
		while (!s_EventQueue.empty()) {
			StrongHandle<Event> event = s_EventQueue.front();
			s_EventQueue.pop();
			for (auto & func : s_EventFunctions) {
				func(*event);
			}
		}
	}

}