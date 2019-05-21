
#include <gtest/gtest.h>
#include "util/struct.hpp"
using namespace fb;

TEST(util_misc, Point_equals)
{
    ASSERT_TRUE (Point(1.0,2.0,3.0) == Point(1.0,2.0,3.0));
    ASSERT_FALSE(Point(1.0,2.0,3.0) == Point(-1.0,2.0,3.0));

    ASSERT_FALSE(Point(1.0,2.0,3.0) != Point(1.0,2.0,3.0));
    ASSERT_TRUE (Point(1.0,2.0,3.0) != Point(-1.0,2.0,3.0));
}

TEST(util_misc, Distance_equals)
{
    ASSERT_TRUE (Distance(1.0,2.0,3.0) == Distance(1.0,2.0,3.0));
    ASSERT_FALSE(Distance(1.0,2.0,3.0) == Distance(-1.0,2.0,3.0));

    ASSERT_FALSE(Distance(1.0,2.0,3.0) != Distance(1.0,2.0,3.0));
    ASSERT_TRUE (Distance(1.0,2.0,3.0) != Distance(-1.0,2.0,3.0));
}

TEST(util_misc, Dictionary_operatorIndex)
{
    Dictionary<int> myDict = {{"abc", 2}, {"def", 3}, {"ghi", 5}};
    ASSERT_EQ( myDict["abc"], 2);
    ASSERT_EQ( myDict["def"], 3);
    ASSERT_EQ( myDict["ghi"], 5);
    ASSERT_DEATH(myDict["xyz"], "Key not found in dictionary");
}

