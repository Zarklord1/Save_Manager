#include <string.h>
#include <stdlib.h>
#include "tinyxml.h"

#include "MetaXML.h"

bool MetaXML::LoadMetaXMLData(const char* filename, const char * region)
{
	ShortName.clear();
	LongName.clear();
	Publisher.clear();
	ProductCode.clear();
	TitleID.clear();
	MaxCommonSize.clear();
	MaxAccountSize.clear();

	TiXmlDocument xmlDoc(filename);
	if(!xmlDoc.LoadFile())
		return false;

	TiXmlElement *appNode =  xmlDoc.FirstChildElement("menu");
	if(!appNode)
		return false;

	TiXmlElement *node = NULL;
	std::string temp("");

	temp = "shortname_";
	temp += region;
	node = appNode->FirstChildElement(temp);
	if(node && node->FirstChild() && node->FirstChild()->Value())
		ShortName = node->FirstChild()->Value();

	temp = "longname_";
	temp += region;
	node = appNode->FirstChildElement(temp);
	if(node && node->FirstChild() && node->FirstChild()->Value())
		LongName = node->FirstChild()->Value();

	temp = "publisher_";
	temp += region;
	node = appNode->FirstChildElement(temp);
	if(node && node->FirstChild() && node->FirstChild()->Value())
		Publisher = node->FirstChild()->Value();

	node = appNode->FirstChildElement("product_code");
	if(node && node->FirstChild() && node->FirstChild()->Value())
		ProductCode = node->FirstChild()->Value();

	node = appNode->FirstChildElement("title_id");
	if(node && node->FirstChild() && node->FirstChild()->Value())
		TitleID = node->FirstChild()->Value();
		TitleID.insert(8,"-");

	node = appNode->FirstChildElement("common_save_size");
	if(node && node->FirstChild() && node->FirstChild()->Value())
		MaxCommonSize = node->FirstChild()->Value();
		u64 commonDec = strtol(MaxCommonSize.c_str(),NULL,16);
		MaxCommonSize = convertSize(commonDec);

	node = appNode->FirstChildElement("account_save_size");
	if(node && node->FirstChild() && node->FirstChild()->Value())
		MaxAccountSize = node->FirstChild()->Value();
		u64 accountDec = strtol(MaxAccountSize.c_str(),NULL,16);
		MaxAccountSize = convertSize(accountDec);
		
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