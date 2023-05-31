/*
 * @Author: Master 251871605@qq.com
 * @Date: 2023-04-20 16:33:00
 * @LastEditors: Master 251871605@qq.com
 * @LastEditTime: 2023-05-31 18:09:48
 * @FilePath: \MeshTest\MyTest\BasicType.h
 * @Description: 
 * 
 * Copyright (c) 2023 by 251871605@qq.com, All Rights Reserved. 
 */
#ifndef BASIC_TYPE_H
#define BASIC_TYPE_H

#include <vector>
#include <cstring>


#include "../vcglib/vcg/simplex/vertex/base.h"
#include "../vcglib/vcg/complex/base.h"
#include "../vcglib/vcg/simplex/vertex/component.h"
#include "../vcglib/vcg/complex/used_types.h"
#include "../vcglib/vcg/complex/algorithms/clean.h"
#include "../vcglib/vcg/complex/allocate.h"

using namespace std;

class MyVertex;
class MyFace;
class MyEdge;

struct MyTriangleTypes : public vcg::UsedTypes< 
        vcg::Use<MyVertex>::AsVertexType, 
        vcg::Use<MyEdge>::AsEdgeType,
        vcg::Use<MyFace>::AsFaceType>{
};

class MyVertex : public vcg::Vertex <MyTriangleTypes , 
                        vcg::vertex::Coord3d , 
                        vcg::vertex::BitFlags ,
                        vcg::vertex::VFAdj>
{
        public:
        MyVertex operator+(MyVertex const& v) const
        {
                MyVertex now;
                now.P() = this->P() + v.P();
                return now;
        }

        MyVertex operator-(MyVertex const& v) const
        {
                MyVertex now;
                now.P() = this->P() - v.P();
                return now;
        }

        MyVertex operator*(double const& s) const
        {
                MyVertex now;
                now.P() = this->P() * s;
                return now;
        }

        MyVertex operator/(double const& s) const
        {
                MyVertex now;
                now.P() = this->P() / s;
                return now;
        }

        double operator*(MyVertex const& v) const
        {
                return this->P() * v.P();
        }

        bool operator==(MyVertex const& v) const
        {
                return this->P() == v.P();
        }

};

class MyEdge : public vcg::Edge <MyTriangleTypes ,
                      vcg::edge::VertexRef ,
                      vcg::edge::BitFlags>
{      
};

class MyFace : public vcg::Face <MyTriangleTypes , 
                      vcg::face::VertexRef , 
                      vcg::face::BitFlags ,
                      vcg::face::FFAdj ,
                      vcg::face::VFAdj ,
                      vcg::face::FEAdj>
{
};

class MyMesh : public vcg::tri::TriMesh<
        std::vector<MyVertex>,
        std::vector<MyEdge>,
        std::vector<MyFace> >
{
        public:
        void UpdateDataStructure() {
                vcg::tri::Clean<MyMesh>::RemoveUnreferencedVertex(*this);
                vcg::tri::Allocator<MyMesh>::CompactEveryVector(*this);
        }
};

#endif // BASIC_TYPE_H