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

class Element;

class TagElement {
public:
	string tag;
	vector<TagElement*> templateList; // comma separated
	TagElement* next;
	TagElement(string _tag) : tag(_tag), templateList({}), next(nullptr) {}
};

// Recursively self-contained resolution map (kind of like a linked list).
class Node {
public:
	map<string, Node> children;
	vector<Element> elements; // tag matches, but template args may not
	SymbolDesc* symbol; // if non-null then end node
};