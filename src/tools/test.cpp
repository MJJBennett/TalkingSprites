#include <string>
#include "string.hpp"
#include <iostream>

#define TEST_EQ(x,y) test_eq(__func__, __LINE__, x, y)
#define TRACK(x) track(__func__, __LINE__, x)

template<typename F, typename L, typename T, typename U>
void test_eq(const F& f, const L& l, const T& t, const U& u)
{
    if (t != u) std::cout << f << ":" << l << " Found inequality: " << t << " != " << u << std::endl;
}

template<typename F, typename L, typename T>
void track(const F& f, const L& l, const T& t)
{
    std::cout << f << ":" << l << " [" << t << "] reached." << std::endl;
}


void test_chop()
{
    using namespace ts::tools;
    const std::string empty = "";
    const std::string zero_splits = "This_is_some_text.";
    const std::string many_splits = "This is some text = other text.";
    const auto chopped_empty = chop_first<0>(empty);
    TEST_EQ(chopped_empty, "");
    const auto chopped_zero = chop_first<5>(zero_splits);
    TEST_EQ(chopped_zero, "");
}

int main()
{
    test_chop();
}
