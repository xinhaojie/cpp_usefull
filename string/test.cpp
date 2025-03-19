#include "fastring.h"
#include <iostream>
#include <string>

void testConstructors() {
    // 默认构造函数
    tiny_string::fastring s1;
    std::cout << "Default constructor: " << (s1.empty() ? "Empty" : "Not empty") << std::endl;

    // 指定初始容量的构造函数
    tiny_string::fastring s2(10);
    std::cout << "Capacity constructor: Capacity = " << s2.capacity() << std::endl;

    // 用给定的字节序列创建 fastring 对象
    const char data[] = "Hello";
    tiny_string::fastring s3(data, 5);
    std::cout << "Byte sequence constructor: " << s3.c_str() << std::endl;

    // 用 C 风格的字符串创建 fastring 对象
    tiny_string::fastring s4("World");
    std::cout << "C-style string constructor: " << s4.c_str() << std::endl;

    // 用 std::string 创建一个 fastring 对象
    std::string stdStr = "Fast";
    tiny_string::fastring s5(stdStr);
    std::cout << "std::string constructor: " << s5.c_str() << std::endl;

    // 将 fastring 对象初始化为 n 个字符 c 构成的字符串
    tiny_string::fastring s6(3, '!');
    std::cout << "Character repeat constructor: " << s6.c_str() << std::endl;

    // 拷贝构造函数
    tiny_string::fastring s7(s4);
    std::cout << "Copy constructor: " << s7.c_str() << std::endl;

    // 移动构造函数
    tiny_string::fastring s8(std::move(s7));
    std::cout << "Move constructor: " << s8.c_str() << std::endl;
}

void testAssignmentOperators() {
    tiny_string::fastring s1;
    s1 = "Test";
    std::cout << "Assignment from C-style string: " << s1.c_str() << std::endl;

    std::string stdStr = "Assignment";
    s1 = stdStr;
    std::cout << "Assignment from std::string: " << s1.c_str() << std::endl;

    tiny_string::fastring s2("Another");
    s1 = s2;
    std::cout << "Assignment from fastring: " << s1.c_str() << std::endl;

    tiny_string::fastring s3("Move");
    s1 = std::move(s3);
    std::cout << "Move assignment: " << s1.c_str() << std::endl;
}

void testAppendAndInsert() {
    tiny_string::fastring s("Append");
    s.append(" more");
    std::cout << "Append C-style string: " << s.c_str() << std::endl;

    std::string stdStr = " data";
    s.append(stdStr);
    std::cout << "Append std::string: " << s.c_str() << std::endl;

    tiny_string::fastring s2(" and");
    s.append(s2);
    std::cout << "Append fastring: " << s.c_str() << std::endl;

    s << 123;
    std::cout << "Insert integer: " << s.c_str() << std::endl;
}

void testComparison() {
    tiny_string::fastring s1("Same");
    tiny_string::fastring s2("Same");
    tiny_string::fastring s3("Different");

    std::cout << "Compare equal fastrings: " << (s1.compare(s2) == 0 ? "Equal" : "Not equal") << std::endl;
    std::cout << "Compare different fastrings: " << (s1.compare(s3) == 0 ? "Equal" : "Not equal") << std::endl;
}

int main() {
    testConstructors();
    testAssignmentOperators();
    testAppendAndInsert();
    testComparison();

    return 0;
}