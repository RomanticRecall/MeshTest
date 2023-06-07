/*
 * @Author: Master 251871605@qq.com
 * @Date: 2023-06-05 16:01:17
 * @LastEditors: Master 251871605@qq.com
 * @LastEditTime: 2023-06-07 14:25:35
 * @FilePath: \MeshTest\MyTest\Shape.h
 * @Description: 
 * 
 * Copyright (c) 2023 by 251871605@qq.com, All Rights Reserved. 
 */
#ifndef SHAPE_H
#define SHAPE_H

#include "BasicType.h"

class Shape
{
    public:
    Shape();
    Shape(std::vector<Point>& vertices , bool type);
    ~Shape();
    void Info();
    void AdjInfo();
    std::vector<std::vector<Point>> GetEdges();
    std::vector<Point> GetVertices();
    bool AddAdjs(Shape * adjshape);
    void interpolation(vector<Point> &edge,Real sz);
    void interpolation(vector<Point> &edge,int sn);
    void Steiner(double sz);
    void Steiner(int sn);
    bool Type();
    protected:
    private:
    bool type;
    std::vector<Point> ShellPnts;
    std::vector<std::vector<Point>> Edges;
    std::vector<Shape *> AdjShapes;
    //throw which edges to adjcent whitch shape , i build index of shape addresses
    std::map<Shape *,std::vector<int>> AdjEdges;
};

#endif