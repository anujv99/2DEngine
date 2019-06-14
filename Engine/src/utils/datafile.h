#pragma once

namespace prev {

	class DataFile {
	public:
		DataFile();
		~DataFile();

		void Load(const std::string & fileName, bool isBinary = false);

		inline void * GetData() const { return (void *)m_Data; }
		inline unsigned int GetNumBytes() const { return m_NumBytes; }
		inline std::string GetExtension() const { return m_Ext; }
	private:
		void Release();
	private:
		std::string m_Ext;
		unsigned int m_NumBytes;
		char * m_Data;
	};

}