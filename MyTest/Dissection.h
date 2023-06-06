/*
 * @Author: Master 251871605@qq.com
 * @Date: 2023-05-29 17:03:13
 * @LastEditors: Master 251871605@qq.com
 * @LastEditTime: 2023-06-06 20:59:00
 * @FilePath: \MeshTest\MyTest\Dissection.h
 * @Description: 
 * 
 * Copyright (c) 2023 by 251871605@qq.com, All Rights Reserved. 
 */
#ifndef DISSECTION_HEAD
#define DISSECTION_HEAD

#include "Point.h"
#include "BasicType.h"
#include "Function.h"

#define EPSILON 0.001

class Dissection
{
    public:
        Dissection();
        Dissection(std::vector<Point> &poly , string s);
        ~Dissection();
        Point Normal(Point p1 , Point p2);
        void preprocess();
        void derepeat();
        void derepeat(std::vector<Point> &temp);
        bool clockwise();
        bool clockwise(std::vector<Point> &temp);
        bool point_on_edge(Point e1,Point e2,Point v);
        bool oppo(int n1,int n2);
        bool oppo(Point p1,Point p2,Point p3,Point p4);
        bool looseoppo(int n1,Point p1,int n2,Point p2);
        bool interleave(int n1,int n2);
        bool equal(Real r1,Real r2);
        bool isRectangle(Point &p1,Point &p2,Point &p3,Point &p4);
        bool intersection(Point &p1,Point &p2,vector<Point> &temp);
        void interbreak(int n1,int n2,vector<vector<Point>>& mark);
        Real parallel_distant(int n1,int n2);
        Real parallel_distant(Point p1,Point p2,Point p3,Point p4);
        void interpolate();
        void close(vector<Point>& temp);
        void rec_split();
        vector<vector<Point>> postprocess(vector<Point>& raw);
        void Info();
        void record(vector<Point> external);
        void record(vector<vector<Point>> external);
        void record(vector<Point> external , string s);
        std::vector<std::vector<Point>> Result();
        std::vector<bool> Types();
    protected:
    private:
        std::string filename;
        std::vector<Point> ShellPnts;
        std::vector<Point> normal;
        std::vector<std::vector<Point>> res;
        std::vector< bool > types;
};

#endif