//
//  SparseMatrix.hpp
//  HP-spline
//
//  Created by Marcin Skotniczny on 24.09.2016.
//  Copyright © 2016 Marcin Skotniczny. All rights reserved.
//

#ifndef SparseMatrix_hpp
#define SparseMatrix_hpp
#include "defs.h"
#include <iostream>
#include <map>
#include <unordered_set>
template<class RowIdx = int, class ColIdx = int, class Val = double>
class SparseMatrix {
    
    
    template<class a, class b>
    using Map = std::map<a,b>;
    Map<RowIdx, Map<ColIdx, Val> > rows;
    Map<ColIdx, Map<RowIdx, Val> > cols;
    int64_t setCount = 0;
public:
    void set(RowIdx row, ColIdx col, Val val) {
        ++setCount;
        rows[row][col] = val;
        cols[col][row] = val;
    }
    const auto& getSetCount() const {
        return setCount;
    }
    const size_t getRowsCount() const {
        return rows.size();
    }
    const size_t getColsCount() const {
        return cols.size();
    }
    const size_t getSetFieldsCount() const {
        size_t res = 0;
        for(auto rowIt: rows) {
            res += rowIt.second.size();
        }
        return res;
    }
    void resetSetCount(int64_t val = 0) {
        setCount = val;
    }
    
    Val get(RowIdx row, ColIdx col) const {
        auto it = rows.find(row);
        if(it == rows.end()) return 0;
        auto it2 = it->second.find(col);
        if(it2 == it->second.end()) return 0;
        return it2->second;
    }
    
    void substractRow(RowIdx what, RowIdx from, Val factor) {
//        std::cout << "SUBS " << what << " from " << from << " by " << factor << std::endl;
        auto& row = rows[what];
        auto& target = rows[from];
        
        for(auto colIt: row) {
            ColIdx colId = colIt.first;
            Val amount = colIt.second*factor;
            auto targetIt = target.find(colId);
            set(from, colId, ((targetIt==target.end())?0:targetIt->second)-amount);
        }
    }
    void multiplyRow(RowIdx rowId, Val factor) {
        auto& row = rows[rowId];
        for(auto colIt: row) {
            ColIdx colId = colIt.first;
            set(rowId, colId, colIt.second*factor);
        }
    }
    
    Map<ColIdx, Val> getResults(ColIdx offCol) const {
        Map<ColIdx, Val> result;
        auto& lastCol = cols.find(offCol)->second;
        for(auto colIt: cols) if(colIt.first != offCol) {
            
            double largest = 0;
            for(auto rowIt: colIt.second) {
                if(rowIt.second > largest && !isZero(rowIt.second)) {
                    result[colIt.first] = lastCol.find(rowIt.first)->second/rowIt.second;
                    largest = rowIt.second;
                }
            }
            assert(largest > 0.99 && largest < 1.01);
        }
        assert(result.size() == cols.size()-1);
        return result;
    }
    
    void eliminate(ColIdx col) {
        std::unordered_set<RowIdx> eliminated_rows;
        
        for(auto colIt : cols) {
            assert(colIt.second.size()>0);
            ColIdx colId = colIt.first;
            if(colId == col) continue;
            
            Val maxVal;// = colIt.second.begin()->second;
            RowIdx maxRowId;// = colIt.second.begin()->first;
            bool first = true;
            for(auto rowIt: colIt.second) if(eliminated_rows.count(rowIt.first) == 0) {
                if(first || abs(rowIt.second) > abs(maxVal)) {
                    maxVal = rowIt.second;
                    maxRowId = rowIt.first;
                    first = false;
                }
            }
            multiplyRow(maxRowId, 1/maxVal);
            for(auto rowIt: colIt.second) if(rowIt.first != maxRowId) {
//                std::cout << "subs " << maxRowId << " " << rowIt.first << " by " << rowIt.second/maxVal << std::endl;
                substractRow(maxRowId, rowIt.first, rowIt.second);
            }
            
//            print(std::cout, nullptr);
            
            eliminated_rows.insert(maxRowId);
        }
    }
    template<class ostream>
    void print(ostream& os, RowIdx fromRow, RowIdx toRow, ColIdx fromCol, ColIdx toCol) {
        for(RowIdx row = fromRow; row<=toRow; row++) {
            for(ColIdx col = fromCol; col<=toCol; col++) {
//                os<<row << ',' <<col<<':';
                os << get(row, col) << '\t';
            }
            os << '\n';
        }
        for(auto colIt: cols) {
            for(auto rowIt: colIt.second) {
                os << rowIt.first << ',' << colIt.first <<':' << rowIt.second <<'\n';
            }
        }
        os<<"#\n";
        for(auto rowIt: rows) {
            for(auto colIt: rowIt.second) {
                os << rowIt.first << ',' << colIt.first <<':' << colIt.second <<'\n';
            }
        }
    }
    
    template<class ostream>
    void print(ostream& os, ColIdx firstCol) {
//        for(auto colIt : cols) if(colIt.first != firstCol){
//            os << colIt.first->getAnchor() << '\n';
//        }
        for(auto rowIt: rows) {
            os << get(rowIt.first, firstCol) << '\t';
            for(auto colIt : cols) if(colIt.first != firstCol){
                //                os<<row << ',' <<col<<':';
                os << get(rowIt.first, colIt.first) << '\t';
            }
            os << '\n';
        }
//        for(auto colIt: cols) {
//            for(auto rowIt: colIt.second) {
//                os << rowIt.first << ',' << colIt.first <<':' << rowIt.second <<'\n';
//            }
//        }
        os<<"!#\n";
//        for(auto rowIt: rows) {
//            for(auto colIt: rowIt.second) {
//                os << rowIt.first << ',' << colIt.first <<':' << colIt.second <<'\n';
//            }
//        }
    }

    
};

#endif /* SparseMatrix_hpp */
