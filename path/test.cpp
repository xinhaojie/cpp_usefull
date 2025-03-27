#include "tiny_path.hpp"
#include <iostream>

using tiny_path::path;
int main() {
    // 测试 path::clean
    std::cout << "path::clean(\"\"): " << path::clean("") << std::endl;
    std::cout << "path::clean(\"./x//y/\"): " << path::clean("./x//y/") << std::endl;
    std::cout << "path::clean(\"./x/..\"): " << path::clean("./x/..") << std::endl;
    std::cout << "path::clean(\"./x/../..\"): " << path::clean("./x/../..") << std::endl;

    // 测试 path::join
    std::cout << "path::join(\"\", \"\"): " << path::join("", "") << std::endl;
    std::cout << "path::join(\"x\", \"y\", \"z\"): " << path::join("x", "y", "z") << std::endl;
    std::cout << "path::join(\"/x/\", \"y\"): " << path::join("/x/", "y") << std::endl;

    // 测试 path::split
    auto [dir1, file1] = path::split("/");
    std::cout << "path::split(\"/\"): {" << dir1 << ", " << file1 << "}" << std::endl;
    auto [dir2, file2] = path::split("/a");
    std::cout << "path::split(\"/a\"): {" << dir2 << ", " << file2 << "}" << std::endl;
    auto [dir3, file3] = path::split("/a/");
    std::cout << "path::split(\"/a/\"): {" << dir3 << ", " << file3 << "}" << std::endl;
    auto [dir4, file4] = path::split("/a/b");
    std::cout << "path::split(\"/a/b\"): {" << dir4 << ", " << file4 << "}" << std::endl;

    // 测试 path::dir
    std::cout << "path::dir(\"a\"): " << path::dir("a") << std::endl;
    std::cout << "path::dir(\"a/\"): " << path::dir("a/") << std::endl;
    std::cout << "path::dir(\"/\"): " << path::dir("/") << std::endl;
    std::cout << "path::dir(\"/a\"): " << path::dir("/a") << std::endl;
    std::cout << "path::dir(\"/a/\"): " << path::dir("/a/") << std::endl;

    // 测试 path::base
    std::cout << "path::base(\"\"): " << path::base("") << std::endl;
    std::cout << "path::base(\"/\"): " << path::base("/") << std::endl;
    std::cout << "path::base(\"/a/\"): " << path::base("/a/") << std::endl;
    std::cout << "path::base(\"/a\"): " << path::base("/a") << std::endl;
    std::cout << "path::base(\"/a/b\"): " << path::base("/a/b") << std::endl;

    // 测试 path::ext
    std::cout << "path::ext(\"/a.c\"): " << path::ext("/a.c") << std::endl;
    std::cout << "path::ext(\"a/b\"): " << path::ext("a/b") << std::endl;
    std::cout << "path::ext(\"/a.c/\"): " << path::ext("/a.c/") << std::endl;
    std::cout << "path::ext(\"a.\"): " << path::ext("a.") << std::endl;

    return 0;
}
