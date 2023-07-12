#ifndef TRIMESH_H
#define TRIMESH_H

#include "BasicType.h"
#include "Shape.h"
#include "../triangle/triangle.h"

class TriMesh
{
    public:
    TriMesh();
    TriMesh(Shape &sp);
    ~TriMesh();
    void Construct(vector<Point> &sp);
    void Result(vector<Point> &pnts , vector<vector<int>> &edges);
    protected:
    private:
    triangulateio input , output;
};

#endif