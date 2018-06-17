#include "rapidxml/rapidxml.hpp"
#include "rapidxml/rapidxml_utils.hpp"
#include "rapidxml/rapidxml_print.hpp"

#include <iostream>

#include "FusionXML.h"
#include "FusionXML.cpp"
//#include "StreamUTF8.h"

using namespace rapidxml;

//using Char = wchar_t;
//using String = std::wstring;

using Char = char;
using String = std::string;

int main(int argc, char* argv[]) {
//  setlocale(LC_ALL, "");
  
  if (argc == 4) {
    xml_document<Char> a;
    xml_document<Char> b;

    try {
      file<Char> aFile(argv[1]);
      file<Char> bFile(argv[2]);
      a.parse<parse_fastest>(const_cast<Char*>(aFile.data()));
      b.parse<parse_fastest>(const_cast<Char*>(bFile.data()));
    }
    catch (...) {
      return 1;
    }

    xml_document<Char> c;
    FusionXML<String, Char> fusion;
    fusion.Union(a, b, c);

    String xmlAsString;
    print(back_inserter(xmlAsString), c);
    printf("%s", xmlAsString.c_str());
    std::cout << xmlAsString.c_str() << std::endl;

    return 1;
  }

  // basic test
  xml_document<Char> origin;
  {
    auto root = origin.allocate_node(node_element, "ROOT");
    auto guid = origin.allocate_attribute("id", "0101");
    root->append_attribute(guid);
    auto ver = origin.allocate_attribute("ver", "1.0");
    root->append_attribute(ver);

    auto a = origin.allocate_node(node_element, "A");
    root->append_node(a);

    auto b = origin.allocate_node(node_element, "B");
    root->append_node(b);

    auto c = origin.allocate_node(node_element, "C");
    c->append_attribute(origin.allocate_attribute("attr", "at"));
    root->append_node(c);

    auto d = origin.allocate_node(node_element, "D");
    d->append_node(origin.allocate_node(node_element, "DA"));
    d->append_node(origin.allocate_node(node_element, "DB"));
    d->append_node(origin.allocate_node(node_element, "DC"));
    root->append_node(d);

    auto f = origin.allocate_node(node_element, "F");
    auto fuid = origin.allocate_attribute("id", "0101");
    f->append_attribute(fuid);
    auto fa = origin.allocate_node(node_element, "FA");
    auto fauid = origin.allocate_attribute("id", "0101");
    fa->append_attribute(fauid);
    fa->append_attribute(origin.allocate_attribute("attr", "at"));
    fa->append_attribute(origin.allocate_attribute("otherAttr", "bt"));
    f->append_node(fa);
    f->append_node(origin.allocate_node(node_element, "FB"));
    root->append_node(f);

    auto f2 = origin.allocate_node(node_element, "F");
    auto fuid2 = origin.allocate_attribute("id", "0101");
    f2->append_attribute(fuid2);
    f2->append_node(origin.allocate_node(node_element, "FB"));
    root->append_node(f2);

    auto g = origin.allocate_node(node_element, "G");
    root->append_node(g);
    origin.append_node(root);
  }

  xml_document<Char> destin;
  {
    auto root = destin.allocate_node(node_element, "ROOT");
    auto guid = destin.allocate_attribute("id", "0101");
    root->append_attribute(guid);
    auto code = origin.allocate_attribute("code", "beta");
    root->append_attribute(code);
    destin.append_node(root);

    auto a = destin.allocate_node(node_element, "A");
    root->append_node(a);

    auto b = destin.allocate_node(node_element, "B");
    b->append_attribute(destin.allocate_attribute("attr", "at"));
    root->append_node(b);

    auto c = destin.allocate_node(node_element, "C");
    c->append_node(destin.allocate_node(node_element, "CA"));
    c->append_node(destin.allocate_node(node_element, "CB"));
    root->append_node(c);

    auto e = destin.allocate_node(node_element, "E");
    root->append_node(e);

    auto f = destin.allocate_node(node_element, "F");
    auto fuid = destin.allocate_attribute("id", "0101");
    f->append_attribute(fuid);
    auto fa = origin.allocate_node(node_element, "FA");
    fa->append_attribute(origin.allocate_attribute("attr", "at"));
    auto fauid = destin.allocate_attribute("id", "0101");
    fa->append_attribute(fauid);
    auto fb = destin.allocate_node(node_element, "FB");
    fb->append_node(destin.allocate_node(node_element, "FBA"));
    f->append_node(fb);
    root->append_node(f);
  }

  FusionXML<String, Char> fusion("id");
  xml_document<Char> result;
  fusion.Union(origin, destin, result);

  std::string fileText;
  print(back_inserter(fileText), result);
  printf("%s", fileText.c_str());

  return 0;
}