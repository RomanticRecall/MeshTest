/*
 * @Author: Master 251871605@qq.com
 * @Date: 2023-05-31 16:03:55
 * @LastEditors: Master 251871605@qq.com
 * @LastEditTime: 2023-05-31 18:14:30
 * @FilePath: \MeshTest\MyTest\Convertcases.cpp
 * @Description: 
 * 
 * Copyright (c) 2023 by 251871605@qq.com, All Rights Reserved. 
 */
#include "Convertcases.h"

Convertcases::Convertcases(){}
Convertcases::~Convertcases(){}

void Convertcases::read(vector<MyVertex> &res ,std::string fn)
{
    std::ifstream ifs;
    ifs.open(fn, std::ios::in);
    string s;
    bool mark = false;
    MyVertex v;
    while(ifs >> s) {
        if(!mark)
        {
            v.P().X() = stof(s);
            mark = true;
        }
        else
        {
            v.P().Y() = stof(s);
            mark = false;
            res.push_back(v);
        }
    }
    ifs.close();
}