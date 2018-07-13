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

void ShowHelp() {
  std::cout << "fusion_xml.exe -id filter inputA inputB <optional OutputC>" << std::endl;
  std::cout << "<optional id filer> -id <filter>" << std::endl;
}

int main(int argc, char* argv[]) {
  if (argc >= 5 && argc <= 6) {
    const String empty = "";
    String filter = argv[2];
    String inputA = argv[3];
    String inputB = argv[4];
    String outputC;
    if (argc > 5) {
      outputC = argv[5];
    }

    if (inputA != empty && inputB != empty) {
      xml_document<Char> a;
      xml_document<Char> b;
      file<Char> aFile(inputA.c_str());
      file<Char> bFile(inputB.c_str());
      try {
        a.parse<parse_default>(const_cast<Char*>(aFile.data()));
        b.parse<parse_default>(const_cast<Char*>(bFile.data()));
      }
      catch (...) {
        return 1;
      }

      xml_document<Char> c;
      FusionXML<String, Char> fusion(filter);
      fusion.Union(a, b, c);
      String xmlAsString;
      print(back_inserter(xmlAsString), c);
        std::ofstream fileStream(outputC);

      if (outputC != empty) {
        fileStream << xmlAsString;
        fileStream.close();
      }
      else {
        std::cout << xmlAsString.c_str() << std::endl;
      }
    }
  }
  else {
    ShowHelp();
  }
  return 0;
}