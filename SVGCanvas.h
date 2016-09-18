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
#define ATTR2(a,val ) " " << a << "=\"" << val << "\""
#include "SVGHelpers.hpp"

template <class ostream = std::ofstream>
class SVGCanvas {
    using string=std::string;
    ostream os;
    
    void initSVG(double x1, double y1, double x2, double y2) {
        double width = x2-x1;
        double height = y2-y1;
        double lineWidth = std::min(1., std::max(width,height)/500);
        double margin = 5*lineWidth;
        
        os << "<svg version=\"1.1\" baseProfile=\"full\" xmlns=\"http://www.w3.org/2000/svg\""
//           << " width=\"500px\""
           << " height=\"500px\""
           << " viewBox=\"" << x1-margin << ' ' << y1-margin << ' ' << x2-x1 + 2*margin << ' ' << y2-y1 + 2*margin << "\""
           << ">"
           << std::endl;
        openGroup("stroke-width", lineWidth, "stroke", "black", "stroke-linecap", "round");
    }
    
    void initSVG(const SVGPoint& p1, const SVGPoint& p2) {
        initSVG(p1.x, p1.y, p2.x, p2.y);
    }
    
public:
    SVGCanvas(const char* s, double x1, double y1, double x2, double y2):os(s, std::ofstream::out) {
        initSVG(x1, y1, x2, y2);
    }
    SVGCanvas(ostream& os, double x1, double y1, double x2, double y2):os(os) {
        initSVG(x1, y1, x2, y2);
    }
    SVGCanvas(const char* s, const SVGPoint& p1, const SVGPoint& p2):os(s, std::ofstream::out) {
        initSVG(p1, p2);
    }
    SVGCanvas(ostream& os, const SVGPoint& p1, const SVGPoint& p2):os(os) {
        initSVG(p1, p2);
    }
    template<class ValueType>
    void openGroup(const char* attrName, const ValueType& value) {
        os  << "<g" << ATTR2(attrName, value) << ">" << std::endl;
    }
    template<class ValueType, class ValueType2>
    void openGroup(const char* attrName, const ValueType& value, const char* attrName2, const ValueType2& value2) {
        os  << "<g" << ATTR2(attrName, value) << ATTR2(attrName2, value2) << ">" << std::endl;
    }
    
    template<class ValueType, class ValueType2, class ValueType3>
    void openGroup(const char* attrName, const ValueType& value, const char* attrName2, const ValueType2& value2, const char* attrName3, const ValueType3& value3) {
        os  << "<g" << ATTR2(attrName, value) << ATTR2(attrName2, value2) << ATTR2(attrName3, value3) << ">" << std::endl;
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
