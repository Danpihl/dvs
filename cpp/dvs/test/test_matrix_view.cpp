#include <gtest/gtest.h>

#include <cmath>
#include <functional>
#include <iostream>
#include <string>
#include <vector>

#include "dvs.h"

using namespace dvs;

using Mat = Matrix<int>;

class TestMatrixView : public testing::Test
{
protected:

    void SetUp() override {}

    void TearDown() override {}
};

TEST_F(TestMatrixView, TestBasic)
{
    Mat m0 = {{11, 12, 13, 14, 15, 16},
              {21, 22, 23, 24, 25, 26},
              {31, 32, 33, 34, 35, 36},
              {41, 42, 43, 44, 45, 46},
              {51, 52, 53, 54, 55, 56},
              {61, 62, 63, 64, 65, 66},
              {71, 72, 73, 74, 75, 76}};

    Mat m4 = {{111, 112, 113, 114, 115, 116},
              {121, 122, 123, 124, 125, 126},
              {131, 132, 133, 134, 135, 136},
              {141, 142, 143, 144, 145, 146},
              {151, 152, 153, 154, 155, 156},
              {161, 162, 163, 164, 165, 166},
              {171, 172, 173, 174, 175, 176}};

    Mat m3 = m0({0, 3}, {1, 4});
    
}

TEST_F(TestMatrixView, TestBasic2)
{
    Mat m0 = {{11, 12, 13, 14, 15, 16},
              {21, 22, 23, 24, 25, 26},
              {31, 32, 33, 34, 35, 36},
              {41, 42, 43, 44, 45, 46},
              {51, 52, 53, 54, 55, 56},
              {61, 62, 63, 64, 65, 66},
              {71, 72, 73, 74, 75, 76}};

    Mat m4 = {{111, 112, 113, 114, 115, 116},
              {121, 122, 123, 124, 125, 126},
              {131, 132, 133, 134, 135, 136},
              {141, 142, 143, 144, 145, 146},
              {151, 152, 153, 154, 155, 156},
              {161, 162, 163, 164, 165, 166},
              {171, 172, 173, 174, 175, 176}};

    // MatrixView<int> m2 = m0({0, 3}, {1, 4});
    // Mat m3 = m0({0, 3}, {1, 4});
    std::cout << m4 << std::endl;
    m4({1, 4}, {1, 4}) = m0({2, 5}, {3, 6});
    std::cout << m4 << std::endl;
}
