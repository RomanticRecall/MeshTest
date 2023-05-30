/*
 * @Author: Master 251871605@qq.com
 * @Date: 2023-04-20 16:31:57
 * @LastEditors: Master 251871605@qq.com
 * @LastEditTime: 2023-05-30 16:39:38
 * @FilePath: \MeshTest\MyTest\test.cpp
 * @Description: 
 * 
 * Copyright (c) 2023 by 251871605@qq.com, All Rights Reserved. 
 */
#include <iostream>
#include "BasicType.h"
#include "Dissection.h"
#include "Function.h"
#include "MyMethod.h"

using namespace std;

int main()
{
    vector<MyVertex> input;
    vector<vector<MyVertex>> output;
    Dissection disc(input);
    output = disc.Result();
    cout << "we split out " << output.size() << " shapes" << endl;
    // MyMesh mesh;
    // string filename = "groud.obj";
    // string filename2 = "groud_output.obj";
    // int loadmask;
    // Read(mesh , filename , loadmask);
    // Info(mesh);
    // MyMethod<MyMesh>::MyUnifrom(mesh , 10);
    // Write(mesh , filename2 , loadmask);
}