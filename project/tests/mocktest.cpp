#include <iostream>
#include "gtest/gtest.h"

TEST(MyMethod, WorksProperly) {
  char arr1[] = "abcd";
  char arr2[] = "abcd";
  int sz = sizeof(arr1)/sizeof(arr1[0]) - 1; // We need to avoid the terminating \0.
  for (int i = 0; i < sz; ++i)
    EXPECT_EQ(arr1[i], arr2[i]);
}

