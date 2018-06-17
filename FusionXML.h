#ifndef FUSION_XML_FUSIONXML_H
#define FUSION_XML_FUSIONXML_H

#include <string>
#include <unordered_set>
#include <unordered_map>

#include "rapidxml/rapidxml.hpp"

typedef wchar_t Char;
typedef std::wstring String;

class Key {
public:
  Key() = default;
  Key(const String& _name) : name(_name) {};
  Key(const String _name, const String& _id, bool _unify) : name(_name), id(_id), unify(_unify) {};
  ~Key() = default;
  bool operator<(const Key& k) const { return name < k.name || (name == k.name && id < k.id); };
  bool operator==(const Key& k) const { return name == k.name && id == k.id && unify == k.unify; };
  const String& Name() const { return name; };
  const String& ID() const { return id; };
  bool Unify() const { return unify; };

private:
  String name{ L"" };
  String id{ L"" };
  bool unify{ false };
};

namespace std {
  template <>
  struct hash<Key> : public unary_function<Key, size_t> {
    size_t operator()(const Key& value) const {
      return std::hash<String>{}(value.Name() + value.ID());
    }
  };

  template <>
  struct equal_to<Key> : public unary_function<Key, bool> {
    bool operator()(const Key& x, const Key& y) const {
      return x == y;
    }
  };
}

class FusionXML {
public:
  using Document = rapidxml::xml_document<Char>;
  using Node = rapidxml::xml_node<Char>;
  using Atribute = rapidxml::xml_attribute<Char>;
  using NodeAtributeMap = std::unordered_map<String, Atribute*>;
  using NodeMultiMap = std::unordered_multimap<Key, const Node*>;
  using KeySet = std::unordered_set<Key>;

  FusionXML();
  explicit FusionXML(const String& idFilter);
  virtual ~FusionXML();

  rapidxml::xml_document<Char>* Union(const Document &a, const Document &b);

  void Union(const Node *a, const Node *b, Node *c);
  void UnionAtributes(const Node *a, Node *b);

private:
  void Append(const Key &k, NodeMultiMap& map, Node *c);
  void UnifyAppend(const Key &k, NodeMultiMap& map, Node *c);
  void EnumerateAtributes(const Node* a, NodeAtributeMap& unionMap) const;
  void EnumerateNodes(const Node *node, KeySet& keys, NodeMultiMap &map) const;

  String idFilter{ L"id" };
  Document* doc = nullptr;
};


#endif
