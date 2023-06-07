/*
 * @Author: Master 251871605@qq.com
 * @Date: 2023-06-05 15:54:38
 * @LastEditors: Master 251871605@qq.com
 * @LastEditTime: 2023-06-07 12:22:50
 * @FilePath: \MeshTest\MyTest\RecMesh.h
 * @Description: 
 * 
 * Copyright (c) 2023 by 251871605@qq.com, All Rights Reserved. 
 */
#ifndef RECMESH_HEAD
#define RECMESH_HEAD

#include "BasicType.h"
#include "Function.h"
#include "Shape.h"

class RecMesh
{
    public:
    RecMesh();
    RecMesh(Shape origin);
    ~RecMesh();
    void Mesh();
    void Result(vector<Point> &pnts , vector<vector<int>> &edge);
    protected:
    private:
    Shape origin;
    std::vector<std::vector<Point>> m;
};

#endif 