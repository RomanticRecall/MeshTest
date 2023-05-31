/*
 * @Author: Master 251871605@qq.com
 * @Date: 2023-05-29 17:03:13
 * @LastEditors: Master 251871605@qq.com
 * @LastEditTime: 2023-05-30 14:43:38
 * @FilePath: \MeshTest\MyTest\Dissection.h
 * @Description: 
 * 
 * Copyright (c) 2023 by 251871605@qq.com, All Rights Reserved. 
 */
#ifndef DISSECTION_HEAD
#define DISSECTION_HEAD

#include "BasicType.h"
#include "Function.h"

class Dissection
{
    public:
        Dissection();
        Dissection(std::vector<MyVertex> &poly);
        ~Dissection();
        void preprocess();
        void derepeat();
        void derepeat(std::vector<MyVertex> &temp);
        bool clockwise();
        bool oppo(int n1,int n2);
        bool oppo(MyVertex p1,MyVertex p2,MyVertex p3,MyVertex p4);
        bool looseoppo(int n1,MyVertex p1,int n2,MyVertex p2);
        bool interleave(int n1,int n2);
        bool equal(double r1,double r2);
        bool isRectangle(MyVertex p1,MyVertex p2,MyVertex p3,MyVertex p4);
        bool intersection(MyVertex p1,MyVertex p2,vector<MyVertex> temp);
        void interbreak(int n1,int n2,vector<vector<MyVertex>>& mark);
        double parallel_distant(int n1,int n2);
        double parallel_distant(MyVertex p1,MyVertex p2,MyVertex p3,MyVertex p4);
        void interpolate();
        void close(vector<MyVertex>& temp);
        void rec_split();
        void postprocess();
        void record(vector<MyVertex> external);
        std::vector<std::vector<MyVertex>> Result();
    protected:
    private:
        std::vector<MyVertex> ShellPnts;
        std::vector<MyVertex> normal;
        std::vector<std::vector<MyVertex>> res;
        std::vector< int > types;
};

#endif