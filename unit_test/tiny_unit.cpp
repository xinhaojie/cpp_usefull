/****************************************************************************************************
* @Author       : xinhaojie xinhaojie@qq.com
* @Date         : 2025-03-19 11:07:27
* @LastEditors  : xinhaojie xinhaojie@qq.com
* @LastEditTime : 2025-03-27 14:11:03
* @FilePath     : /cpp_usefull/unit_test/tiny_unit.cpp
* @Description  :
* @Copyright (c) 2025 by xinhaojie@qq.com, All Rights Reserved.
****************************************************************************************************/
#include "tiny_unit.hpp"

namespace tiny_unit {

void Test::run() {
    try {
        setup();
        test_body();
        tear_down();
        std::cout << "[PASS] " << get_test_name() << std::endl;
    } catch (const std::exception& e) {
        std::cout << "[FAIL] " << get_test_name() << ": " << e.what() << std::endl;
    }
}

TestInfo::TestInfo(const std::string& test_case_name, const std::string& test_name, std::function<Test*()> factory)
    : test_case_name_(test_case_name), test_name_(test_name), factory_(factory) {
    TestRegistry::get_instance().register_test(this);
}

Test* TestInfo::create_test() const {
    return factory_();
}

std::string TestInfo::get_full_name() const {
    return test_case_name_ + "." + test_name_;
}

TestRegistry& TestRegistry::get_instance() {
    static TestRegistry instance;
    return instance;
}

void TestRegistry::register_test(TestInfo* test_info) {
    tests_.push_back(test_info);
}

void TestRegistry::run_all_tests() {
    for (TestInfo* test_info : tests_) {
        Test* test = test_info->create_test();
        test->run();
        delete test;
    }
}

} // namespace tiny_unit