#include "img.hpp"
#include <iostream>

int main() {
    try {
        Img boardImg;
        boardImg.read(R"(C:\Users\User\Downloads\bootcamp\assets\board.png)");
        boardImg.show();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}