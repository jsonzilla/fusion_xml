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
  const Node *aNode = a.first_node();
  const Node *bNode = b.first_node(aNode->name()); //deve encontrar primeiramente uma raiz em comum
  auto root = doc->allocate_node(node_element, aNode->name());
  UnionAtributes(aNode, bNode, root);
  Union(aNode->first_node(), bNode->first_node(), root);
  doc->append_node(root);

  // clean empty names
  //Node* aa = doc->first_node();
  //while (aa != nullptr) {
  //  if (strcmp(aa->name(), "") == 0) {
  //    doc->remove_node(aa);
  //  }
  //  aa = aa->next_sibling();
  //}

  return doc;
}

void FusionXML::Union(const Node *a, const Node *b, Node *c) {
  NodeMultiMap unionMap;
  KeySet keys;
  EnumerateNodes(a, keys, unionMap);
  EnumerateNodes(b, keys, unionMap);
  
  for (auto&& k : keys) {
    if (k.Unify()) {
      Unify(k, unionMap, c);
    }
    else {
      auto p = unionMap.find(k);
      c->append_node(doc->clone_node(p->second));
    }
  }
}

void FusionXML::Unify(const Key &k, NodeMultiMap& map, Node *c) {
  if (map.count(k) > 1) {
    char* name = doc->allocate_string(k.Name().c_str());
    auto accumulator = doc->allocate_node(node_element, name);
    auto ret = map.equal_range(k);
    for (auto it = ret.first; it != ret.second; ++it) {
      if (auto node = it->second) {
        UnionAtributes(node, accumulator, accumulator);
        Union(node->first_node(), accumulator->first_node(), accumulator);
      }
    }
    c->append_node(accumulator);
  }
  else {
    auto r = map.find(k);
    if (r != map.end()) {
      c->append_node(doc->clone_node(r->second));
    }
  }
}

void FusionXML::UnionAtributes(const Node *a, const Node *b, Node *c) {
  NodeAtributeMap unionMap;
  PopulateMapAttributes(a, unionMap);
  PopulateMapAttributes(b, unionMap);

  for (auto&& u : unionMap) {
    c->append_attribute(doc->allocate_attribute(u.second->name(), u.second->value()));
  }
}

void FusionXML::PopulateMapAttributes(const Node* node, NodeAtributeMap& unionMap) const {
  if (node != nullptr) {
    for (Atribute* attr = node->first_attribute(); attr != nullptr; attr = attr->next_attribute()) {
      if (char* name = attr->name()) {
        unionMap.emplace(std::string(name), attr);
      }
    }
  }
}

void FusionXML::EnumerateNodes(const Node *_node, KeySet& keys, NodeMultiMap &map) const {
  if (_node != nullptr) {
    const char* filter = idFilter.c_str();
    for (const Node* node = _node; node != nullptr; node = node->next_sibling()) {
      if (Atribute *attr = node->first_attribute(filter)) {
        Key k(node->name(), attr->value(), true);
        keys.emplace(k);
        map.emplace(k, node);
      }
      else {
        if (char* name = node->name()) {
          Key k(node->name());
          keys.emplace(k);
          map.emplace(k, node);
        }
      }
    }
  }
}



