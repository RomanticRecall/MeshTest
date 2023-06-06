/*
 * @Author: Master 251871605@qq.com
 * @Date: 2023-05-31 16:03:55
 * @LastEditors: Master 251871605@qq.com
 * @LastEditTime: 2023-06-06 12:47:59
 * @FilePath: \MeshTest\MyTest\Convertcases.cpp
 * @Description: 
 * 
 * Copyright (c) 2023 by 251871605@qq.com, All Rights Reserved. 
 */
#include "Convertcases.h"

Convertcases::Convertcases(){}
Convertcases::~Convertcases(){}

void Convertcases::read(vector<Point> &res ,std::string fn)
{
    std::ifstream ifs;
    ifs.open(fn, std::ios::in);
    string s;
    bool mark = false;
    Point v;
    while(ifs >> s) {
        if(!mark)
        {
            v.X() = stod(s);
            mark = true;
        }
        else
        {
            v.Y() = stod(s);
            mark = false;
            res.push_back(v);
        }
    }
    ifs.close();
}