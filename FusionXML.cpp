#ifndef FUSION_XML_FUSIONXML_CPP
#define FUSION_XML_FUSIONXML_CPP

#include "FusionXML.h"

using namespace rapidxml;

template <typename S, typename C>
void FusionXML<S, C>::Union(const Document& a, const Document& b, Document& c) {
  doc = nullptr;
  doc = &c;
  const Node *aNode = a.first_node();
  const Node *bNode = b.first_node(); 
  auto root = doc->allocate_node(node_element, aNode->name());
  UnionAtributes(aNode, root);
  UnionAtributes(bNode, root);
  Union(aNode->first_node(), bNode->first_node(), root);
  c.append_node(root);
}

template <typename S, typename C>
void FusionXML<S, C>::Union(const Node *a, const Node *b, Node *c) {
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

template <typename S, typename C>
void FusionXML<S, C>::Append(const KeyType &k, NodeMultiMap& map, Node *c) {
  auto listNodes = map.equal_range(k);
  for (auto it = listNodes.first; it != listNodes.second; ++it) {
    if (auto node = it->second) {
      c->append_node(doc->clone_node(node));
    }
  }
}

template <typename S, typename C>
void FusionXML<S, C>::UnifyAppend(const KeyType &k, NodeMultiMap& map, Node *c) {
  if (map.count(k) > 1) {
    auto accumulator = doc->allocate_node(node_element, doc->allocate_string(k.Name().c_str()));
    auto listNodes = map.equal_range(k);
    for (auto it = listNodes.first; it != listNodes.second; ++it) {
      if (auto node = it->second) {
        UnionAtributes(node, accumulator);
        UnionValues(node, accumulator);
        Union(node->first_node(), accumulator->first_node(), accumulator);
      }
    }
    c->append_node(accumulator);
  }
  else {
    Append(k, map, c);
  }
}

template <typename S, typename C>
void FusionXML<S, C>::UnionValues(const Node *a, Node *b) {
    if (a->value_size() > 0) {
      b->value(doc->allocate_string(a->value()));
    }
}

template <typename S, typename C>
void FusionXML<S, C>::UnionAtributes(const Node *a, Node *b) {
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

template <typename S, typename C>
void FusionXML<S, C>::EnumerateAtributes(const Node* node, NodeAtributeMap& unionMap) const {
  if (node != nullptr) {
    for (Atribute* attr = node->first_attribute(); attr != nullptr; attr = attr->next_attribute()) {
      if (C* name = attr->name()) {
        unionMap.emplace(S(name), attr);
      }
    }
  }
}

template <typename S, typename C>
void FusionXML<S, C>::EnumerateNodes(const Node *_node, KeySet& keys, NodeMultiMap &map) const {
  if (_node != nullptr) {
    const C* filter = idFilter.c_str();
    for (const Node* node = _node; node != nullptr; node = node->next_sibling()) {
      if (Atribute *attr = node->first_attribute(filter)) {
        KeyType k(node->name(), attr->value(), true);
        keys.emplace(k);
        map.emplace(k, node);
      }
      else {
        if (C* name = node->name()) {
          KeyType k(node->name());
          keys.emplace(k);
          map.emplace(k, node);
        }
      }
    }
  }
}

#endif