/*
 * @Author: Master 251871605@qq.com
 * @Date: 2023-04-20 16:44:18
 * @LastEditors: Master 251871605@qq.com
 * @LastEditTime: 2023-06-07 10:36:23
 * @FilePath: \MeshTest\MyTest\Function.h
 * @Description: 
 * 
 * Copyright (c) 2023 by 251871605@qq.com, All Rights Reserved. 
 */
#ifndef FUNCTION_H
#define FUNCTION_H

#include <cstring>
#include "BasicType.h"
#include "../vcglib/wrap/io_trimesh/import.h"
#include "../vcglib/wrap/io_trimesh/export.h"
#include "../vcglib/vcg/complex/algorithms/update/topology.h"
#include "../vcglib/vcg/complex/algorithms/update/flag.h"
#include "../vcglib/vcg/complex/algorithms/update/selection.h"

using namespace std;


void ReadOBJ(MyMesh &mesh, string s, int &loadmask);

void WriteOBJ(MyMesh &mesh, string s, int &loadmask);

void WritePLY(MyMesh &mesh, string s);

void MyWriteOBJ(MyMesh &mesh , string s);

void Info(MyMesh &mesh);

template <class T>
void reverse(vector<T> &temp)
{
    T k;
    if(temp.size() == 0)
    return ;
    for(int i = 0;i < temp.size() / 2;i ++)
    {
        k = temp[i];
        temp[i] = temp[temp.size() - 1 - i];
        temp[temp.size() - 1 - i] = k;
    }
}

#endif // FUNCTION_H