//
//  SVGRenderer.hpp
//  HP-spline
//
//  Created by Marcin Skotniczny on 18.09.2016.
//  Copyright © 2016 Marcin Skotniczny. All rights reserved.
//

#ifndef SVGRenderer_hpp
#define SVGRenderer_hpp

#include "SVGCanvas.h"
#include "projections.hpp"
#include "CellId.h"
#include "Point.h"
#include "Mesh.h"
#include "IGANode.hpp"

template<int DIMS, class Projection = IdentityProjection>
std::pair<SVGPoint, SVGPoint> calculateBounds(const CellId<DIMS>& cid, const Projection& p) {
    auto points = cid.getLowerDimensionalityBounds(0);
    bool first = true;
    SVGPoint ptMin(0,0), ptMax(0,0);
    for(auto & point: points) {
        if(first) {
            ptMin = ptMax = p(point.getFrom());
            first = false;
        } else {
            auto pt2 = p(point.getFrom());
            if(pt2.x < ptMin.x) ptMin.x = pt2.x;
            if(pt2.y < ptMin.y) ptMin.y = pt2.y;
            if(pt2.x > ptMax.x) ptMax.x = pt2.x;
            if(pt2.y > ptMax.y) ptMax.y = pt2.y;
        }
    }
    return std::make_pair(ptMin, ptMax);
}
template<> std::pair<SVGPoint, SVGPoint> calculateBounds<4, PerspectiveProjection> (const CellId<4>& cid, const PerspectiveProjection& p);


template<int DIMS>
bool isFasade(const CellId<DIMS>& line) {
    bool fasade = false;
    FOR(i, DIMS) {
        if(line.getFrom()[i] == 0 && line.getTo()[i] == 0) fasade = true;
    }
    return fasade;
}

template<int DIMS, class Projection = IdentityProjection, class SVGCanvas = SVGCanvas<> >
class SVGRenderer {
    SVGCanvas canvas;
    Projection projection;
    std::string filename;
public:
    SVGRenderer(const char* s, const CellId<DIMS>& bounds, const Projection& p):
        filename(s),
        canvas(s, calculateBounds(bounds, p).first, calculateBounds(bounds, p).second),
        projection(p) {
    }
    SVGRenderer(const char* s, const CellId<DIMS>& bounds):
        filename(s),
        projection(),
        canvas(s, calculateBounds(bounds, Projection()).first, calculateBounds(bounds, Projection()).second){}
    void drawCellDiagonal(const CellId<DIMS>& cid) {
        canvas.drawLine(projection(cid.getFrom()), projection(cid.getTo()));
    }
    void drawBounds(const CellId<DIMS>& bounds, bool onlyFasade = false) {
        canvas.writeComment("bounds " + toString(bounds));
        auto lines = bounds.getLowerDimensionalityBounds(1);
        for(auto & line: lines) {
            if(!onlyFasade || isFasade(line)) {
                drawCellDiagonal(line);
            }
        }
        
//        canvas.drawPoint(projection(bounds));
    }
    void drawMesh(const Mesh<DIMS>& mesh, bool onlyFasade = false) {
        canvas.openGroup("stroke", "black");
        for(auto& element : mesh.getElements()) {
            
            drawBounds(element->getBounds(), onlyFasade);
        }
        
        canvas.closeGroup();
//        drawNeighborsGraph(mesh);
//        canvas.openGroup("stroke", "blue", "fill", "white");
        for(auto& element : mesh.getElements()) {
            if(element->label.size() >0) {
                canvas.drawText(projection(element->getBounds()), element->label );
            }
        }
//        canvas.closeGroup();
    }
    
    void drawBounds4D(const CellId<DIMS>& bounds, const SVGPoint offset, int remDim) {
        canvas.writeComment("bounds4D " + toString(bounds));
        auto lines = bounds.getLowerDimensionalityBounds(1);
        for(auto & line: lines) if(isFasade(line)) {
            auto l1 = line.removeDimension(remDim);
            if(l1.getDimensionality() == 1 && isFasade(l1) ) {
                canvas.drawLine(projection(l1.getFrom()) + offset, projection(l1.getTo()) + offset);
            }
        }
        
        //        canvas.drawPoint(projection(bounds));
    }
    void drawMesh4D(const Mesh<DIMS>& mesh) {
        canvas.openGroup("stroke", "black");
        
        
        auto bounds = calculateBounds(mesh.getBounds(), Projection());
        SVGPoint offset((bounds.second.x - bounds.first.x)/2, (bounds.second.y - bounds.first.y)/2);
        
        for(auto& element : mesh.getElements()) {
            FOR(i, DIMS) {
                drawBounds4D(element->getBounds(), SVGPoint(i&1?offset.x:0, i==1||i==2?0:offset.y), i);
            }
        }
        
        
        canvas.closeGroup();
        
    }
    
    template<int DEG>
    void drawNode(const IGANode<DEG, DIMS>& node) {
//        canvas.openGroup("stroke", "rgba(255,0,0,0.5)");
        canvas.openArrowGroup();
        
        for(auto el: node.getSupport()) {
            if(el->getBounds() == node.getAnchor()) continue;
            canvas.drawLine(projection(el->getBounds()), projection(node.getAnchor()));
        }
        canvas.closeGroup();
//        canvas.openGroup("stroke", "rgba(0,255,0,0.5)");
//        canvas.drawPoint(projection(node.getAnchor()->getBounds()));
//        canvas.closeGroup();
    }
    
    void drawNeighborsGraph(const Mesh<DIMS>& mesh) {
        canvas.openArrowGroup();
        for(auto& el1 : mesh.getElements()) {
            for(auto& el2 : el1->getNeighbors()) {
                canvas.drawLine(projection(el1->getBounds()), projection(el2->getBounds()));
            }
        }
        canvas.closeGroup();
    }
    
    void close() {
//        canvas.openGroup("fill", "yellow");
//        canvas.drawPoint(projection(Point<DIMS>::origin()));
//        canvas.closeGroup();
        canvas.close();
    }
    void openImage() {
        system(("open " + filename).c_str());
    }
    void getCanvas() {
        return canvas;
    }
};


std::string getTmpCanvasFilename();

void renderAndOpen(const Mesh<2>& mesh, const char* filename=nullptr);

void renderAndOpen(const Mesh<3>& mesh, const char* filename=nullptr, bool onlyFasade=false);
void renderAndOpen(const Mesh<4>& mesh, const char* filename=nullptr);

template<class NodeSet>
void renderAndOpen(const Mesh<2>& mesh, const NodeSet& nodes, const char* filename=nullptr) {
    std::string s = filename?filename:getTmpCanvasFilename();
    SVGRenderer<2> r(s.c_str(), mesh.getBounds());
    r.drawMesh(mesh);
    for(auto node: nodes.getNodes()) {
        auto n = dynamic_cast<IGANode<2, 2>*>(node);
        if(node!=nullptr) r.drawNode(*n);
    }
    r.close();
    r.openImage();
}


#endif /* SVGRenderer_hpp */
