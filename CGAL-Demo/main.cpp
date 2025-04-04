#include "LAR_STL.h"
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "用法: " << argv[0] << " <输入 STL 文件路径> <输出 STL 文件路径>" << std::endl;
        return 1;
    }

    std::string input_filename = argv[1];
    std::string output_filename = argv[2];

    // 创建 LAR_STL 对象并加载和修复文件
    LAR_STL stl_processor(input_filename);

    // 检查文件是否成功加载和修复
    if (!stl_processor.get_repaired_mesh().is_empty()) {
        std::cout << "文件加载和修复成功。" << std::endl;

        // 验证流形性
        if (stl_processor.is_manifold()) {
            std::cout << "修复后的网格是流形的。" << std::endl;
        } else {
            std::cout << "修复后的网格不是流形的。" << std::endl;
        }

        // 保存修复后的网格
        if (stl_processor.save_repaired_mesh(output_filename)) {
            std::cout << "修复后的网格已保存到 " << output_filename << std::endl;
        } else {
            std::cerr << "保存修复后的网格时出错。" << std::endl;
        }
    } else {
        std::cerr << "文件加载和修复失败。" << std::endl;
    }

    return 0;
}    