#include <string.h>
#include <stdlib.h>
#include "tinyxml2.h"
#include "logger.h"

#include "MetaXML.h"

bool MetaXML::LoadMetaXMLData(const char * filename, const char * region) {
	ShortName.clear();
	LongName.clear();
	Publisher.clear();
	ProductCode.clear();
	TitleID.clear();
	MaxCommonSize.clear();
	MaxAccountSize.clear();

	tinyxml2::XMLDocument doc;
	doc.LoadFile(filename);
	if(doc.ErrorID() != 0)
		return false;

	tinyxml2::XMLElement * curElement = NULL;
	std::string temp("");

	temp = "shortname_";
	temp += region;
	curElement = doc.FirstChildElement("menu")->FirstChildElement(temp.c_str());
	if (curElement && curElement->GetText()) {
		ShortName = curElement->GetText();
	}

	temp = "longname_";
	temp += region;
	curElement = doc.FirstChildElement("menu")->FirstChildElement(temp.c_str());
	if (curElement && curElement->GetText()) {
		LongName = curElement->GetText();
	}

	temp = "publisher_";
	temp += region;
	curElement = doc.FirstChildElement("menu")->FirstChildElement(temp.c_str());
	if (curElement && curElement->GetText()) {
		Publisher = curElement->GetText();
	}

	curElement = doc.FirstChildElement("menu")->FirstChildElement("product_code");
	if (curElement && curElement->GetText()) {
		ProductCode = curElement->GetText();
	}

	curElement = doc.FirstChildElement("menu")->FirstChildElement("title_id");
	if (curElement && curElement->GetText()) {
		TitleID = curElement->GetText();
		TitleID.insert(8,"-");
	}

	curElement = doc.FirstChildElement("menu")->FirstChildElement("common_save_size");
	if (curElement && curElement->GetText()) {
		MaxCommonSize = curElement->GetText();
		u64 commonDec = strtoll(MaxCommonSize.c_str(),NULL,16);
		MaxCommonSize = convertSize(commonDec);
	}

	curElement = doc.FirstChildElement("menu")->FirstChildElement("account_save_size");
	if (curElement && curElement->GetText()) {
		MaxAccountSize = curElement->GetText();
		u64 accountDec = strtoll(MaxAccountSize.c_str(),NULL,16);
		MaxAccountSize = convertSize(accountDec);
	}
		
	return true;
}

/* Get short name */
const char * MetaXML::GetShortName() const
{
	return ShortName.c_str();
}

/* Get long name */
const char * MetaXML::GetLongName() const
{
	return LongName.c_str();
}

/* Get publisher */
const char * MetaXML::GetPublisher() const
{
	return Publisher.c_str();
}

/* Get product code */
const char * MetaXML::GetProductCode() const
{
	return ProductCode.c_str();
}

/* Get title id */
const char * MetaXML::GetTitleID() const
{
	return TitleID.c_str();
}

/* Get maximum common size */
const char * MetaXML::GetMaxCommonSize() const
{
	return MaxCommonSize.c_str();
}

/* Get maximum unique size */
const char * MetaXML::GetMaxAccountSize() const
{
	return MaxAccountSize.c_str();
}

std::string MetaXML::convertSize(u64 size) {               
    static const char *SIZES[] = {"b", "kb", "mb", "gb"};
    int div = 0;

    while (size >= 1024 && div < (sizeof SIZES / sizeof *SIZES)) {
        div++;
        size /= 1024;
    }

    char buffer[20];
    sprintf(buffer, "%llu%s", size, SIZES[div]);
    std::string result = buffer;
    return result;
}