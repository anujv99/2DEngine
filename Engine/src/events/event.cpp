#include "pch.h"
#include "event.h"

#include "src/common/error.h"

namespace prev {

	std::vector<prev::EventHandler::EventFunc> EventHandler::s_EventFunctions;
	std::vector<Event *> EventHandler::s_EventQueue;

	void EventHandler::RegisterEventFunction(EventFunc func) {
		if (func) {
			s_EventFunctions.push_back(func);
			return;
		}
		ERROR_TRACE(ERR_TYPE_INVALID_FUNCTION, "Trying to bind invalid event function");
	}

	void EventHandler::FlushEventQueue() {
		for (auto & e : s_EventQueue) {
			for (auto & f : s_EventFunctions) {
				f(*e);
			}
		}
	}

	void EventHandler::EventHappened(Event * e) {
		s_EventQueue.push_back(e);
	}

}