#pragma once

#include "common/error.h"

namespace prev {

	template<class T>
	class Singleton {
	public:
		template<typename ... Args>
		static void CreateInst(const Args & ... arg);
		static void DestroyInst();

		inline static bool InstExists();
		inline static T * Get();
		inline static T & Ref();
	private:
		static T * s_Instance;
	};

	template<class T>
	template<typename ... Args>
	inline void Singleton<T>::CreateInst(const Args & ... arg) {
		if (!s_Instance) s_Instance = new T(arg ...);
	}

	template<class T>
	inline void Singleton<T>::DestroyInst() {
		if (s_Instance != nullptr) {
			delete s_Instance;
			s_Instance = nullptr;
		}
	}

	template<class T>
	inline bool Singleton<T>::InstExists() {
		return s_Instance != nullptr;
	}

	template<class T>
	inline T * Singleton<T>::Get() {
		return s_Instance;
	}

	template<class T>
	inline T & Singleton<T>::Ref() {
		ASSERT(s_Instance != nullptr);
		return *s_Instance;
	}

	template<class T>
	T * Singleton<T>::s_Instance = nullptr;

}
