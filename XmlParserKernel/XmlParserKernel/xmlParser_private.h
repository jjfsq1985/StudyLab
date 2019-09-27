#pragma once
#include  "xmldef.h"

#include "rapidxml.hpp"
#include "rapidxml_utils.hpp"
#include "rapidxml_print.hpp"
using namespace rapidxml;

class xmlNode: public xml_node<wchar_t>
{
public:
	xmlNode()
		:xml_node<wchar_t>(rapidxml::node_element)
	{
	}

	~xmlNode()
	{

	}
};


class xmlAttr : public xml_attribute<wchar_t>
{
public:
	xmlAttr()
		:xml_attribute<wchar_t>()
	{

	}
	~xmlAttr()
	{

	}
};
