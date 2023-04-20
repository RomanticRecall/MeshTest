/*
 * @Author: Master 251871605@qq.com
 * @Date: 2023-04-20 16:44:18
 * @LastEditors: Master 251871605@qq.com
 * @LastEditTime: 2023-04-20 19:52:30
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

using namespace std;

class Read
{
    Read();
    Read(string s);
};

class Write
{
    Write();
    Write(string s);
};

#endif // FUNCTION_H