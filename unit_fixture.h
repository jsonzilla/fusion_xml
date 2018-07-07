#ifndef  UNION_FIXTURE_H
#define  UNION_FIXTURE_H

#include <unordered_map>
#include <unordered_set>

#pragma warning(push, 0)  
#include "rapidxml.hpp"
#pragma warning(pop)  

#include "Key.h"
#include "FusionXML.h"
#include "FusionXML.cpp"
#include "gtest/gtest.h"

//--------------------------------------------------------------------------------------------------------------------------

class UnitFixture : public ::testing::Test
{
  public:
    using Document = rapidxml::xml_document<char>;
    using Node = rapidxml::xml_node<char>;
    using Atribute = rapidxml::xml_attribute<char>;
    using NodeAtributeMap = std::unordered_map<std::string, Atribute*>;
    using NodeMultiMap = std::unordered_multimap<Key<std::string, char>, const Node*>;
    using KeySet = std::unordered_set<Key<std::string, char>>;

    UnitFixture() = default;
    virtual ~UnitFixture() = default;

    virtual void SetUp();
    virtual void TearDown() {}

    static bool EqualDocument(
      Document& a,
      Document& b
    );

    static bool EqualNodeChilds(
      Node* a,
      Node* b
    );

    static bool EqualNode(
      Node* a,
      Node* b
    );

    static bool EqualAttibutes(
      Node* a,
      Node* b
    );

  protected:
    void InitMyOriginTestData();
    void InitMyMergeTestData();
    void InitMyResultTestData();

    rapidxml::xml_document<char> origin;
    rapidxml::xml_document<char> merge;
    rapidxml::xml_document<char> result;
    FusionXML<std::string, char> fusion{ "id" };
};


//--------------------------------------------------------------------------------------------------------------------------

#endif