#pragma once

#include <iostream>
#include <sstream>
#include <string>
#include <string_view>
#include <map>
#include <vector>

using std::map;
using std::istringstream;
using std::string;
using std::string_view;
using std::vector;
using PTR_T = unsigned int; // 32-bit

class SymbolDesc {
public:
	string path;
	PTR_T address;
};

class Symbol {
public:
	SymbolDesc* symdesc;
	int len;
};

class TagElement; // forward declaration

// information in the tail TagElement
class TailInfo {
public:
	int l;
	TagElement* parameters;
};

class TagElement {
public:
	string tag;
	vector<TagElement*> templateList; // comma separated
	TagElement* next;
	bool hasBrackets; // distinguish between <> and no information
	TailInfo* tailInfo;

	TagElement(string _tag) :
		tag(_tag),
		templateList({}),
		next(nullptr),
		hasBrackets(false),
		tailInfo(nullptr) // non-null for tail element
	{}
};