/*
 * @Author: Master 251871605@qq.com
 * @Date: 2023-05-31 16:03:50
 * @LastEditors: Master 251871605@qq.com
 * @LastEditTime: 2023-06-06 12:09:20
 * @FilePath: \MeshTest\MyTest\Convertcases.h
 * @Description: 
 * 
 * Copyright (c) 2023 by 251871605@qq.com, All Rights Reserved. 
 */
#ifndef CONVERTCASES_H
#define CONVERTCASES_H

#include "BasicType.h"
#include <fstream>

class Convertcases
{
    public:
    Convertcases();
    ~Convertcases();
    void read(vector<Point> &res ,std::string s);
    protected:
    private:
};

#endif