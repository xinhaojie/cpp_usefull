/****************************************************************************************************
* @Author       : xinhaojie xinhaojie@qq.com
* @Date         : 2025-03-19 11:08:01
* @LastEditors  : xinhaojie xinhaojie@qq.com
* @LastEditTime : 2025-03-19 11:31:57
* @FilePath     : /cpp_usefull/unit_test/test.cpp
* @Description  :
* @Copyright (c) 2025 by xinhaojie@qq.com, All Rights Reserved.
****************************************************************************************************/
#include "tiny_unit.hxx"

// 示例测试用例
TINY_UNIT_TEST(ExampleTestSuite, SubtractionTest) {
    int a = 5;
    int b = 3;
    int result = a - b;
    TINY_UNIT_ASSERT_EQ(3, result);
}

int main() {
    tiny_unit::TestRegistry::get_instance().run_all_tests();
    return 0;
}