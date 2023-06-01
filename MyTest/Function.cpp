/*
 * @Author: Master 251871605@qq.com
 * @Date: 2023-04-20 20:00:58
 * @LastEditors: Master 251871605@qq.com
 * @LastEditTime: 2023-06-01 15:37:14
 * @FilePath: \MeshTest\MyTest\Function.cpp
 * @Description: 
 * 
 * Copyright (c) 2023 by 251871605@qq.com, All Rights Reserved. 
 */
#include "Function.h"

void ReadOBJ(MyMesh &mesh, string s , int &loadmask){
    vcg::tri::io::ImporterOBJ<MyMesh>::Open(mesh, s.c_str(), loadmask);
    vcg::tri::UpdateTopology<MyMesh>::FaceFace(mesh);
    vcg::tri::UpdateTopology<MyMesh>::VertexFace(mesh);
}

void WriteOBJ(MyMesh &mesh , string s , int &loadmask){
    vcg::tri::io::ExporterOBJ<MyMesh>::Save(mesh, s.c_str() , loadmask);
}

void WritePLY(MyMesh &mesh , string s){
    // int mask = vcg::tri::io::ExporterPLY<MyMesh>::GetExportMaskCapability();
    int mask = 0;
    mask |= vcg::tri::io::Mask::IOM_EDGEINDEX;
    vcg::tri::io::ExporterPLY<MyMesh>::Save(mesh, s.c_str() , mask);
}

//for test
void MyWriteOBJ(MyMesh &mesh , string s)
{
    std::map<MyVertex *,int> VtoI;
    std::ofstream ofs;
    ofs.open(s.c_str() , std::ios::out);
    for(int i = 0;i < mesh.vn;i ++){
        VtoI[ &mesh.vert[i] ] = i;
        ofs << "v " << mesh.vert[i].P().X() << " " << mesh.vert[i].P().Y() << " " << mesh.vert[i].P().Z() << "\n";
    }
}

void Info(MyMesh &mesh)
{
    std::cout << "Mesh Info : " << std::endl;
    std::cout << "number of points : " << mesh.vn << std::endl;

    // for(int i = 0; i < mesh.vn;i ++){
    //     std::cout << mesh.vert[i].P().X() << " " << mesh.vert[i].P().Y() << " " << mesh.vert[i].P().Z() << std::endl;
    // }

    std::cout << "number of triangles : " << mesh.fn << std::endl;
    std::cout << std::endl;
    // for(int i = 0;i < mesh.fn;i ++){
    //     std::cout << mesh.face[i].V(0)->P().X() << " " << mesh.face[i].V(0)->P().Y() << " " << mesh.face[i].V(0)->P().Z() << std::endl;
    //     std::cout << mesh.face[i].V(1)->P().X() << " " << mesh.face[i].V(1)->P().Y() << " " << mesh.face[i].V(1)->P().Z() << std::endl;
    //     std::cout << mesh.face[i].V(2)->P().X() << " " << mesh.face[i].V(2)->P().Y() << " " << mesh.face[i].V(2)->P().Z() << std::endl;
    //     std::cout << std::endl;
    // }
}