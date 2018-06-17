#include "FusionXML.h"

using namespace rapidxml;

FusionXML::FusionXML() {
}

FusionXML::FusionXML(const String &idFilter) : idFilter(idFilter) {
}

FusionXML::~FusionXML() {
}

rapidxml::xml_document<Char>* FusionXML::Union(const Document& a, const Document& b) {
  delete doc;
  doc = new Document();
  const Node *aNode = a.first_node();
  const Node *bNode = b.first_node(); //deve encontrar primeiramente uma raiz em comum
  auto root = doc->allocate_node(node_element, aNode->name());
  UnionAtributes(aNode, root);
  UnionAtributes(bNode, root);
  Union(aNode->first_node(), bNode->first_node(), root);
  doc->append_node(root);

  // clean empty names
  //Node* aa = doc->first_node();
  //while (aa != nullptr) {
  //  if (wcscmp(aa->name(), L"") == 0) {
  //    doc->remove_node(aa);
  //  }
  //  aa = aa->next_sibling();
  //}

  return doc;
}

void FusionXML::Union(const Node *a, const Node *b, Node *c) {
  NodeMultiMap map;
  KeySet keys;
  EnumerateNodes(a, keys, map);
  EnumerateNodes(b, keys, map);

  for (auto&& k : keys) {
    if (k.Unify()) {
      UnifyAppend(k, map, c);
    }
    else {
      Append(k, map, c);
    }
  }
}

void FusionXML::Append(const Key &k, NodeMultiMap& map, Node *c) {
  auto listNodes = map.equal_range(k);
  for (auto it = listNodes.first; it != listNodes.second; ++it) {
    if (auto node = it->second) {
      c->append_node(doc->clone_node(node));
    }
  }
}

void FusionXML::UnifyAppend(const Key &k, NodeMultiMap& map, Node *c) {
  if (map.count(k) > 1) {
    auto accumulator = doc->allocate_node(node_element, doc->allocate_string(k.Name().c_str()));
    auto listNodes = map.equal_range(k);
    for (auto it = listNodes.first; it != listNodes.second; ++it) {
      if (auto node = it->second) {
        UnionAtributes(node, accumulator);
        Union(node->first_node(), accumulator->first_node(), accumulator);
      }
    }
    c->append_node(accumulator);
  }
  else {
    Append(k, map, c);
  }
}

void FusionXML::UnionAtributes(const Node *a, Node *b) {
  NodeAtributeMap map;
  EnumerateAtributes(a, map);
  
  for (auto&& p : map) {
    auto atr = p.second;
    if (auto has = b->first_attribute(atr->name())) {
      b->remove_attribute(has);
    }
    b->append_attribute(doc->allocate_attribute(atr->name(), atr->value()));
  }
}

void FusionXML::EnumerateAtributes(const Node* node, NodeAtributeMap& unionMap) const {
  if (node != nullptr) {
    for (Atribute* attr = node->first_attribute(); attr != nullptr; attr = attr->next_attribute()) {
      if (Char* name = attr->name()) {
        unionMap.emplace(std::wstring(name), attr);
      }
    }
  }
}

void FusionXML::EnumerateNodes(const Node *_node, KeySet& keys, NodeMultiMap &map) const {
  if (_node != nullptr) {
    const Char* filter = idFilter.c_str();
    for (const Node* node = _node; node != nullptr; node = node->next_sibling()) {
      if (Atribute *attr = node->first_attribute(filter)) {
        Key k(node->name(), attr->value(), true);
        keys.emplace(k);
        map.emplace(k, node);
      }
      else {
        if (Char* name = node->name()) {
          Key k(node->name());
          keys.emplace(k);
          map.emplace(k, node);
        }
      }
    }
  }
}
