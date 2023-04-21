/*
 * @Author: Master 251871605@qq.com
 * @Date: 2023-04-20 16:33:00
 * @LastEditors: Master 251871605@qq.com
 * @LastEditTime: 2023-04-21 15:14:25
 * @FilePath: \MeshTest\MyTest\BasicType.h
 * @Description: 
 * 
 * Copyright (c) 2023 by 251871605@qq.com, All Rights Reserved. 
 */
#ifndef BASIC_TYPE_H
#define BASIC_TYPE_H

#include <vector>


#include "../vcglib/vcg/simplex/vertex/base.h"
#include "../vcglib/vcg/complex/base.h"
#include "../vcglib/vcg/simplex/vertex/component.h"
#include "../vcglib/vcg/complex/used_types.h"

using namespace std;

class MyVertex;
class MyFace;

struct MyTriangleTypes : public vcg::UsedTypes< 
        vcg::Use<MyVertex>::AsVertexType, 
        vcg::Use<MyFace>::AsFaceType>{
};

class MyVertex : public vcg::Vertex <MyTriangleTypes , vcg::vertex::Coord3d>
{
};

class MyFace : public vcg::Face <MyTriangleTypes , vcg::face::VertexRef>
{
};

class MyMesh : public vcg::tri::TriMesh<
        std::vector<MyVertex>,
        std::vector<MyFace> >
{
};

#endif // BASIC_TYPE_H