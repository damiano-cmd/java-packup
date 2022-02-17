#include "java.h"

int main() 
{

    auto a = any(20);

    std::cout << a.size() << "\n";

    a.set(true);

    std::cout << a.size() << "\n";

    return 0;
}