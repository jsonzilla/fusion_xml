#include "unit_fixture.h"

#include "gtest/gtest.h"

TEST_F(UnitFixture, unionDocuments)
{
  rapidxml::xml_document<char> destin;
  fusion.Union(origin, merge, destin);

  EXPECT_FALSE(UnitFixture::EqualDocument(destin, result));
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}