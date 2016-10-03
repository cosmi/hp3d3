//
//  drawings.cpp
//  HP-spline
//
//  Created by Marcin Skotniczny on 03.10.2016.
//  Copyright © 2016 Marcin Skotniczny. All rights reserved.
//

#include "drawings.hpp"


void drawAllStuff() {
    //point singularity 2D
    {
        Mesh<2> m(CellId<2>({0,0},{8, 8}));
        refineMeshUntilBoundsByQuadDivisions(m, CellId<2>::unit());
        renderAndOpen(m, "/tmp/PointSingularity2D.svg");
    }
    
    //point singularity 3D
    {
        Mesh<3> m(CellId<3>::cube(8));
        refineMeshUntilBoundsByQuadDivisions(m, CellId<3>::unit());
        renderAndOpen(m, "/tmp/PointSingularity3D.svg", true);
    }
}