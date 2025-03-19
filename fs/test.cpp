#include "tiny_fs.hxx"
#include <iostream>


void search_and_rename_hxx(const std::string& dir_path) {
    auto dir_items = tiny_fs::dir(dir_path).all();
    for (const auto& item : dir_items) {
        if (tiny_fs::is_dir(dir_path + "/" + item)) {
            search_and_rename_hxx(dir_path + "/" + item);
        } else if (tiny_fs::is_file(dir_path + "/" + item)) {
            std::string file_path = dir_path + "/" + item;
            std::string file_name = tiny_fs::file_name(file_path);
            std::string file_ext = tiny_fs::file_ext(file_path);
            if (file_ext == ".hxx") {

            }
        }
    }
}

int main() {
    // 测试 tiny_fs::dir
    tiny_fs::dir d(".");
    auto all_items = d.all();
    std::cout << "Directory items:" << std::endl;
    for (const auto& item : all_items) {
        std::cout << item << std::endl;

    }

    return 0;
}