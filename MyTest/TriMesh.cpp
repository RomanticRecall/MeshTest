/*
 * @Author: Master 251871605@qq.com
 * @Date: 2023-06-07 18:04:19
 * @LastEditors: Master 251871605@qq.com
 * @LastEditTime: 2023-07-12 20:03:17
 * @FilePath: \MeshTest\MyTest\TriMesh.cpp
 * @Description: 
 * 
 * Copyright (c) 2023 by 251871605@qq.com, All Rights Reserved. 
 */
#include "TriMesh.h"

TriMesh::TriMesh(){}

TriMesh::TriMesh(Shape &sp)
{
    vector<Point> temp;
    temp.clear();
    for(int i = 0;i < sp.GetEdges().size();i ++)
    {
        for(int j = 0;j < sp.GetEdges()[i].size() - 1;j ++)
        temp.push_back(sp.GetEdges()[i][j]);
    }
    temp.push_back(sp.GetEdges()[0][0]);
    Construct(temp);
}

TriMesh::~TriMesh(){}

void TriMesh::Construct(vector<Point> &sp)
{
    input.numberofpoints = sp.size();
    input.pointlist = (double *)malloc(2 * input.numberofpoints * sizeof(double));
    for(int i = 0;i < sp.size();i ++)
    {
        input.pointlist[i * 2] = sp[i].X().toDouble();
        input.pointlist[i * 2 + 1] = sp[i].Y().toDouble();
    }
    
    input.pointmarkerlist = (int *) malloc(input.numberofpoints * sizeof(int));
    for(int i = 0;i < input.numberofpoints;i ++)
    input.pointmarkerlist[i] = 1;

    input.numberofsegments = sp.size();
    input.segmentlist = (int *)malloc(2 * input.numberofsegments * sizeof(int));
    for(int i = 0;i < input.numberofsegments - 1;i ++)
    {
        input.segmentlist[i * 2] = i;
        input.segmentlist[i * 2 + 1] = i + 1;
    }
    input.segmentlist[input.numberofsegments * 2 - 2] = input.numberofsegments - 1;
    input.segmentlist[input.numberofsegments * 2 - 1] = 0;
    input.segmentmarkerlist = (int *)malloc(input.numberofsegments * sizeof(int));
    for(int i = 0;i < input.numberofsegments;i ++)
    input.segmentmarkerlist[i] = 1;

    input.numberofholes = 0;
    input.numberofregions = 0;

    output.pointlist = (double *)NULL;
    output.pointattributelist = (double *)NULL;
    output.pointmarkerlist = (int *)NULL;
    output.trianglelist = (int *)NULL;
    output.triangleattributelist = (double *)NULL;
    output.neighborlist = (int *)NULL;
    output.segmentlist = (int *)NULL;
    output.segmentmarkerlist = (int *)NULL;
    output.edgelist = (int *)NULL;
    output.edgemarkerlist = (int *)NULL;

    ostringstream parameter;
    triangulate( "pQzeYYAa10000" , &input , &output , (triangulateio *)NULL);

}

void TriMesh::Result(vector<Point> &pnts , vector<vector<int>> &edges)
{
    vector<int> edge;
    for(int i = 0 ; i < output.numberofpoints ; i ++)
    pnts.push_back(Point(output.pointlist[i * 2] , output.pointlist[i * 2 + 1] , 0.0));

    for(int i = 0 ; i < output.numberofedges ; i ++)
    {
        edge.clear();
        edge.push_back(output.edgelist[i * 2]);
        edge.push_back(output.edgelist[i * 2 + 1]);
        edges.push_back(edge);
    }
}