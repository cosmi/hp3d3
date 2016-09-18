//
//  SVGCanvas.h
//  HP-spline
//
//  Created by Marcin Skotniczny on 18.09.2016.
//  Copyright © 2016 Marcin Skotniczny. All rights reserved.
//

#ifndef SVGCanvas_h
#define SVGCanvas_h

#include <iostream>
#include <fstream>
#define ATTR(a) " " << #a << "=\"" << a << "\""
#include "SVGHelpers.hpp"

template <class ostream = std::ofstream>
class SVGCanvas {
    using string=std::string;
    ostream os;
    
    void initSVG(double x1, double y1, double x2, double y2) {
        os << "<svg version=\"1.1\" baseProfile=\"full\" xmlns=\"http://www.w3.org/2000/svg\""
           << " width=\"600px\""
           << " height=\"500px\""
           << " viewBox=\"" << x1 << ' ' << y1 << ' ' << x2 << ' ' << y2 << "\""
           << ">"
           << std::endl;
        openGroup(1, "black");
    }
    
    void initSVG(const SVGPoint& p1, const SVGPoint& p2) {
        initSVG(p1.x, p1.y, p2.x, p2.y);
    }
    
public:
    SVGCanvas(string s, double x1, double y1, double x2, double y2):os(s.c_str(), std::ofstream::out) {
        initSVG(x1, y1, x2, y2);
    }
    SVGCanvas(ostream& os, double x1, double y1, double x2, double y2):os(os) {
        initSVG(x1, y1, x2, y2);
    }
    SVGCanvas(string s, const SVGPoint& p1, const SVGPoint& p2):os(s.c_str(), std::ofstream::out) {
        initSVG(p1, p2);
    }
    SVGCanvas(ostream& os, const SVGPoint& p1, const SVGPoint& p2):os(os) {
        initSVG(p1, p2);
    }
    void openGroup(double strokeWidth, const char* stroke) {
        os  << "<g" << ATTR(stroke) << ATTR(strokeWidth) << ">" << std::endl;
    }
    void openGroup(double strokeWidth) {
        os  << "<g" << ATTR(strokeWidth) << ">" << std::endl;
    }
    void openGroup(const char* stroke) {
        os  << "<g" << ATTR(stroke) << ">" << std::endl;
    }
    void closeGroup() {
        os << "</g>" << std::endl;
    }
    
    void closeSVG() {
        closeGroup();
        os << "</svg>" << std::endl;
    }
    void close() {
        closeSVG();
        os.close();
    }
    void drawLine(double x1, double y1, double x2, double y2) {
        os << "<line x1=\"" << x1 <<"\" y1=\"" << y1 << "\" x2=\"" << x2 << "\" y2=\"" << y2 << "\"/>" << std::endl;
    }
    void drawLine(const SVGPoint& p1, const SVGPoint& p2) {
        drawLine(p1.x, p1.y, p2.x, p2.y);
    }
    void drawLine(const SVGLine& line) {
        drawLine(line.p1, line.p2);
    }
};

#endif /* SVGCanvas_h */
