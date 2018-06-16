#ifndef FUSION_XML_FUSIONXML_H
#define FUSION_XML_FUSIONXML_H

#include <string>
#include <unordered_set>
#include <unordered_map>

#include "rapidxml/rapidxml.hpp"

class Key {
public:
    Key() = default;
    Key(const std::string& _name) : name(_name) {};
    Key(const std::string& _name, const std::string& _id, bool _unify) : name(_name), id(_id), unify(_unify) {};
    ~Key() = default;
    bool operator<(const Key& k) const { return name < k.name || (name == k.name && id < k.id); };
    bool operator==(const Key& k) const { return name == k.name && id == k.id && unify == k.unify; };
    const std::string& Name() const { return name;  };
    const std::string& ID() const { return id; };
    bool Unify() const { return unify; };

private:
    std::string name { "" };
    std::string id { "" };
    bool unify { false };
};

namespace std {
    template <>
    struct hash<Key> : public unary_function<Key, size_t> {
        size_t operator()(const Key& value) const {
            return std::hash<std::string>{}(value.Name() + value.ID());
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
    using Document = rapidxml::xml_document<char>;
    using Node = rapidxml::xml_node<char>;
    using Atribute = rapidxml::xml_attribute<char>;
    using NodeAtributeMap = std::unordered_map<std::string, Atribute*>;
    using NodeMultiMap = std::unordered_multimap<Key, const Node*>;
    using KeySet = std::unordered_set<Key>;

    FusionXML();
    explicit FusionXML(const std::string& idFilter);
    virtual ~FusionXML();

    rapidxml::xml_document<char>* Union(const Document &a, const Document &b);

    void Union(const Node *a, const Node *b, Node *c);
    void UnionAtributes(const Node *a, const Node *b, Node *c);

private:
    void Unify(const Key &k, NodeMultiMap& map, Node *c);
    void PopulateMapAttributes(const Node* a, NodeAtributeMap& unionMap) const;
    void EnumerateNodes(const Node *node, KeySet& keys, NodeMultiMap &map) const;

    std::string idFilter { "id" };
    Document* doc = nullptr;
};


#endif
