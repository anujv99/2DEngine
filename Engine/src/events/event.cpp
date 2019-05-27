#include "pch.h"
#include "event.h"

#include "src/helper/error.h"

namespace prev {

	std::vector<prev::EventHandler::EventFunc> EventHandler::m_EventFunctions;
	std::vector<Event *> EventHandler::m_EventQueue;

	void EventHandler::RegisterEventFunction(EventFunc func) {
		if (func) {
			m_EventFunctions.push_back(func);
			return;
		}
		ERROR_TRACE(ERR_TYPE_INVALID_FUNCTION, "Trying to bind invalid event function");
	}

	void EventHandler::FlushEventQueue() {
		for (auto & e : m_EventQueue) {
			for (auto & f : m_EventFunctions) {
				f(*e);
			}
		}
	}

	void EventHandler::EventHappened(Event * e) {
		m_EventQueue.push_back(e);
	}

}