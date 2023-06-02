/*
 * @Author: Master 251871605@qq.com
 * @Date: 2023-05-29 17:03:13
 * @LastEditors: Master 251871605@qq.com
 * @LastEditTime: 2023-06-02 18:26:40
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
        Dissection(std::vector<MyVertex> &poly , string s);
        ~Dissection();
        void preprocess();
        void derepeat();
        void derepeat(std::vector<MyVertex> &temp);
        bool clockwise();
        bool point_on_edge(MyVertex e1,MyVertex e2,MyVertex v);
        bool oppo(int n1,int n2);
        bool oppo(MyVertex p1,MyVertex p2,MyVertex p3,MyVertex p4);
        bool looseoppo(int n1,MyVertex p1,int n2,MyVertex p2);
        bool interleave(int n1,int n2);
        bool equal(double r1,double r2);
        bool isRectangle(MyVertex &p1,MyVertex &p2,MyVertex &p3,MyVertex &p4);
        bool intersection(MyVertex &p1,MyVertex &p2,vector<MyVertex> &temp);
        void interbreak(int n1,int n2,vector<vector<MyVertex>>& mark);
        double parallel_distant(int n1,int n2);
        double parallel_distant(MyVertex p1,MyVertex p2,MyVertex p3,MyVertex p4);
        void interpolate();
        void close(vector<MyVertex>& temp);
        void rec_split();
        vector<vector<MyVertex>> postprocess(vector<MyVertex>& raw);
        void Info();
        void record(vector<MyVertex> external);
        void record(vector<vector<MyVertex>> external);
        void record(vector<MyVertex> external , string s);
        std::vector<std::vector<MyVertex>> Result();
    protected:
    private:
        std::string filename;
        std::vector<MyVertex> ShellPnts;
        std::vector<MyVertex> normal;
        std::vector<std::vector<MyVertex>> res;
        std::vector< int > types;
};

#endif