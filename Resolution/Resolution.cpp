#include "Resolution.h"

#include <iostream>
#include <stack>
#include <sstream>
#include <string>
#include <string_view>
#include <map>
#include <vector>

using std::map;
using std::istringstream;
using std::stack;
using std::string;
using std::string_view;
using std::vector;

vector<SymbolDesc> symbols = {
	{ "A.dll::B::C", 1234 },
	{ "A.exe::B::C", 4321 },
	{ "B::C", 1000 },
	{ "Q<T1,T2>::A::B::C", 123456789 }
};

void printStack(stack<TagElement*> st)
{
	while (!st.empty()) {
		std::cout << st.top()->tag << '\n';
		st.pop();
	}
	std::cout << "\n";
}

void printTree(TagElement* node, int i = 0, int t = 0)
{
	if (node == nullptr) return;
	std::cout << i << " " << t << " " << node->tag << '\n';
	if (node->next) printTree(node->next, i, t + 1);
	for (auto n : node->templateList) {
		printTree(n, i + 1, t);
	}
}

void linkNode(TagElement* root, TagElement* child)
{
	if (root) {
		while (root->next != nullptr) root = root->next;
		root->next = child;
	}
}

void addNode(stack<TagElement*>& tagStack, TagElement* node)
{
	// if linkTag (i.e., there was just a ::), 
	TagElement* top = tagStack.top();

	if (top->next) {
		std::cout << "link " << top->next->tag << " -> " << node->tag << '\n';
		linkNode(top->next, node);
		top->next = nullptr;
	}
	else {
		std::cout << "insert into " << top->tag << ", " << node->tag << '\n';
		top->templateList.push_back(node);
	}
}

TagElement* parseStringToTree(string const& path)
{
	std::cout << path << "\n\n";
	TagElement* root = new TagElement("<ROOT>");

	stack<TagElement*> tagStack;
	tagStack.push(root);

	size_t currTagStart = 0;

	for (int i = 0; i != path.length(); i++) {
		switch (path[i]) {
		case '<':
		{
			TagElement* tag = new TagElement(path.substr(currTagStart, i - currTagStart));
			tagStack.push(tag);
			currTagStart = i + 1;
			break;
		}
		case ',': // similar to ::?
		{
			// add last tag element, if it didn't have a template
			if (currTagStart != i) {
				// > would noramlly push element, but if there was no template, add a leaf node
				TagElement* tag = new TagElement(path.substr(currTagStart, i - currTagStart));
				addNode(tagStack, tag);
				// std::cout << "leaf node special case\n";
			}
			currTagStart = i + 1;
			break;
		}
		case '>':
		{
			if (tagStack.top() == root) {
				std::cout << "invalid syntax: too many >\n";
				return nullptr;
			}
			// add last tag element, if it didn't have a template
			if (currTagStart != i) {
				TagElement* tag = new TagElement(path.substr(currTagStart, i - currTagStart));
				addNode(tagStack, tag);
				// std::cout << "leaf node specal case (2)\n";
			}
			// pop current tag, and insert it into parent's list
			TagElement* curr = tagStack.top();
			tagStack.pop();
			addNode(tagStack, curr);
			currTagStart = i + 1;
			break;
		}
		case ':':
		{
			if (path[i + 1] != ':') break;

			// add last tag element, if it didn't have a template
			if (currTagStart != i) {
				TagElement* tag = new TagElement(path.substr(currTagStart, i - currTagStart));
				addNode(tagStack, tag);
				// std::cout << "leaf node special case (3)\n";
			}

			// I already put tag creation logic at < , etc so easiest now to just
			// set it so it will link with this then...
			TagElement* linkTag = (*(tagStack.top()->templateList.end() - 1));
			while (linkTag->next) linkTag = linkTag->next;
			tagStack.top()->next = linkTag;

			std::cout << "prev tag: " << linkTag->tag << '\n';
			std::cout << "stack top: " << tagStack.top()->tag << '\n';

			i++; // read next :
			currTagStart = i + 1;
			break;
		}
		default:
			break;
		}
	}

	auto top = tagStack.top();
	tagStack.pop();


	return top;
}

void registerSymbol(SymbolDesc& sym)
{

}

int main()
{
	TagElement* root = parseStringToTree("A<B<C::ASDF::FDSA<Q::R,Z>,E>,D,V>");
	printTree(root);
}