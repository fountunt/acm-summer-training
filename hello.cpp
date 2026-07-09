#include <iostream>
#include <vector>
#include <string>

int main() {
    std::cout << "=== VS Code C++ 环境配置成功！ ===\n" << std::endl;

    std::vector<std::string> tools = {
        "编译器: GCC 14.2.0",
        "调试器: GDB 15.2",
        "构建工具: Make 4.4.1",
        "编辑器: VS Code + C/C++ Extension"
    };

    std::cout << "已安装工具:" << std::endl;
    for (const auto& tool : tools) {
        std::cout << "  ✓ " << tool << std::endl;
    }

    std::cout << "\n快捷键说明:" << std::endl;
    std::cout << "  Ctrl+Shift+B  — 编译当前文件" << std::endl;
    std::cout << "  F5           — 调试当前文件" << std::endl;
    std::cout << "  Ctrl+F5      — 运行（不调试）" << std::endl;

    return 0;
}
