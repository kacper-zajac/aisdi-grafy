#include <iostream>
#include "library.h"

int main() {
    Graph x;
    x.createGraph();
    std::cout << "is empty - " << x.isEmpty() << std::endl;
    std::cout << "is connected - " << x.isEveryVertexConnected() << std::endl;
    x.printGraphEdges();
    x.isCoherent();
}