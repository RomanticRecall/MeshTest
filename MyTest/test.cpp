/*
 * @Author: Master 251871605@qq.com
 * @Date: 2023-04-20 16:31:57
 * @LastEditors: Master 251871605@qq.com
 * @LastEditTime: 2023-07-18 10:44:52
 * @FilePath: \MeshTest\MyTest\test.cpp
 * @Description: 
 * 
 * Copyright (c) 2023 by 251871605@qq.com, All Rights Reserved. 
 */
#include <iostream>
#include "BasicType.h"
#include "Dissection.h"
#include "Shape.h"
#include "RecMesh.h"
#include "TriMesh.h"
#include "Convertcases.h"
#include "Function.h"
#include "MyMethod.h"

using namespace std;

void MergeAndWriteMesh(std::vector<Shape> domain , string s)
{
    MyMesh mesh;
    std::vector<Point> pnts;
    std::vector<std::vector<int>> edges;
    int pnt_shift = 0 , edge_shift = 0;

    for(int i = 0;i < domain.size();i ++)
    {
        domain[i].Steiner(-1.0);

        if(!domain[i].Type())
        {
            RecMesh recmesh(domain[i]);
            recmesh.Mesh();
            pnts.clear();
            edges.clear();
            recmesh.Result(pnts , edges);
        }
        else
        {
            pnts.clear();
            edges.clear();
            TriMesh trimesh(domain[i]);
            trimesh.Result(pnts,edges);
        }

        vcg::tri::Allocator<MyMesh>::AddVertices(mesh, pnts.size());
        vcg::tri::Allocator<MyMesh>::AddEdges(mesh, edges.size());

        for(int j = 0;j < pnts.size();j ++)
        {
            mesh.vert[j + pnt_shift].P().X() = pnts[j].X().toDouble();
            mesh.vert[j + pnt_shift].P().Y() = pnts[j].Y().toDouble();
            mesh.vert[j + pnt_shift].P().Z() = 0.000000;
        }

        for(int j = 0;j < edges.size();j ++)
        {
            mesh.edge[j + edge_shift].V(0) = &mesh.vert[edges[j][0] + pnt_shift];
            mesh.edge[j + edge_shift].V(1) = &mesh.vert[edges[j][1] + pnt_shift];
        }
        pnt_shift += pnts.size();
        edge_shift += edges.size();
    }
    mesh.UpdateDataStructure();
    string filename = s;
    for(int i = s.length();i >= 0;i --)
    {
        if(s[i] == '/')
        {
            filename = s.substr(i + 1, s.length() - i - 1);
            break;
        }
        if(i == 0)
        filename == s;
    }
    std::cout << "vn: " << mesh.vn << " en: " << mesh.en << " fn: " << mesh.fn << std::endl;
    string fs = "../source/final_result/" + filename.substr(0,filename.size() - 4) + "_recmesh.ply";
    WritePLY(mesh, fs);
}

int main()
{
    vector<Point> input;
    vector<vector<Point>> raw_domain;
    Convertcases cvtcs;
    string s = "../source/txt_source/Mesh_decimal_1.txt";
    cvtcs.read( input , s );
    Dissection disc( input , s );
    raw_domain = disc.Result();
    std::vector<bool> types = disc.Types();

    std::vector<Shape> domain;
    for(int i = 0;i < raw_domain.size();i ++){
        domain.push_back(Shape(raw_domain[i] , types[i]));
    }
    for(int i = 0;i < domain.size();i ++)
    {
        for(int j = 0;j < domain.size();j ++)
        {
            if(i == j)  continue;
            domain[i].AddAdjs(&domain[j]);
        }
    }

    MergeAndWriteMesh(domain , s);

}