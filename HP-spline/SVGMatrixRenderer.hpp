//
//  SVGMatrixRenderer.hpp
//  HP-spline
//
//  Created by Marcin Skotniczny on 02.10.2016.
//  Copyright © 2016 Marcin Skotniczny. All rights reserved.
//

#ifndef SVGMatrixRenderer_hpp
#define SVGMatrixRenderer_hpp

#include "SVGCanvas.h"
#include "SparseMatrix.hpp"

template<class SparseMatrix, class SVGCanvas = SVGCanvas<> >
class SVGMatrixRenderer {
    constexpr const static double FIELD_SIZE = 10.;
    SVGCanvas canvas;
    const SparseMatrix& matrix;
    std::string filename;
public:
    SVGMatrixRenderer(const char* s, const SparseMatrix& matrix):
    filename(s),
    canvas(s, 0,0, matrix.getRowsCount()*FIELD_SIZE, matrix.getColsCount()*FIELD_SIZE),
    matrix(matrix) {
        
    }
    
    void draw() {
        auto colNumbers = matrix.getColNumbers();
        auto rowNumbers = matrix.getRowNumbers();
        
        for(auto& col: matrix.getCols()) {
            for(auto& row: col.second) {
                double val = row.second;
                if(isZero(val)) {
                    canvas.openGroup("fill", "red");
                }
                canvas.drawRectangle(SVGPoint(colNumbers[col.first]*FIELD_SIZE, rowNumbers[row.first]*FIELD_SIZE),
                                     SVGPoint((colNumbers[col.first]+1)*FIELD_SIZE, (rowNumbers[row.first]+1)*FIELD_SIZE));
                if(isZero(val)) {
                    canvas.closeGroup();
                }
            }
        }
    }
    
    void close() {
        canvas.close();
    }
    void openImage() {
        system(("open " + filename).c_str());
    }
    void getCanvas() {
        return canvas;
    }
};

template<class RowIdx, class ColIdx, class Val, class RowCompare, class ColCompare>
void renderAndOpen(const SparseMatrix<RowIdx, ColIdx, Val, RowCompare, ColCompare> & sm, const char* filename=nullptr) {
    static int matrixId = 0;
    std::string s = "/tmp/matrix" + toString((int)(matrixId++)) + ".svg";
    SVGMatrixRenderer<SparseMatrix<RowIdx, ColIdx, Val, RowCompare, ColCompare>> r(s.c_str(), sm);
    r.draw();
    r.close();
    r.openImage();
}


#endif /* SVGMatrixRenderer_hpp */
