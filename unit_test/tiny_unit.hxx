#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <functional>
#include <stdexcept>

namespace tiny_unit {

// 测试基类
class Test {
public:
    virtual void setup() {}
    virtual void tear_down() {}
    virtual void test_body() = 0;
    virtual ~Test() = default;

    void run();
    virtual std::string get_test_name() const = 0;
};

// 测试信息类
class TestInfo {
public:
    TestInfo(const std::string& test_case_name, const std::string& test_name, std::function<Test*()> factory);
    Test* create_test() const;
    std::string get_full_name() const;

private:
    std::string test_case_name_;
    std::string test_name_;
    std::function<Test*()> factory_;
};

// 测试注册表类
class TestRegistry {
public:
    static TestRegistry& get_instance();
    void register_test(TestInfo* test_info);
    void run_all_tests();

private:
    std::vector<TestInfo*> tests_;
    TestRegistry() = default;
    ~TestRegistry() = default;
    TestRegistry(const TestRegistry&) = delete;
    TestRegistry& operator=(const TestRegistry&) = delete;
};

// 定义测试用例的宏
#define TINY_UNIT_TEST(test_case_name, test_name) \
    class test_case_name##_##test_name : public tiny_unit::Test { \
    public: \
        test_case_name##_##test_name() = default; \
        ~test_case_name##_##test_name() override = default; \
        void test_body() override; \
        std::string get_test_name() const override { return #test_case_name "." #test_name; } \
    }; \
    static tiny_unit::TestInfo test_case_name##_##test_name##_info(#test_case_name, #test_name, []() { return new test_case_name##_##test_name(); }); \
    void test_case_name##_##test_name::test_body()

// 断言宏
#define TINY_UNIT_ASSERT_EQ(expected, actual) \
    if ((expected) != (actual)) { \
        throw std::runtime_error(std::string("Assertion failed: ") + #expected + " == " + #actual \
                                + ", expected: " + std::to_string(expected) + ", actual: " + std::to_string(actual)); \
    }

} // namespace tiny_unit
