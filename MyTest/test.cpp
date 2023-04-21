/*
 * @Author: Master 251871605@qq.com
 * @Date: 2023-04-20 16:31:57
 * @LastEditors: Master 251871605@qq.com
 * @LastEditTime: 2023-04-21 17:21:48
 * @FilePath: \MeshTest\MyTest\test.cpp
 * @Description: 
 * 
 * Copyright (c) 2023 by 251871605@qq.com, All Rights Reserved. 
 */
#include <iostream>
#include "BasicType.h"
#include "Function.h"

using namespace std;

int main()
{
    MyMesh mesh;
    string filename = "AirBoat.obj";
    string filename2 = "AirBoat_output.obj";
    int loadmask;
    Read(mesh , filename , loadmask);
    Info(mesh);
    Write(mesh , filename2 , loadmask);
}