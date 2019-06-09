#pragma once

#include "bindable.h"
#include "vertexshader.h"
#include "datatypes.h"

namespace prev {

	class VertexLayout : public HandledObject<VertexLayout>, public Bindable {
	protected:
		VertexLayout() : m_IsBuilding(false), m_NumEntries(0) {}
	public:
		virtual ~VertexLayout() {}

		virtual void Bind() = 0;
		virtual void UnBind() = 0;
		
		virtual BindableType GetType() const override {
			return BindableType::VERTEX_LAYOUT;
		}

		static StrongHandle<VertexLayout> CreateVertexLayout();
	protected:
		virtual bool BuildVertexLayout(StrongHandle<VertexShader> vertexShader) = 0;
	public:
		void BeginEntries() {
			m_IsBuilding = true;
		}
		void AddEntry(LayoutDataType dataType, unsigned int numElements, size_t offsetBytes, const std::string & layoutName, bool normalize) {
			ASSERT(m_IsBuilding);
			ASSERT(m_NumEntries < MAX_ENTRIES);
			ASSERT(numElements <= MAX_ELEMENT && numElements > 0);

			if (numElements == 3) {
				if (dataType != PV_DATA_TYPE_FLOAT_32 && dataType != PV_DATA_TYPE_SINT_32 && dataType != PV_DATA_TYPE_UINT_32) {
					LOG_ERROR(
						"Cannot use 3 comonent with Data type whose component size is less than 32 bit\n"
						"Use PV_DATA_TYPE_FLOAT_32 or PV_DATA_TYPE_SINT_32 or PV_DATA_TYPE_UINT_32 if using 3 component vertex"
					);
					return;
				}
			}

			Entry & entry = m_Entries[m_NumEntries];
			entry.EntryDataType = dataType;
			entry.NumElements = numElements;
			entry.OffsetBytes = offsetBytes;
			entry.LayoutName = layoutName;
			entry.Normalized = normalize;

			m_NumEntries++;
		}
		//Vertex Shader required by DirectX
		void EndEntries(StrongHandle<VertexShader> vertexShader) {
			ASSERT(m_IsBuilding);
			m_IsBuilding = false;

			if (m_NumEntries == 0)
				return;

			BuildVertexLayout(vertexShader);
		}
	protected:
		struct Entry {
			bool Normalized;
			LayoutDataType EntryDataType;
			unsigned int NumElements;
			size_t OffsetBytes;
			std::string LayoutName; //Used by DirectX
		};

		static const unsigned int MAX_ENTRIES = 8;
		static const unsigned int MAX_ELEMENT = 4;
		Entry m_Entries[MAX_ENTRIES];
		unsigned int m_NumEntries;

		bool m_IsBuilding;
	};

}
