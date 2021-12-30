#include "java.h"

int main() 
{
    std::string fileData;
    readFile("g.txt", fileData);

    auto el = compile(fileData);

    for (int i = 0; i < el.size(); i++) {
        std::cout << el[i].str() << '\n';
    }

    return 0;
}