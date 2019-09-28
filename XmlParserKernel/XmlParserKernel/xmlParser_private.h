#pragma once
#include  "xmldef.h"

#include "rapidxml.hpp"
#include "rapidxml_utils.hpp"
#include "rapidxml_print.hpp"
using namespace rapidxml;

class xmlNode: public xml_node<char>
{
public:
	xmlNode()
		:xml_node<char>(rapidxml::node_element)
	{
	}

	~xmlNode()
	{

	}
};


class xmlAttr : public xml_attribute<char>
{
public:
	xmlAttr()
		:xml_attribute<char>()
	{

	}
	~xmlAttr()
	{

	}
};
