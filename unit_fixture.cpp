#include "unit_fixture.h"

#include "gtest/gtest.h"

#pragma warning(push, 0)  
#include "rapidxml_print.hpp"
#include "rapidxml_utils.hpp"
#pragma waring(pop)

using namespace rapidxml;

constexpr auto SHOWINCONSOLE = true;

//--------------------------------------------------------------------------------------------------------------------------

void UnitFixture::SetUp()
{
  InitMyOriginTestData();
  InitMyMergeTestData();
  InitMyResultTestData();
}

//--------------------------------------------------------------------------------------------------------------------------

void UnitFixture::InitMyOriginTestData()
{
  auto root = origin.allocate_node(node_element, "Root");
  auto Id = origin.allocate_attribute("Id", "0101");
  root->append_attribute(Id);

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
  auto fa = origin.allocate_node(node_element, "FA");
  fa->append_attribute(origin.allocate_attribute("attr", "at"));
  fa->append_attribute(origin.allocate_attribute("otherAttr", "bt"));
  f->append_node(fa);
  f->append_node(origin.allocate_node(node_element, "FB"));
  root->append_node(f);

  auto g = result.allocate_node(node_element, "G");
  root->append_node(g);
  origin.append_node(root);
}

//--------------------------------------------------------------------------------------------------------------------------

void UnitFixture::InitMyMergeTestData()
{
  auto root = merge.allocate_node(node_element, "Root");
  auto Id = merge.allocate_attribute("id", "0101");
  root->append_attribute(Id);
  merge.append_node(root);

  auto a = merge.allocate_node(node_element, "A");
  root->append_node(a);

  auto b = merge.allocate_node(node_element, "B");
  b->append_attribute(merge.allocate_attribute("attr", "at"));
  root->append_node(b);

  auto c = merge.allocate_node(node_element, "C");
  root->append_node(c);

  auto e = merge.allocate_node(node_element, "E");
  root->append_node(e);

  auto f = merge.allocate_node(node_element, "F");
  f->append_node(merge.allocate_node(node_element, "FA"));
  auto fb = merge.allocate_node(node_element, "FB");
  fb->append_node(merge.allocate_node(node_element, "FBA"));
  f->append_node(fb);
  root->append_node(f);
}

//--------------------------------------------------------------------------------------------------------------------------

void UnitFixture::InitMyResultTestData()
{
  auto root = result.allocate_node(node_element, "Root");
  auto Id = result.allocate_attribute("id", "0101");
  root->append_attribute(Id);
  result.append_node(root);

  auto a = result.allocate_node(node_element, "A");
  root->append_node(a);

  auto b = result.allocate_node(node_element, "B");
  b->append_attribute(result.allocate_attribute("attr", "at"));
  root->append_node(b);

  auto c = result.allocate_node(node_element, "C");
  c->append_attribute(origin.allocate_attribute("attr", "at"));
  c->append_node(result.allocate_node(node_element, "CA"));
  c->append_node(result.allocate_node(node_element, "CB"));
  root->append_node(c);

  auto d = origin.allocate_node(node_element, "D");
  d->append_node(origin.allocate_node(node_element, "DA"));
  d->append_node(origin.allocate_node(node_element, "DB"));
  d->append_node(origin.allocate_node(node_element, "DC"));
  root->append_node(d);

  auto e = result.allocate_node(node_element, "E");
  root->append_node(e);

  auto f = result.allocate_node(node_element, "F");
  auto fa = origin.allocate_node(node_element, "FA");
  fa->append_attribute(origin.allocate_attribute("attr", "at"));
  fa->append_attribute(origin.allocate_attribute("otherAttr", "bt"));
  f->append_node(fa);
  auto fb = result.allocate_node(node_element, "FB");
  fb->append_node(result.allocate_node(node_element, "FBA"));
  f->append_node(fb);
  root->append_node(f);

  auto g = result.allocate_node(node_element, "G");
  root->append_node(g);
}

//--------------------------------------------------------------------------------------------------------------------------

bool UnitFixture::EqualDocument(
  Document& a,
  Document& b
)
{
  return EqualNodeChilds(a.first_node(), b.first_node());
}

//--------------------------------------------------------------------------------------------------------------------------

bool UnitFixture::EqualNodeChilds(
  Node* a,
  Node* b
)
{
  bool equal = true;
  Node* aNode = a->first_node();
  Node* bNode = b->first_node();

  while (equal && aNode != nullptr && bNode != nullptr) {
    equal = EqualNode(aNode, bNode);
    aNode = aNode->next_sibling();
    bNode = bNode->next_sibling();
  }

  return equal;
}

//--------------------------------------------------------------------------------------------------------------------------

bool UnitFixture::EqualNode(
  Node* a,
  Node* b
)
{
  bool equal { true };
  const bool aNulo { a == nullptr };
  const bool bNulo { b == nullptr };
  if (aNulo ^ bNulo) {
    return false;
  }

  equal = (aNulo && bNulo) || (
            strcmp(a->name(), b->name()) == 0 &&
            strcmp(a->value(), b->value()) == 0 &&
            EqualAttibutes(a, b) &&
            EqualNodeChilds(a, b));

  if (SHOWINCONSOLE && !equal) {
    printf("\nfound diference in node!\n");
    if (!EqualAttibutes(a, b)) {
      printf("not same attributes!\n");
    }
    if (!EqualNodeChilds(a, b)) {
      printf("not same childs!\n");
    }
    if (!aNulo && !bNulo && strcmp(a->name(), b->name()) != 0) {
      printf("not same name!\n%s != %s\n", a->name(), b->name());
    }
    if (!aNulo && !bNulo && strcmp(a->value(), b->value()) != 0) {
      printf("not same value!\n%s != %s\n", a->value(), b->value());
    }
  }

  return equal;
}

//--------------------------------------------------------------------------------------------------------------------------

bool UnitFixture::EqualAttibutes(
  Node* a,
  Node* b
)
{
  bool equal { true };
  Atribute* aAttr = a->first_attribute();
  Atribute* bAttr = b->first_attribute();

  while (equal && aAttr != nullptr && bAttr != nullptr) {
    equal = (strcmp(aAttr->name(), bAttr->name()) == 0 && strcmp(aAttr->value(), bAttr->value()) == 0);
    aAttr = aAttr->next_attribute();
    bAttr = bAttr->next_attribute();
  }

  if (SHOWINCONSOLE && !equal) {
    printf("\nfound diference in attribute!\n");
    if (aAttr != nullptr && bAttr != nullptr && strcmp(a->name(), b->name()) != 0) {
      printf("not same name!\n%s != %s\n", a->name(), b->name());
    }
    if (aAttr != nullptr && bAttr != nullptr && strcmp(a->value(), b->value()) != 0) {
      printf("not same value!\n%s != %s\n", a->value(), b->value());
    }
  }

  return equal;
}


//--------------------------------------------------------------------------------------------------------------------------
