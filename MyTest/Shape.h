#ifndef SHAPE_H
#define SHAPE_H

#include "BasicType.h"

class Shape
{
    public:
    Shape();
    Shape(std::vector<MyVertex>& vertices);
    ~Shape();
    void Info();
    void AdjInfo();
    std::vector<MyVertex> GetVertices();
    bool AddAdjs(Shape * adjshape);
    protected:
    private:
    std::vector<MyVertex> ShellPnts;
    std::vector<Shape *> AdjShapes;
    //throw which edges to adjcent whitch shape , i build index of shape addresses
    std::map<Shape *,std::vector<int>> AdjEdges;
};

#endif