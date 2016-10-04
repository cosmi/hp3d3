//
//  drawings.cpp
//  HP-spline
//
//  Created by Marcin Skotniczny on 03.10.2016.
//  Copyright © 2016 Marcin Skotniczny. All rights reserved.
//

#include "drawings.hpp"
#include "UniversalSolver.hpp"

void drawMatrices() {
    
}


void drawAllStuff() {
    int SIZE = 8;
    
    
//    return;
    {
        Mesh<2> m(CellId<2>::cube(SIZE));
    
        
        refineMeshUntilBoundsByQuadDivisions(m, CellId<2>({0,0}, {2, 2}));
        
        renderAndOpen(m, "/tmp/Helper1.svg");
    }
    
    //refinement steps
    {
        Mesh<3> m(CellId<3>::cube(SIZE));
        renderAndOpen(m, "/tmp/Refinement1.svg", true);
        for(int i = 0; i<SIZE; i+=4) {
            refineMeshUntilBoundsByQuadDivisions(m, CellId<3>({0,i,0}, {4, i+4,4}));
        }
        renderAndOpen(m, "/tmp/Refinement2.svg", true);
        for(int i = 0; i<SIZE; i+=2) {
            refineMeshUntilBoundsByQuadDivisions(m, CellId<3>({0,i,0}, {2, i+2,2}));
        }
        renderAndOpen(m, "/tmp/Refinement3.svg", true);
    }
    
    SIZE=16;
    
    //point singularity 2D
    {
        Mesh<2> m(CellId<2>::cube(SIZE));
        refineMeshUntilBoundsByQuadDivisions(m, CellId<2>::unit());
        renderAndOpen(m, "/tmp/PointSingularity2D.svg");
        constructAndRenderMatrixWithMidpoints(m, "/tmp/PointSingularity2Dmatrix.svg");
    }
    
    //point singularity 3D
    {
        Mesh<3> m(CellId<3>::cube(SIZE));
        refineMeshUntilBoundsByQuadDivisions(m, CellId<3>::unit());
        renderAndOpen(m, "/tmp/PointSingularity3D.svg", true);
        constructAndRenderMatrixWithMidpoints(m, "/tmp/PointSingularity3Dmatrix.svg");
    }
    
    //point singularity 4D
    {
        Mesh<4> m(CellId<4>::cube(SIZE));
        refineMeshUntilBoundsByQuadDivisions(m, CellId<4>({0,0,0,SIZE-1},{1,1,1,SIZE}));
        renderAndOpen(m, "/tmp/PointSingularity4D.svg");
        constructAndRenderMatrixWithMidpoints(m, "/tmp/PointSingularity4Dmatrix.svg");
    }
    
    //edge singularity 2D
    {
        Mesh<2> m(CellId<2>::cube(SIZE));
        FOR(i, SIZE) {
            refineMeshUntilBoundsByQuadDivisions(m, CellId<2>({0,i}, {1, i+1}));
        }
        renderAndOpen(m, "/tmp/EdgeSingularity2D.svg");
        constructAndRenderMatrixWithMidpoints(m, "/tmp/EdgeSingularity2Dmatrix.svg");
    }
    
    //edge singularity 3D
    {
        Mesh<3> m(CellId<3>::cube(SIZE));
        FOR(i, SIZE) {
            refineMeshUntilBoundsByQuadDivisions(m, CellId<3>({0,i,0}, {1, i+1,1}));
        }
        renderAndOpen(m, "/tmp/EdgeSingularity3D.svg", true);
        constructAndRenderMatrixWithMidpoints(m, "/tmp/EdgeSingularity3Dmatrix.svg");
    }
    
    //edge singularity 4D
    {
        Mesh<4> m(CellId<4>::cube(SIZE));
        FOR(i, SIZE) {
            refineMeshUntilBoundsByQuadDivisions(m, CellId<4>({0,i,0,SIZE-1}, {1, i+1,1,SIZE}));
        }
        
        renderAndOpen(m, "/tmp/EdgeSingularity4D.svg");
        constructAndRenderMatrixWithMidpoints(m, "/tmp/EdgeSingularity4Dmatrix.svg");
    }
    SIZE=8;
    //face singularity 3D
    {
        Mesh<3> m(CellId<3>::cube(SIZE));
        FOR(i, SIZE) {
            FOR(j,SIZE) {
                refineMeshUntilBoundsByQuadDivisions(m, CellId<3>({j,i,0}, {j+1, i+1,1}));
            }
        }
        renderAndOpen(m, "/tmp/FaceSingularity3D.svg", true);
        constructAndRenderMatrixWithMidpoints(m, "/tmp/FaceSingularity3Dmatrix.svg");
    }
    
    //face singularity 4D
    {
        Mesh<4> m(CellId<4>::cube(SIZE));
        FOR(i, SIZE) {
            FOR(j,SIZE) {
                refineMeshUntilBoundsByQuadDivisions(m, CellId<4>({0,i,j,SIZE-1}, {1, i+1,j+1,SIZE}));
            }
        }
        renderAndOpen(m, "/tmp/FaceSingularity4D.svg");
        constructAndRenderMatrixWithMidpoints(m, "/tmp/FaceSingularity4Dmatrix.svg");
    }
    
    //hyperface singularity 4D
    {
        Mesh<4> m(CellId<4>::cube(SIZE));
        FOR(i, SIZE) {
            FOR(j,SIZE) {
                FOR(k,SIZE) {
                    refineMeshUntilBoundsByQuadDivisions(m, CellId<4>({0,i,j,k}, {1, i+1,j+1,k+1}));
                }
            }
        }
        renderAndOpen(m, "/tmp/HyperfaceSingularity4D.svg");
        constructAndRenderMatrixWithMidpoints(m, "/tmp/HyperfaceSingularity4Dmatrix.svg");
    }
    return;
    SIZE=16;
    //point singularity 2D
    {
        Mesh<2> m(CellId<2>({0,0},{SIZE, SIZE}));
        refineMeshUntilBoundsByQuadDivisions(m, CellId<2>::unit());
        enforceSecondNeiborDifference(m);
        renderAndOpen(m, "/tmp/PointSingularity2D+.svg");
    }
    
    //point singularity 3D
    {
        Mesh<3> m(CellId<3>::cube(SIZE));
        refineMeshUntilBoundsByQuadDivisions(m, CellId<3>::unit());
        enforceSecondNeiborDifference(m);
        renderAndOpen(m, "/tmp/PointSingularity3D+.svg", true);
    }
    
    //point singularity 4D
    {
        Mesh<4> m(CellId<4>::cube(SIZE));
        refineMeshUntilBoundsByQuadDivisions(m, CellId<4>({0,0,0,SIZE-1},{1,1,1,SIZE}));
        enforceSecondNeiborDifference(m);
        renderAndOpen(m, "/tmp/PointSingularity4D+.svg");
    }
    
    //edge singularity 2D
    {
        Mesh<2> m(CellId<2>({0,0},{SIZE, SIZE}));
        FOR(i, SIZE) {
            refineMeshUntilBoundsByQuadDivisions(m, CellId<2>({0,i}, {1, i+1}));
        }
        enforceSecondNeiborDifference(m);
        renderAndOpen(m, "/tmp/EdgeSingularity2D+.svg");
    }
    
    //edge singularity 3D
    {
        Mesh<3> m(CellId<3>::cube(SIZE));
        FOR(i, SIZE) {
            refineMeshUntilBoundsByQuadDivisions(m, CellId<3>({0,i,0}, {1, i+1,1}));
        }
        enforceSecondNeiborDifference(m);
        renderAndOpen(m, "/tmp/EdgeSingularity3D+.svg", true);
    }
    
    //edge singularity 4D
    {
        Mesh<4> m(CellId<4>::cube(SIZE));
        FOR(i, SIZE) {
            refineMeshUntilBoundsByQuadDivisions(m, CellId<4>({0,i,0,SIZE-1}, {1, i+1,1,SIZE}));
        }
        enforceSecondNeiborDifference(m);
        renderAndOpen(m, "/tmp/EdgeSingularity4D+.svg");
    }
    SIZE=8;
    //face singularity 3D
    {
        Mesh<3> m(CellId<3>::cube(SIZE));
        FOR(i, SIZE) {
            FOR(j,SIZE) {
                refineMeshUntilBoundsByQuadDivisions(m, CellId<3>({j,i,0}, {j+1, i+1,1}));
            }
        }
        enforceSecondNeiborDifference(m);
        renderAndOpen(m, "/tmp/FaceSingularity3D+.svg", true);
    }
    
    //face singularity 4D
    {
        Mesh<4> m(CellId<4>::cube(SIZE));
        FOR(i, SIZE) {
            FOR(j,SIZE) {
                refineMeshUntilBoundsByQuadDivisions(m, CellId<4>({0,i,j,SIZE-1}, {1, i+1,j+1,SIZE}));
            }
        }
        enforceSecondNeiborDifference(m);
        renderAndOpen(m, "/tmp/FaceSingularity4D+.svg");
    }
    
    //hyperface singularity 4D
    {
        Mesh<4> m(CellId<4>::cube(SIZE));
        FOR(i, SIZE) {
            FOR(j,SIZE) {
                FOR(k,SIZE) {
                    refineMeshUntilBoundsByQuadDivisions(m, CellId<4>({0,i,j,k}, {1, i+1,j+1,k+1}));
                }
            }
        }
        enforceSecondNeiborDifference(m);
        renderAndOpen(m, "/tmp/HyperfaceSingularity4D+.svg");
    }

}