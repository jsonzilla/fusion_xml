#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <locale>
#include <codecvt>

#include "rapidxml/rapidxml.hpp"
#include "rapidxml/rapidxml_iterators.hpp"
#include "rapidxml/rapidxml_utils.hpp"
#include "rapidxml/rapidxml_print.hpp"

#include "FusionXML.h"

using namespace rapidxml;

namespace {
  void ReadInUTF8(const std::wstring& filePath, std::wstringstream& stream) {
    std::wifstream wif(filePath);
    wif.imbue(std::locale(std::locale::empty(), new std::codecvt_utf8<wchar_t, 0x10ffff, std::consume_header>));
    stream << wif.rdbuf();
  }

  void WriteInUTF8(const std::wstring& filePath, const std::wstring& content) {
    std::wofstream wof;
    wof.imbue(std::locale(std::locale::empty(), new std::codecvt_utf8<wchar_t, 0x10ffff, std::generate_header>));
    wof.open(filePath);
    wof << content;
    wof.close();
  }
}

int main(int argc, char* argv[]) {
  xml_document<wchar_t> a;
  xml_document<wchar_t> b;
  
  if (argc == 3) {
    try {
      file<wchar_t> aFile(argv[1]);
      file<wchar_t> bFile(argv[2]);
      a.parse<parse_fastest>(const_cast<wchar_t*>(aFile.data()));
      b.parse<parse_fastest>(const_cast<wchar_t*>(bFile.data()));
    }
    catch (...) {
      return 1;
    }

    FusionXML fusion(L"id");
    xml_document<wchar_t>* result = fusion.Union(a, b);

    std::wstring fileText;
    print(back_inserter(fileText), *result);
    printf("%lls", fileText.c_str());
    WriteInUTF8(L"out.xml", fileText);

    return 1;
  }

  // basic test
  xml_document<wchar_t> origin;
  {
    auto root = origin.allocate_node(node_element, L"ROOT");
    auto guid = origin.allocate_attribute(L"id", L"0101");
    root->append_attribute(guid);
    auto ver = origin.allocate_attribute(L"ver", L"1.0");
    root->append_attribute(ver);

    auto a = origin.allocate_node(node_element, L"A");
    root->append_node(a);

    auto b = origin.allocate_node(node_element, L"B");
    root->append_node(b);

    auto c = origin.allocate_node(node_element, L"C");
    c->append_attribute(origin.allocate_attribute(L"attr", L"at"));
    root->append_node(c);

    auto d = origin.allocate_node(node_element, L"D");
    d->append_node(origin.allocate_node(node_element, L"DA"));
    d->append_node(origin.allocate_node(node_element, L"DB"));
    d->append_node(origin.allocate_node(node_element, L"DC"));
    root->append_node(d);

    auto f = origin.allocate_node(node_element, L"F");
    auto fuid = origin.allocate_attribute(L"id", L"0101");
    f->append_attribute(fuid);
    auto fa = origin.allocate_node(node_element, L"FA");
    auto fauid = origin.allocate_attribute(L"id", L"0101");
    fa->append_attribute(fauid);
    fa->append_attribute(origin.allocate_attribute(L"attr", L"at"));
    fa->append_attribute(origin.allocate_attribute(L"otherAttr", L"bt"));
    f->append_node(fa);
    f->append_node(origin.allocate_node(node_element, L"FB"));
    root->append_node(f);

    auto f2 = origin.allocate_node(node_element, L"F");
    auto fuid2 = origin.allocate_attribute(L"id", L"0101");
    f2->append_attribute(fuid2);
    f2->append_node(origin.allocate_node(node_element, L"FB"));
    root->append_node(f2);

    auto g = origin.allocate_node(node_element, L"G");
    root->append_node(g);
    origin.append_node(root);
  }

  xml_document<wchar_t> destin;
  {
    auto root = destin.allocate_node(node_element, L"ROOT");
    auto guid = destin.allocate_attribute(L"id", L"0101");
    root->append_attribute(guid);
    auto code = origin.allocate_attribute(L"code", L"beta");
    root->append_attribute(code);
    destin.append_node(root);

    auto a = destin.allocate_node(node_element, L"A");
    root->append_node(a);

    auto b = destin.allocate_node(node_element, L"B");
    b->append_attribute(destin.allocate_attribute(L"attr", L"at"));
    root->append_node(b);

    auto c = destin.allocate_node(node_element, L"C");
    c->append_node(destin.allocate_node(node_element, L"CA"));
    c->append_node(destin.allocate_node(node_element, L"CB"));
    root->append_node(c);

    auto e = destin.allocate_node(node_element, L"E");
    root->append_node(e);

    auto f = destin.allocate_node(node_element, L"F");
    auto fuid = destin.allocate_attribute(L"id", L"0101");
    f->append_attribute(fuid);
    auto fa = origin.allocate_node(node_element, L"FA");
    fa->append_attribute(origin.allocate_attribute(L"attr", L"at"));
    auto fauid = destin.allocate_attribute(L"id", L"0101");
    fa->append_attribute(fauid);
    auto fb = destin.allocate_node(node_element, L"FB");
    fb->append_node(destin.allocate_node(node_element, L"FBA"));
    f->append_node(fb);
    root->append_node(f);
  }

  FusionXML fusion(L"id");
  xml_document<wchar_t>* result = fusion.Union(origin, destin);

  std::string fileText;
  print(back_inserter(fileText), *result);
  printf("%s", fileText.c_str());

  std::cout << "######################################" << std::endl;
  std::cout << "######################################" << std::endl;
  std::cout << "######################################" << std::endl;
  std::cout << "######################################" << std::endl;
  std::cout << "######################################" << std::endl;
  return 0;
}