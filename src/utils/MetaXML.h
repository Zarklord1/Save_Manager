#pragma once

#include <string>
#include <wut_types.h>

class MetaXML {
	public:
		MetaXML() { };
		MetaXML(const char* filename) { LoadMetaXMLData(filename); };
		MetaXML(const char* filename, const char * region) { LoadMetaXMLData(filename, region); };

		bool LoadMetaXMLData(const char* filename, const char * region = "en");

		const char * GetShortName() const;
		const char * GetLongName() const;
		const char * GetPublisher() const;
		const char * GetProductCode() const;
		const char * GetTitleID() const;
		const char * GetMaxCommonSize() const;
		const char * GetMaxAccountSize() const;

	protected:
		std::string ShortName;
		std::string LongName;
		std::string Publisher;
		std::string ProductCode;
		std::string TitleID;
		std::string MaxCommonSize;
		std::string MaxAccountSize;

	private:
		std::string convertSize(u64 size);
};