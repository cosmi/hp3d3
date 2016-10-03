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
    
    //point singularity 4D
    {
        Mesh<4> m(CellId<4>::cube(8));
        refineMeshUntilBoundsByQuadDivisions(m, CellId<4>({0,0,0,7},{1,1,1,8}));
        renderAndOpen(m, "/tmp/PointSingularity4D.svg");
    }
    
    //edge singularity 2D
    {
        Mesh<2> m(CellId<2>({0,0},{8, 8}));
        FOR(i, 8) {
            refineMeshUntilBoundsByQuadDivisions(m, CellId<2>({0,i}, {1, i+1}));
        }
        renderAndOpen(m, "/tmp/EdgeSingularity2D.svg");
    }
    
    //edge singularity 3D
    {
        Mesh<3> m(CellId<3>::cube(8));
        FOR(i, 8) {
            refineMeshUntilBoundsByQuadDivisions(m, CellId<3>({0,i,0}, {1, i+1,1}));
        }
        renderAndOpen(m, "/tmp/EdgeSingularity3D.svg", true);
    }
    
    //edge singularity 4D
    {
        Mesh<4> m(CellId<4>::cube(8));
        FOR(i, 8) {
            refineMeshUntilBoundsByQuadDivisions(m, CellId<4>({0,i,0,7}, {1, i+1,1,8}));
        }
        renderAndOpen(m, "/tmp/EdgeSingularity4D.svg");
    }
    //face singularity 3D
    {
        Mesh<3> m(CellId<3>::cube(8));
        FOR(i, 8) {
            FOR(j,8) {
                refineMeshUntilBoundsByQuadDivisions(m, CellId<3>({j,i,0}, {j+1, i+1,1}));
            }
        }
        renderAndOpen(m, "/tmp/FaceSingularity3D.svg", true);
    }
    
    //face singularity 4D
    {
        Mesh<4> m(CellId<4>::cube(8));
        FOR(i, 8) {
            FOR(j,8) {
                refineMeshUntilBoundsByQuadDivisions(m, CellId<4>({0,i,j,7}, {1, i+1,j+1,8}));
            }
        }
        renderAndOpen(m, "/tmp/FaceSingularity4D.svg");
    }
    
    //hyperface singularity 4D
    {
        Mesh<4> m(CellId<4>::cube(8));
        FOR(i, 8) {
            FOR(j,8) {
                FOR(k,8) {
                    refineMeshUntilBoundsByQuadDivisions(m, CellId<4>({0,i,j,k}, {1, i+1,j+1,k+1}));
                }
            }
        }
        renderAndOpen(m, "/tmp/FaceSingularity4D.svg");
    }

}