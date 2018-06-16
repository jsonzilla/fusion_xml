#include "FusionXML.h"

using namespace rapidxml;

FusionXML::FusionXML() {
}

FusionXML::FusionXML(const std::string &idFilter) : idFilter(idFilter) {
}

FusionXML::~FusionXML() {
}

rapidxml::xml_document<char>* FusionXML::Union(const Document& a, const Document& b) {
    delete doc;
    doc = new Document();
    //HIRO deve encontrar primeiramente uma raiz em comum
    const Node *aNode = a.first_node();
    const Node *bNode = b.first_node();
    auto c = doc->allocate_node(node_element, aNode->name());
    UnionAtributes(aNode, bNode, c);
    Union(aNode, bNode, c);
    doc->append_node(c);
    return doc;
}

void FusionXML::Union(const Node *a, const Node *b, Node *c) {
    NodeMultiMap unionMap;
    PopulateMapNodes(b, unionMap);
    PopulateMapNodes(a, unionMap);

    for (auto&& u : unionMap) {
        const Key k = u.first;
        if (k.unify) {
            Unify(k, unionMap, c);
        }
        c->append_node(doc->clone_node(u.second));
    }
}

void FusionXML::Unify(const Key &k, NodeMultiMap map, Node *c) {
    if (map.count(k) > 1) {
        auto d = doc->allocate_node(node_element, k.name.c_str());
        auto r = map.equal_range(k);
        auto i = r.first;
        const Node* b = i->second;
        ++i;
        for (; i != r.second; ++i) {
            const Node* a = i->second;
            UnionAtributes(a, b, d);
            Union(a->first_node(), b->first_node(), d);
            b = a;
        }
        c->append_node(d);
    }
    else {
        auto r = map.find(k);
        c->append_node(doc->clone_node(r->second));
    }
}

void FusionXML::UnionAtributes(const Node *a, const Node *b, Node *c) {
    NodeAtributeMap unionMap;
    PopulateMapAttributes(b, unionMap);
    PopulateMapAttributes(a, unionMap);

    for (auto&& u : unionMap) {
        c->append_attribute(doc->allocate_attribute(u.second->name(), u.second->value()));
    }
}

void FusionXML::PopulateMapAttributes(const Node* a, NodeAtributeMap& unionMap) const
{
    for (Atribute* attr = a->first_attribute(); attr != nullptr; attr = attr->next_attribute()) {
        if (char* name = attr->name()) {
            unionMap.emplace(std::string(name), attr);
        }
    }
}

void FusionXML::PopulateMapNodes(const Node *node, NodeMultiMap &map) const {
    const char* filter = idFilter.c_str();
    for (const Node* n = node; n != nullptr; n = n->next_sibling()) {
        if (Atribute *attr = n->first_attribute(filter)) {
            Key k;
            k.name = n->name();
            k.id = attr->value();
            k.unify = true;
            map.emplace(k, n);
        }
        else {
            if (char* name = n->name()) {
                Key k;
                k.name = n->name();
                map.emplace(k, n);
            }
        }
    }
}



