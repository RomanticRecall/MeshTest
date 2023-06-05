/*
 * @Author: Master 251871605@qq.com
 * @Date: 2023-04-20 16:31:57
 * @LastEditors: Master 251871605@qq.com
 * @LastEditTime: 2023-06-05 17:47:41
 * @FilePath: \MeshTest\MyTest\test.cpp
 * @Description: 
 * 
 * Copyright (c) 2023 by 251871605@qq.com, All Rights Reserved. 
 */
#include <iostream>
#include "BasicType.h"
#include "Dissection.h"
#include "Shape.h"
#include "Convertcases.h"
#include "Function.h"
#include "MyMethod.h"


using namespace std;

int main()
{
    vector<MyVertex> input;
    vector<vector<MyVertex>> raw_domain;
    Convertcases cvtcs;
    string s = "../source/txt_source/Mesh_Geom_Path.txt";
    cvtcs.read( input , s );
    Dissection disc( input , s );
    raw_domain = disc.Result();
    cout << "we split out " << raw_domain.size() << " shapes" << endl;
    // std::vector<Shape> domain;
    // for(int i = 0;i < raw_domain.size();i ++){
    //     domain.push_back(Shape(raw_domain[i]));
    // }
    // for(int i = 0;i < domain.size();i ++)
    // {
    //     for(int j = 0;j < domain.size();j ++)
    //     {
    //         if(i == j)  continue;
    //         domain[i].AddAdjs(&domain[j]);
    //     }
    // }
    // for(int i = 0;i < domain.size();i ++)
    // {
    //     domain[i].Info();
    //     domain[i].AdjInfo();
    // }
}