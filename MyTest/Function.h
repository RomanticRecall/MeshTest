/*
 * @Author: Master 251871605@qq.com
 * @Date: 2023-04-20 16:44:18
 * @LastEditors: Master 251871605@qq.com
 * @LastEditTime: 2023-04-22 22:11:32
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


void Read(MyMesh &mesh, string s, int &loadmask);

void Write(MyMesh &mesh, string s, int &loadmask);

void Info(MyMesh &mesh);

#endif // FUNCTION_H