/*
 * @Author: Master 251871605@qq.com
 * @Date: 2023-04-20 16:31:57
 * @LastEditors: Master 251871605@qq.com
 * @LastEditTime: 2023-06-01 20:27:30
 * @FilePath: \MeshTest\MyTest\test.cpp
 * @Description: 
 * 
 * Copyright (c) 2023 by 251871605@qq.com, All Rights Reserved. 
 */
#include <iostream>
#include "BasicType.h"
#include "Dissection.h"
#include "Convertcases.h"
#include "Function.h"
#include "MyMethod.h"

using namespace std;

int main()
{

    vector<MyVertex> input;
    vector<vector<MyVertex>> output;
    Convertcases cvtcs;
    cvtcs.read( input , "ARC_path_bug1035.txt" );
    Dissection disc(input);
    output = disc.Result();
    cout << "we split out " << output.size() << " shapes" << endl;

}