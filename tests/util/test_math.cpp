
#include <gtest/gtest.h>
#include "util/math.hpp"
using namespace fb;

TEST(util_misc, abs)
{
    ASSERT_EQ(fb::abs(+2)  , 2);
    ASSERT_EQ(fb::abs(-2)  , 2);
    ASSERT_FLOAT_EQ(fb::abs(+2.0), 2.0);
    ASSERT_FLOAT_EQ(fb::abs(-2.0), 2.0);
}

TEST(util_misc, min)
{
    ASSERT_EQ(fb::min(2,3)  , 2);
    ASSERT_EQ(fb::min(2,-3)  , -3);
    ASSERT_FLOAT_EQ(fb::min(2.0,3.0), 2.0);
    ASSERT_FLOAT_EQ(fb::min(2.0,-3.0), -3.0);
}

TEST(util_misc, max)
{
    ASSERT_EQ(fb::max(2,3), 3);
    ASSERT_EQ(fb::max(2,-3), 2);
    ASSERT_FLOAT_EQ(fb::max(2.0,3.0), 3.0);
    ASSERT_FLOAT_EQ(fb::max(2.0,-3.0), 2.0);
}

TEST(util_misc, intPow)
{
    ASSERT_DOUBLE_EQ(fb::intPow<3>(2.0), 8.0);
    ASSERT_DOUBLE_EQ(fb::intPow<1>(2.0), 2.0);
    ASSERT_DOUBLE_EQ(fb::intPow<0>(2.0), 1.0);
}

TEST(util_misc, areFloatsEqual)
{
    ASSERT_TRUE ( areFloatsEqual(2.0, 2.0)        );
    ASSERT_FALSE( areFloatsEqual(2.0,-2.0)        );
    ASSERT_TRUE ( areFloatsEqual(2.0, 2.0 + 1e-16));
    ASSERT_FALSE( areFloatsEqual(0.0, 2.0)        );
    ASSERT_TRUE ( areFloatsEqual(0.0, 0.0)        );
    ASSERT_FALSE( areFloatsEqual(0.0, 1e100)      );
}

