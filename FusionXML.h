#ifndef FUSION_XML_FUSIONXML_H
#define FUSION_XML_FUSIONXML_H

#include <string>
#include <unordered_set>
#include <unordered_map>

#pragma warning(push, 0)  
#include "rapidxml.hpp"
#pragma warning(pop)  

#include "Key.h"

template <typename S, typename C>
class FusionXML {
public:
  using Document = rapidxml::xml_document<C>;

  FusionXML() = delete;
  explicit FusionXML(const S& idFilter) : idFilter(idFilter) {};
  virtual ~FusionXML() = default;

  void Union(const Document &a, const Document &b, Document &c);

private:
  using Atribute = rapidxml::xml_attribute<C>;
  using NodeAtributeMap = std::unordered_map<S, Atribute*>;
  using KeyType = Key<S, C>;
  using Node = rapidxml::xml_node<C>;
  using NodeMultiMap = std::unordered_multimap<Key<S, C>, const Node*>;
  using KeySet = std::unordered_set<Key<S, C>>;

  void Append(const KeyType &k, NodeMultiMap& map, Node *c);
  void UnifyAppend(const KeyType &k, NodeMultiMap& map, Node *c);
  void EnumerateAtributes(const Node* a, NodeAtributeMap& unionMap) const;
  void EnumerateNodes(const Node *node, KeySet& keys, NodeMultiMap &map) const;
  void Union(const Node *a, const Node *b, Node *c);
  void UnionAtributes(const Node *a, Node *b);
  void UnionValues(const Node *a, Node *b);

  S idFilter;
  Document* doc = nullptr;
};


#endif
