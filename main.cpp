#pragma warning(push, 0)  
#include "rapidxml/rapidxml.hpp"
#include "rapidxml/rapidxml_utils.hpp"
#include "rapidxml/rapidxml_print.hpp"
#pragma warning(pop)  

#include <iostream>

#include "FusionXML.h"
#include "FusionXML.cpp"

using namespace rapidxml;

//using Char = wchar_t;
//using String = std::wstring;

using Char = char;
using String = std::string;

int main(int argc, char* argv[]) {

  if (argc == 3) {
    xml_document<Char> a;
    xml_document<Char> b;
    file<Char> aFile(argv[1]);
    file<Char> bFile(argv[2]);
    try {
      a.parse<parse_default>(const_cast<Char*>(aFile.data()));
      b.parse<parse_default>(const_cast<Char*>(bFile.data()));
    }
    catch (...) {
      return 1;
    }

    xml_document<Char> c;
    FusionXML<String, Char> fusion("id");
    fusion.Union(a, b, c);

    String xmlAsString;
    print(back_inserter(xmlAsString), c);
    std::cout << xmlAsString.c_str() << std::endl;

    return 1;
  }

  return 0;
}