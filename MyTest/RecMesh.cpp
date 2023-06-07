/*
 * @Author: Master 251871605@qq.com
 * @Date: 2023-06-05 15:54:52
 * @LastEditors: Master 251871605@qq.com
 * @LastEditTime: 2023-06-07 16:36:10
 * @FilePath: \MeshTest\MyTest\RecMesh.cpp
 * @Description: 
 * 
 * Copyright (c) 2023 by 251871605@qq.com, All Rights Reserved. 
 */
#include "RecMesh.h"

RecMesh::RecMesh(){}

RecMesh::RecMesh(Shape origin):origin(origin){}

RecMesh::~RecMesh(){}

void RecMesh::Mesh()
{
    vector<vector<Point>> edges = origin.GetEdges();
    int row = edges[0].size() , col = edges[1].size();
    vector<vector<Point>> matrix(row , vector<Point>(col));
    vector<Point> nowedge = edges[0] , oppoedge = edges[2] , hedge = edges[1] , hoppoedge = edges[3];
    reverse(nowedge); reverse(hoppoedge);
    Point d1,d2;
    Real k1,k2,b1,b2,x,y;
    for(int i = 0;i < col;i ++)
    matrix[0][i] = hedge[i];
    if(edges[0][0].X() < edges[0][1].X() + 0.00001 && edges[0][0].X() > edges[0][1].X() - 0.00001)
    {
        for(int i = 1;i < row - 1;i ++)
        {
            matrix[i][0] = nowedge[i];
            for(int j = 1;j < col - 1;j ++)
            {
                x = hedge[j].X();
                y = nowedge[i].Y();
                matrix[i][j] = Point(x , y);
            }
            matrix[i][col - 1] = oppoedge[i];
        }
    }
    else if(edges[0][0].Y() < edges[0][1].Y() + 0.00001 && edges[0][0].Y() > edges[0][1].Y() - 0.00001)
    {
        for(int i = 1;i < row - 1;i ++)
        {
            matrix[i][0] = nowedge[i];
            for(int j = 1;j < col - 1;j ++)
            {
                x = nowedge[i].X();
                y = hedge[j].Y();
                matrix[i][j] = Point(x , y);
            }
            matrix[i][col - 1] = oppoedge[i];
        }
    }
    else
    {
        for(int i = 1;i < row - 1;i ++)
        {
            d1 = oppoedge[i] - nowedge[i];
            k1 = d1.Y() / d1.X();
            b1 = nowedge[i].Y() - k1 * nowedge[i].X();
            matrix[i][0] = nowedge[i];
            for(int j = 1;j < col - 1;j ++)
            {
                d2 = hoppoedge[j] - hedge[j];
                k2 = d2.Y() / d2.X();
                b2 = hedge[j].Y() - k2 * hedge[j].X();
                x = (b2 - b1) / (k1 - k2);
                y = k1 * x + b1;
                matrix[i][j] = Point(x , y);
            }
            matrix[i][col - 1] = oppoedge[i];
        }
    }
    for(int i = 0;i < col;i ++)
    matrix[row - 1][i] = hoppoedge[i];

    m = matrix;
}

void RecMesh::Result(vector<Point> &pnts , vector<vector<int>> &edge)
{
    pnts.clear();
    edge.clear();
    vector<int> temp;
    int col = m[0].size() , row = m.size();
    
    for(int i = 0;i < row;i ++)
    {
        for(int j = 0;j < col;j ++)
        pnts.push_back(m[i][j]);
    }

    for(int i = 0;i < row - 1;i ++)
    {
        for(int j = 0;j < col - 1;j ++)
        {
            temp.clear();
            temp.push_back(i * col + j);
            temp.push_back(i * col + j + 1);
            edge.push_back(temp);

            temp.clear();
            temp.push_back(i * col + j);
            temp.push_back((i + 1) * col + j);
            edge.push_back(temp);

            temp.clear();
            temp.push_back(i * col + j + 1);
            temp.push_back((i + 1) * col + j + 1);
            edge.push_back(temp);

            temp.clear();
            temp.push_back((i + 1) * col + j);
            temp.push_back((i + 1) * col + j + 1);
            edge.push_back(temp);
        }
    }

}