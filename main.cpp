#include <iostream>

#include "rapidxml/rapidxml.hpp"
#include "rapidxml/rapidxml_print.hpp"

#include "FusionXML.h"

using namespace rapidxml;

int main() {
    xml_document<char> origin;
    {
        auto root = origin.allocate_node(node_element, "ROOT");
        auto guid = origin.allocate_attribute("id", "e32595d7-5379-41a3-bacf-72137581be28");
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
        auto fuid = origin.allocate_attribute("id", "e32595d7-5379-41a3-bacf-72137581be28");
        f->append_attribute(fuid);
        auto fa = origin.allocate_node(node_element, "FA");
        fa->append_attribute(origin.allocate_attribute("attr", "at"));
        fa->append_attribute(origin.allocate_attribute("otherAttr", "bt"));
        f->append_node(fa);
        f->append_node(origin.allocate_node(node_element, "FB"));
        root->append_node(f);

        auto f2 = origin.allocate_node(node_element, "F");
        auto fuid2 = origin.allocate_attribute("id", "e32595d7-5379-41a3-bacf-72137581be28");
        f2->append_attribute(fuid2);
        auto fa2 = origin.allocate_node(node_element, "FA");
        fa2->append_attribute(origin.allocate_attribute("attr", "at"));
        fa2->append_attribute(origin.allocate_attribute("otherAttr", "bt"));
        f2->append_node(fa2);
        f2->append_node(origin.allocate_node(node_element, "FB"));
        root->append_node(f2);

        auto g = origin.allocate_node(node_element, "G");
        root->append_node(g);
        origin.append_node(root);
    }

    xml_document<char> destin;
    {
        auto root = destin.allocate_node(node_element, "ROOT");
        auto guid = destin.allocate_attribute("id", "e32595d7-5379-41a3-bacf-72137581be28");
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
        auto fuid = destin.allocate_attribute("id", "e32595d7-5379-41a3-bacf-72137581be28");
        f->append_attribute(fuid);
        f->append_node(destin.allocate_node(node_element, "FA"));
        auto fb = destin.allocate_node(node_element, "FB");
        fb->append_node(destin.allocate_node(node_element, "FBA"));
        f->append_node(fb);
        root->append_node(f);
    }

    FusionXML fusion("id");
    xml_document<char>* result = fusion.Union(origin, destin);

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