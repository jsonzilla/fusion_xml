#ifndef FUSION_XML_FUSIONXML_H
#define FUSION_XML_FUSIONXML_H

#include <string>
#include <map>
#include <unordered_map>

#include "rapidxml/rapidxml.hpp"

struct Key {
    std::string name { "" };
    std::string id { "" };
    bool unify { false };

    bool operator<(const Key& k) const { return name < k.name && id < k.id; };
};

namespace std
{
    template <>
    struct hash<Key> : public unary_function<Key, size_t> {
        size_t operator()(const Key& value) const {
            return std::hash<std::string>{}(value.name + value.id);
        }
    };

    template <>
    struct equal_to<Key> : public unary_function<Key, bool> {
        bool operator()(const Key& x, const Key& y) const {
            return false;
        }
    };
}

class FusionXML {
public:
    using Document = rapidxml::xml_document<char>;
    using Node = rapidxml::xml_node<char>;
    using Atribute = rapidxml::xml_attribute<char>;
    using NodeAtributeMap = std::map<std::string, Atribute*>;
    using NodeMap = std::map<std::string, const Node*>;
    using NodeMultiMap = std::unordered_multimap<Key, const Node*>;

    FusionXML();

    explicit FusionXML(const std::string& idFilter);

    virtual ~FusionXML();

    rapidxml::xml_document<char>* Union(const Document &a, const Document &b);

    void Union(const Node *a, const Node *b, Node *c);

    void UnionAtributes(const Node *a, const Node *b, Node *c);

private:
    void Unify(const Key &k, NodeMultiMap map, Node *c);

    void PopulateMapAttributes(const Node* a, NodeAtributeMap& unionMap) const;

    void PopulateMapNodes(const Node *node, NodeMultiMap &map) const;

    std::string idFilter { "id" };
    Document* doc = nullptr;
};


#endif
