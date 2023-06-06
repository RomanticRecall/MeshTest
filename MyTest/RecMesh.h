#ifndef RECMESH_HEAD
#define RECMESH_HEAD

#include "BasicType.h"
#include "Shape.h"

class RecMesh
{
    public:
    RecMesh();
    RecMesh(vector<Shape> origin);
    ~RecMesh();
    vector<Shape> Mesh();
    protected:
    private:
    vector<Shape> origin;
};

#endif 