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
#include <vector>
#include <unordered_map>
template<class RowIdx = int, class ColIdx = int, class Val = double, class RowCompare=std::less<RowIdx>, class ColCompare=std::less<ColIdx>  >
class SparseMatrix {
    
    
    template<class a, class b, class c>
    using Map = std::map<a,b,c>;
    Map<RowIdx, Map<ColIdx, Val, ColCompare>, RowCompare> rows;
    Map<ColIdx, Map<RowIdx, Val, RowCompare>, ColCompare> cols;
    int64_t setCount = 0;
public:
    void set(RowIdx row, ColIdx col, Val val) {
        ++setCount;
        rows[row][col] = val;
        cols[col][row] = val;
    }
    void clear(RowIdx row, ColIdx col) {
        {
            auto it = rows[row].find(col);
            assert(it != rows[row].end());
            rows[row].erase(it);
        }
        {
            auto it = cols[col].find(row);
            assert(it != cols[col].end());
            cols[col].erase(it);
        }
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
    
//    void substractRow(RowIdx what, RowIdx from, Val factor) {
////        std::cout << "SUBS " << what << " from " << from << " by " << factor << std::endl;
//        auto& row = rows[what];
//        auto& target = rows[from];
//        
//        for(auto colIt: row) {
//            ColIdx colId = colIt.first;
//            Val amount = colIt.second*factor;
//            auto targetIt = target.find(colId);
//            set(from, colId, ((targetIt==target.end())?0:targetIt->second)-amount);
//        }
//    }
    void multiplyRow(RowIdx rowId, Val factor) {
        auto& row = rows[rowId];
        for(auto colIt: row) {
            ColIdx colId = colIt.first;
            set(rowId, colId, colIt.second*factor);
        }
    }
    
    Map<ColIdx, Val, ColCompare> getResults(ColIdx offCol) const {
        Map<ColIdx, Val, ColCompare> result;
        auto& lastCol = cols.find(offCol)->second;
        for(auto colIt: cols) if(colIt.first != offCol) {
            
            double largest = 0;
            for(auto rowIt: colIt.second) {
                if(rowIt.second > largest && !isZero(rowIt.second)) {
                    result[colIt.first] = lastCol.find(rowIt.first)->second/rowIt.second;
                    largest = rowIt.second;
                }
            }
//            assert(largest > 0.99 && largest < 1.01);
        }
        assert(result.size() == cols.size()-1);
        return result;
    }
    
    void eliminate(ColIdx col) {
        std::unordered_set<RowIdx> eliminated_rows;
        std::unordered_set<ColIdx> eliminated_cols;
//        print(std::cout, nullptr);
        
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
                    if(!isZero(rowIt.second)) break;
                }
            }
            
            eliminated_rows.insert(maxRowId);
            eliminated_cols.insert(colIt.first);
            
            assert(!isZero(maxVal));
            multiplyRow(maxRowId, 1/maxVal);
            
            auto& row = rows.find(maxRowId)->second;
            for(auto& rowIt: colIt.second) if(rowIt.first != maxRowId) {
//                std::cout << "subs " << maxRowId << " " << rowIt.first << " by " << rowIt.second/maxVal << std::endl;
//                substractRow(maxRowId, rowIt.first, rowIt.second);
                // to avoid problems, zero the column
//                clear(rowIt.first, colIt.first);
                setCount--;
                Val factor = get(rowIt.first, colIt.first);
                set(rowIt.first, colIt.first, 0);
                for(auto& col2It: row) {
                    if(eliminated_cols.count(col2It.first) == 0) {
                        Val oldVal = get(rowIt.first, col2It.first);
                        set(rowIt.first, col2It.first, oldVal-factor*get(maxRowId, col2It.first));
                    }
                }
            }
//            renderAndOpen(*this);
            
//            print(std::cout, nullptr);
            
        }
    }
    void eliminateInOrder(ColIdx col, const std::vector<ColIdx>& order) {
        std::unordered_set<RowIdx> eliminated_rows;
        std::unordered_set<ColIdx> eliminated_cols;
        //        print(std::cout, nullptr);
        
        for(auto colId : order) {
            assert(rows.find(colId) != rows.end());
            assert(cols.find(colId) != cols.end());
            auto& colIt = *cols.find(colId);
            assert(colIt.second.size()>0);
//            ColIdx colId = colIt.first;
            assert(colId != col);
            if(colId == col) continue;
            
//            Val maxVal;// = colIt.second.begin()->second;
//            RowIdx maxRowId;// = colIt.second.begin()->first;
//            bool first = true;
//            for(auto rowIt: colIt.second) if(eliminated_rows.count(rowIt.first) == 0) {
//                if(first || abs(rowIt.second) > abs(maxVal)) {
//                    maxVal = rowIt.second;
//                    maxRowId = rowIt.first;
//                    first = false;
//                    if(!isZero(rowIt.second)) break;
//                }
//            }
            
            eliminated_rows.insert(colId);
//            eliminated_cols.insert(colId);
            
//            assert(!isZero(maxVal));
            multiplyRow(colId, 1/get(colId, colId));
            
            auto& row = rows.find(colId)->second;
            for(auto& rowIt: colIt.second) if(rowIt.first != colId) {
                //                std::cout << "subs " << maxRowId << " " << rowIt.first << " by " << rowIt.second/maxVal << std::endl;
                //                substractRow(maxRowId, rowIt.first, rowIt.second);
                // to avoid problems, zero the column
                //                clear(rowIt.first, colIt.first);
                setCount--;
                Val factor = get(rowIt.first, colIt.first);
                set(rowIt.first, colIt.first, 0);
                for(auto& col2It: row) {
                    if(eliminated_cols.count(col2It.first) == 0) {
                        Val oldVal = get(rowIt.first, col2It.first);
                        set(rowIt.first, col2It.first, oldVal-factor*get(colId, col2It.first));
                    }
                }
            }
            //            renderAndOpen(*this);
            
            //            print(std::cout, nullptr);
            
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

    const Map<ColIdx, Map<RowIdx, Val, RowCompare>, ColCompare> getCols() const {
        return cols;
    }
    const Map<RowIdx, Map<ColIdx, Val, ColCompare>, RowCompare> getRows() const {
        return rows;
    }
    
    std::unordered_map<RowIdx, int> getRowNumbers() const {
        int i = 0;
        std::unordered_map<RowIdx, int> res;
        for(auto& row: rows) {
            res[row.first] = i++;
        }
        return res;
    }
    std::unordered_map<ColIdx, int> getColNumbers() const {
        int i = 0;
        std::unordered_map<ColIdx, int> res;
        for(auto& col: cols) {
            res[col.first] = i++;
        }
        return res;
    }
};

#endif /* SparseMatrix_hpp */
