//
//  main.cpp
//  HP-spline
//
//  Created by Marcin on 27.04.2016.
//  Copyright © 2016 Marcin Skotniczny. All rights reserved.
//

#include <iostream>
#include "Mesh.h"

int main(int argc, const char * argv[]) {
    // insert code here...

    
    Mesh<3> m(CellId<3>({0,0,0},{4,4,4}));
    m.splitInAllDims(*m.getElements().begin());
    std::cout << m.getElements().size() << "Hello, World!\n";
    return 0;
}
