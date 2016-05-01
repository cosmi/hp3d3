//
//  main.cpp
//  HP-spline
//
//  Created by Marcin on 27.04.2016.
//  Copyright © 2016 Marcin Skotniczny. All rights reserved.
//

#include <iostream>
#include "Mesh.h"
#include "meshBuilders.h"

#include "test.h"


int main(int argc, const char * argv[]) {
//    selfTest();
    using namespace std;
    const int DIMS = 2;
    Mesh<DIMS> m(CellId<DIMS>({0,0,0},{4,4,4}));
    refineMeshUntilBounds(m, CellId<DIMS>({0,0,0}, {1,1,1}));
    std::cout << m.getElements().size() << "Hello, World!\n";
    for (auto el : m.getElements()) {
        std:: cout << el->getBounds() << std::endl;
    }
    cout << m.getQuadTree().findElementsOverlappingBounds(CellId<DIMS>({1,1,1},{3,3,3})).size() << endl;
    return 0;
}
