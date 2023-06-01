/*
 * @Author: Master 251871605@qq.com
 * @Date: 2023-05-29 17:03:19
 * @LastEditors: Master 251871605@qq.com
 * @LastEditTime: 2023-06-01 20:21:54
 * @FilePath: \MeshTest\MyTest\Dissection.cpp
 * @Description: 
 * 
 * Copyright (c) 2023 by 251871605@qq.com, All Rights Reserved. 
 */
#include "Dissection.h"

Dissection::Dissection(){}

Dissection::Dissection(std::vector<MyVertex>& poly)
{
     ShellPnts.clear();
     for(int i = 0; i < poly.size();i ++)
     ShellPnts.push_back(poly[i]);
     record(poly , "original.ply");
}

Dissection::~Dissection(){}

void Dissection::preprocess()
{
     vector<MyVertex> tool;
     double dx,dy,theta,min_x = ShellPnts[0].P().X();
     MyVertex temp;
     bool clock_wise = clockwise();
     for(int i = 0;i < ShellPnts.size() - 1;i ++)
     {
          dx = ShellPnts[i + 1].P().X() - ShellPnts[i].P().X();
          dy = ShellPnts[i + 1].P().Y() - ShellPnts[i].P().Y();
          if(dy < EPSIL && dy > (-1.0) * EPSIL )
          {
               if(dx > EPSIL)
               theta = 0.0;
               else
               theta = M_PI;
          }
          else
          {
               if(dx < EPSIL && dx > (-1.0) * EPSIL)
               {
                    if(dy > EPSIL)
                    theta = M_PI_2;
                    else
                    theta = M_PI_2 * 3.0;
               }
               else if(dx > EPSIL)
               theta = atan(dy / dx);
               else
               theta = atan(dy / dx) + M_PI;
          }
          if(!clock_wise)
          theta += M_PI_2;
          else
          theta += M_PI_2 * 3.0;
          while(theta >= M_PI * 2.0)
          theta -= M_PI * 2.0;
          temp.P() = vcg::Point3d::Construct<double>( cos(theta) , sin(theta) , 0.0 );
          normal.push_back( temp );
     }
}

void Dissection::derepeat()
{
     for(int i = 0;i < normal.size();i++)
     cout << normal[i].P().X() << " " << normal[i].P().Y() << " " << endl;
     for(int i = 1;i < normal.size();i ++)
     {
          if(normal[i] == normal[i - 1] || normal[i] == normal[i - 1] * (-1.0) )
          {
               ShellPnts.erase(ShellPnts.begin() + i);
               normal.erase(normal.begin() + i);
               i --;
          }
     }
     if( normal[normal.size() - 1] == normal[0] || normal[normal.size() - 1] == normal[0] * (-1.0) )
     {
          normal.erase( normal.begin() );
          ShellPnts.erase( ShellPnts.begin() );
          ShellPnts[ShellPnts.size() - 1] = ShellPnts[0];
     }
     if(ShellPnts.size() < 3)
     ShellPnts.clear();
}

bool Dissection::clockwise()
{
     double min_x , dy;
     bool first = true;
     int min_subscript = 0;
     for(int i = 0;i < ShellPnts.size();i ++)
     {
          if(first)
          {
               min_x = ShellPnts[i].P().X();
               min_subscript = i;
               first = false;
          }
          else if( ShellPnts[i].P().X() < min_x )
          {
               min_x = ShellPnts[i].P().X();
               min_subscript = i;
          }
     }
     int len = ShellPnts.size();
     dy = ShellPnts[ (min_subscript + 1) % (len - 1) ].P().Y() - ShellPnts[min_subscript].P().Y();
     if( dy < EPSIL && dy > EPSIL * (-1.0) )
     {
          dy = ShellPnts[min_subscript].P().Y() - ShellPnts[ (min_subscript - 1 + len - 1) % (len - 1) ].P().Y();
          if(dy > EPSIL)
          return true;
          else
          return false;
     }
     else
     {
          if(dy > EPSIL)
          return true;
          else
          return false;
     }
}

bool Dissection::oppo(int n1,int n2)
{
     if(normal[n1].P().Y() < EPSIL && normal[n1].P().Y() > EPSIL * (-1.0) && normal[n2].P().Y() < EPSIL && normal[n2].P().Y() > EPSIL * (-1.0) )
     {
          if(ShellPnts[n1].P().X() < ShellPnts[n2].P().X() && normal[n1].P().X() > EPSIL && normal[n2].P().X() < EPSIL * (-1.0) )
          return true;
          else if(ShellPnts[n1].P().X() > ShellPnts[n2].P().X() && normal[n1].P().X() < (-1.0) * EPSIL && normal[n2].P().X() > EPSIL)
          return true;
          return false;
     }

     double x1,x2,y1,y2,k,yb1,yb2;
     k = ( ShellPnts[n1 + 1].P().Y() - ShellPnts[n1].P().Y() ) / ( ShellPnts[n1 + 1].P().X() - ShellPnts[n1].P().X() );
     
     x1 = ShellPnts[n1].P().X();
     y1 = ShellPnts[n1].P().Y();
     x2 = ShellPnts[n2].P().X();
     y2 = ShellPnts[n2].P().Y();
     yb1 = y1 - k * x1;
     yb2 = y2 - k * x2;

     if(yb1 > yb2 && normal[n1].P().Y() < (-1.0) * EPSIL && normal[n2].P().Y() > EPSIL)
     return true;
     if(yb1 < yb2 && normal[n1].P().Y() > EPSIL && normal[n2].P().Y() < (-1.0) * EPSIL )
     return true;
     return false;
     
}

bool Dissection::oppo(MyVertex v1,MyVertex v2,MyVertex v3,MyVertex v4)
{
     double dx,dy,theta1,theta2,k1,k2;
     dx = v2.P().X() - v1.P().X();
     dy = v2.P().Y() - v1.P().Y();

     if(dy < EPSIL && dy > (-1.0) * EPSIL)
     {
          if(dx > 0)
          theta1 = 0.0;
          else
          theta1 = M_PI;
     }
     else
     {
          if(dx < EPSIL && dx > (-1.0) * EPSIL)
          {
               if(dy > EPSIL)
               theta1 = M_PI_2;
               else
               theta1 = M_PI_2 * 3.0;
          }
          else if(dx > EPSIL)
          theta1 = atan(dy/dx);
          else
          theta1 = atan(dy/dx) + M_PI;
     }
     
     if(!clockwise())
     theta1 += M_PI_2;
     else
     theta1 += 3.0 * M_PI_2;
     
     while(theta1 >= 2.0 * M_PI)
     theta1 -= 2.0 * M_PI;
     dx = v4.P().X() - v3.P().X();
     dy = v4.P().Y() - v3.P().Y(); 
     if(dy < EPSIL && dy > (-1.0) * EPSIL)
     {
          if(dx > 0)
          theta2 = 0.0;
          else
          theta2 = M_PI;
     }
     else
     {
          if(dx < EPSIL && dx > (-1.0) * EPSIL)
          {
               if(dy > EPSIL)
               theta2 = M_PI_2;
               else
               theta2 = M_PI_2 * 3.0;
          }
          else if(dx > EPSIL)
          theta2 = atan(dy/dx);
          else
          theta2 = atan(dy/dx) + M_PI;
     }
     if(!clockwise())
     theta2 += M_PI_2;
     else
     theta2 += 3.0 * M_PI_2;
     while(theta2 >= 2.0 * M_PI)
     theta2 -= 2.0 * M_PI;
     MyVertex d1,d2;

     d1.P() = vcg::Point3d::Construct<double>( cos(theta1) , sin(theta1) , 0.0 );
     d2.P() = vcg::Point3d::Construct<double>( cos(theta2) , sin(theta2) , 0.0 );
     if( !equal( d1.P().X(),d2.P().Y() * (-1.0) ) && !equal( d1.P().Y() ,d2.P().Y() * (-1.0) ) )
     return false;
     if(d1.P().Y() < EPSIL && d1.P().Y() > (-1.0) * EPSIL && d2.P().Y() < EPSIL && d2.P().Y() > (-1.0) * EPSIL)
     {
          if(v1.P().X() < v3.P().X() && d1.P().X() > EPSIL && d2.P().X() < (-1.0) * EPSIL)
          return true;
          else if(v1.P().X() > v3.P().X() && d1.P().X() < (-1.0) * EPSIL && d2.P().X() > EPSIL)
          return true;
          return false;
     }
     
     double x1,x2,y1,y2,k,yb1,yb2;
     k = ( v2.P().Y() - v1.P().Y() ) / ( v2.P().X() - v1.P().X() );
        x1 = v1.P().X();
        y1 = v1.P().Y();
        x2 = v3.P().X();
        y2 = v3.P().Y();
        yb1 = y1 - k * x1;
        yb2 = y2 - k * x2;
        if(yb1 > yb2 && d1.P().Y() < (-1.0) * EPSIL && d2.P().Y() > EPSIL)
        return true;
        if(yb1 < yb2 && d1.P().Y() > EPSIL && d2.P().Y() < (-1.0) * EPSIL)
        return true;
        return false;
}

bool Dissection::interleave(int n1,int n2)
{
     double k1,k2,x1,x2,y1,y2,x,y;
     if(normal[n1].P().Y() < EPSIL && normal[n1].P().Y() > (-1.0) * EPSIL)
     {
          if( ShellPnts[n1].P().Y() > min( ShellPnts[n2].P().Y() , ShellPnts[n2 + 1].P().Y() ) + EPSIL &&
               ShellPnts[n1].P().Y() < max( ShellPnts[n2].P().Y() , ShellPnts[n2 + 1].P().Y() ) - EPSIL )
          return true;
          if( ShellPnts[n1 + 1].P().Y() > min( ShellPnts[n2].P().Y() , ShellPnts[n2 + 1].P().Y() ) + EPSIL &&
               ShellPnts[n1 + 1].P().Y() < max( ShellPnts[n2].P().Y() , ShellPnts[n2 + 1].P().Y() ) - EPSIL )
          return true;
          else
          return false;
     }
     if(normal[n1].P().X() < EPSIL && normal[n1].P().X() > (-1.0) * EPSIL && normal[n2].P().X() < EPSIL && normal[n2].P().X() > (-1.0) * EPSIL)
     {
          if(ShellPnts[n1].P().X() > min( ShellPnts[n2].P().X() , ShellPnts[n2 + 1].P().X() ) + EPSIL && 
               ShellPnts[n1].P().X() < max( ShellPnts[n2].P().X() , ShellPnts[n2 + 1].P().X() ) - EPSIL )
          return true;
          if(ShellPnts[n1 + 1].P().X() > min( ShellPnts[n2].P().X() , ShellPnts[n2 + 1].P().X() ) + EPSIL &&
               ShellPnts[n1 + 1].P().X() < max( ShellPnts[n2].P().X() , ShellPnts[n2 + 1].P().X() ) - EPSIL  )
          return true;
          return false;
     }
     k1 = normal[n1].P().Y() / normal[n1].P().X();
     k2 = (-1.0) / k1;
     x1 = ShellPnts[n1 + 1].P().X();
     y1 = ShellPnts[n1 + 1].P().Y();
     x2 = ShellPnts[n2].P().X();
     y2 = ShellPnts[n2].P().Y();
     x = (k1 * x1 - y1 - k2 * x2 + y2) / (k1 - k2);
     y = k2 * x - k2 * x2 + y2;

     if( ( x > min( ShellPnts[n1].P().X() , ShellPnts[n2 + 1].P().X() ) + EPSIL &&
          x < max(ShellPnts[n2].P().X() , ShellPnts[n2 + 1].P().X() ) - EPSIL ) || 
          ( y > min(ShellPnts[n2].P().Y() , ShellPnts[n2 + 1].P().Y() ) + EPSIL &&
          y < max(ShellPnts[n2].P().Y() , ShellPnts[n2 + 1].P().Y() ) - EPSIL ) )
     return true;

     x1 = ShellPnts[n1].P().X();
     y1 = ShellPnts[n1].P().Y();
     x2 = ShellPnts[n2].P().X();
     y2 = ShellPnts[n2].P().Y();
     x = (k1 * x1 - y1 - k2 * x2 + y2) / (k1 - k2);
     y = k2 * x - k2 * x2 + y2;

     if( ( x > min( ShellPnts[n1].P().X() , ShellPnts[n2 + 1].P().X() ) + EPSIL &&
          x < max(ShellPnts[n2].P().X() , ShellPnts[n2 + 1].P().X() ) - EPSIL ) || 
          ( y > min(ShellPnts[n2].P().Y() , ShellPnts[n2 + 1].P().Y() ) + EPSIL &&
          y < max(ShellPnts[n2].P().Y() , ShellPnts[n2 + 1].P().Y() ) - EPSIL ) )
     return true;
     return false;
}

bool Dissection::equal(double d1,double d2)
{
     if(d1 - d2 > EPSIL || d1 - d2 < (-1.0) * EPSIL)
     return false;
     return true;
}

bool Dissection::isRectangle(MyVertex &v1,MyVertex &v2,MyVertex &v3,MyVertex &v4)
{
     double k = ( v2.P().Y() - v1.P().Y() ) / ( v2.P().X() - v1.P().X() );
     double l1 = ( v2.P().Y() - v1.P().Y() ) * (v2.P().Y() - v1.P().Y()) + (v2.P().X() - v1.P().X()) * (v2.P().X() - v1.P().X());
     double l2 = ( v4.P().Y() - v3.P().Y()) * (v4.P().Y() - v3.P().Y()) + (v4.P().X() - v3.P().X()) * (v4.P().X() - v3.P().X());

     if(l1 < l2 - EPSIL || l1 > l2 + EPSIL)
     return false;

     double inner_conductor = (v2.P().X() - v1.P().X()) * (v4.P().X() - v1.P().X()) + (v2.P().Y() - v1.P().Y()) * (v4.P().Y() - v1.P().Y());

     if(inner_conductor > EPSIL || inner_conductor < -EPSIL)
     return false;

     return true;
}

bool Dissection::intersection(MyVertex &v1,MyVertex &v2,vector<MyVertex>& temp)
{
     for(int i = 0; i < temp.size();i ++)
     {
          if( (temp[i] != v1 && temp[i + 1] != v2) && temp[i] != v2 && temp[i + 1] != v1 )
          {
               if(max(v1.P().X(),v2.P().X()) < min(temp[i].P().X(),temp[i + 1].P().X()))
               continue;
               if(max(v1.P().Y(),v2.P().Y()) < min(temp[i].P().Y(),temp[i + 1].P().Y()))
               continue;
               if(min(v1.P().X(),v2.P().X()) > max(temp[i].P().X(),temp[i + 1].P().X()))
               continue;
               if(min(v1.P().Y(),v2.P().Y()) > max(temp[i].P().Y(),temp[i + 1].P().Y()))
               continue;
               if( ( (temp[i].P().X() - v1.P().X()) * (temp[i].P().Y() - temp[i + 1].P().Y()) - 
                    (temp[i].P().Y() - v1.P().Y()) * (temp[i].P().X() - temp[i + 1].P().X()) ) * 
                    ( (temp[i].P().X() - v2.P().X()) * (temp[i].P().Y() - temp[i + 1].P().Y()) - 
                    (temp[i].P().Y() - v2.P().Y()) * (temp[i].P().X() - temp[i + 1].P().X()) ) <= EPSIL &&
                    ( (v1.P().X() - temp[i].P().X()) * (v1.P().Y() - v2.P().Y()) - 
                    (v1.P().Y() - temp[i].P().Y()) * (v1.P().X() - v2.P().X()) ) * 
                    ( (v1.P().X() - temp[i + 1].P().X()) * (v1.P().Y() - v2.P().Y()) - 
                    (v1.P().Y() - temp[i + 1].P().Y()) * (v1.P().X() - v2.P().X()) ) <= EPSIL )
               return true;
          }
     }
     return false;
}

double Dissection::parallel_distant(MyVertex v1,MyVertex v2,MyVertex v3,MyVertex v4)
{
     if(v2.P().X() - v1.P().X() < EPSIL && v2.P().X() - v1.P().X() > (-1.0) * EPSIL)
     return v3.P().X() > v1.P().X() ? v3.P().X() - v1.P().X() : v1.P().X() - v3.P().X();
     if(v2.P().Y() - v1.P().Y() < EPSIL && v2.P().Y() - v1.P().Y() > (-1.0) * EPSIL)
     return v3.P().Y() > v1.P().Y() ? v3.P().Y() - v1.P().Y() : v1.P().Y() - v3.P().Y();
     double k1,k2,x1,x2,y1,y2,x,y;
     k2 = ( v2.P().Y() - v1.P().Y() ) / ( v2.P().X() - v1.P().X() );
     k1 = (-1.0) / k2;
     x1 = v2.P().X();
     y1 = v2.P().Y();
     x2 = v3.P().X();
     y2 = v3.P().Y();
     x = (k1 * x1 - y1 - k2 * x2 + y2) / ( k1 - k2 );
     y = k2 * x - k2 * x1 + y1;
     return sqrt( (x - x1) * (x - x1) + (y - y1) * (y - y1) );
}

void Dissection::interpolate()
{
     std::vector<MyVertex> temp;
     std::vector<std::vector<MyVertex>> mark(normal.size());
     bool jud = false;
     int mark_j,cnt = 0;
     double mn_distant,k;
     for(int i = 0; i < normal.size();i ++)
     {
          jud = false;
          for(int j = 0;j < normal.size();j ++)
          {
               if( i != j && equal(normal[i].P().X(),normal[j].P().X() * (-1.0)) && equal(normal[i].P().Y(),normal[j].P().Y() * (-1.0)) && ( interleave(i,j) || interleave(j,i) ) && oppo(i,j) )
               {
                    k = parallel_distant( ShellPnts[i] , ShellPnts[ (i + 1) % ShellPnts.size() ] , ShellPnts[j] , ShellPnts[ (j + ShellPnts.size() - 1) % ShellPnts.size() ] );
                    if(!jud)
                    {
                         mn_distant = k;
                         mark_j = j;
                         jud = true;
                    }
                    else if(mn_distant > k)
                    {
                         mn_distant = k;
                         mark_j = j;
                    }
               }
          }
          if(jud)
          interbreak(i,mark_j,mark);
     }
     vector<MyVertex> temp_pnts,temp_normal;
     temp_pnts.clear();  temp_normal.clear();
     for(int i = 0; i < mark.size(); i ++)
     {
          temp_pnts.push_back(ShellPnts[i]);
          for(int j = 0;j < mark[i].size();j ++)
               temp_pnts.push_back(mark[i][j]);
          temp_normal.push_back(normal[i]);
          for(int j = 0;j < mark[i].size();j ++)
               temp_normal.push_back(normal[i]);
     }
     close(temp_pnts);
     ShellPnts = temp_pnts;
     normal = temp_normal;
}

void Dissection::interbreak(int n1,int n2,vector<vector<MyVertex>>& mark)
{
     double k1,k2,x1,x2,y1,y2,x,y;
     k1 = normal[n1].P().Y() / normal[n1].P().X();
     k2 = (-1.0) / k1;
     bool jud;
     if(normal[n1].P().Y() < EPSIL && normal[n1].P().Y() > (-1.0) * EPSIL)
     {
          if( ShellPnts[n1 + 1].P().Y() > min( ShellPnts[n2].P().Y() , ShellPnts[n2 + 1].P().Y() ) + EPSIL  && ShellPnts[n1 + 1].P().Y() < max( ShellPnts[n2].P().Y() , ShellPnts[n2 + 1].P().Y() ) - EPSIL )
          {
               jud = true;
               for(int i = 0;i < mark[n2].size();i ++)
               if(equal( ShellPnts[n2].P().X() , mark[n2][i].P().X() ) && equal( ShellPnts[n1 + 1].P().Y() , mark[n2][i].P().Y() ) )
               {
                    jud = false;
                    break;
               }
               if(jud)
               {
                    MyVertex now;
                    now.P().Construct<double> (ShellPnts[n2].P().X() , ShellPnts[n1 + 1].P().Y() , 0.0);
                    mark[n2].push_back(now);
               }
          }
          if( ShellPnts[n1].P().Y() > min( ShellPnts[n2].P().Y() , ShellPnts[n2 + 1].P().Y() ) + EPSIL  && ShellPnts[n1].P().Y() < max( ShellPnts[n2].P().Y() , ShellPnts[n2 + 1].P().Y() ) - EPSIL )
          {
               jud = true;
               for(int i = 0;i < mark[n2].size();i ++)
               if(equal(ShellPnts[n2].P().X() , mark[n2][i].P().X()) && equal(ShellPnts[n1].P().Y() , mark[n2][i].P().Y()) )
               {
                    jud = false;
                    break;
               }
               if(jud)
               {
                    MyVertex now;
                    now.P().Construct<double> (ShellPnts[n2].P().X() , ShellPnts[n1].P().Y() , 0.0);
                    mark[n2].push_back(now);
               }
          }
          return ;
     }
     if(normal[n1].P().X() < EPSIL && normal[n1].P().X() > (-1.0) * EPSIL && normal[n2].P().X() < EPSIL && normal[n2].P().X() > (-1.0) * EPSIL)
     {
          if( ShellPnts[n1 + 1].P().X() > min( ShellPnts[n2].P().X() , ShellPnts[n2 + 1].P().X() ) + EPSIL && ShellPnts[n1 + 1].P().X() < max( ShellPnts[n2].P().X() , ShellPnts[n2 + 1].P().X() ) - EPSIL )
          {
               jud = true;
               for(int i = 0;i < mark[n2].size();i ++)
               if( equal(ShellPnts[n1 + 1].P().X() , mark[n2][i].P().X()) && equal(ShellPnts[n2].P().Y() , mark[n2][i].P().Y()) )
               {
                    jud = false;
                    break;
               }
               if(jud)
               {
                    MyVertex now;
                    now.P().Construct<double> (ShellPnts[n1 + 1].P().X() , ShellPnts[n2].P().Y() , 0.0);
                    mark[n2].push_back(now);
               }
          }
          if( ShellPnts[n1].P().X() > min( ShellPnts[n2].P().X() , ShellPnts[n2 + 1].P().X() ) + EPSIL && ShellPnts[n1].P().X() < max( ShellPnts[n2].P().X() , ShellPnts[n2 + 1].P().X() ) - EPSIL )
          {
               jud = true;
               for(int i = 0;i < mark[n2].size();i ++)
               if( equal(ShellPnts[n1].P().X() , mark[n2][i].P().X()) && equal(ShellPnts[n2].P().Y() , mark[n2][i].P().Y()) )
               {
                    jud = false;
                    break;
               }
               if(jud)
               {
                    MyVertex now;
                    now.P().Construct<double> (ShellPnts[n1].P().X() , ShellPnts[n2].P().Y() , 0.0);
                    mark[n2].push_back(now);
               }
          }
          return ;
     }
     
     x1 = ShellPnts[n1 + 1].P().X();
     y1 = ShellPnts[n1 + 1].P().Y();
     x2 = ShellPnts[n2].P().X();
     y2 = ShellPnts[n2].P().Y();
     x = (k1 * x1 - y1 - k2 * x2 + y2) / (k1 - k2);
     y = k2 * x - k2 * x2 + y2;

     if( ( x > min(ShellPnts[n2].P().X(),ShellPnts[n2 + 1].P().X()) + EPSIL && 
          x < max(ShellPnts[n2].P().X(),ShellPnts[n2 + 1].P().X()) - EPSIL ) || 
          ( y > min(ShellPnts[n2].P().Y(),ShellPnts[n2 + 1].P().Y()) + EPSIL &&
          y < max(ShellPnts[n2].P().Y(),ShellPnts[n2 + 1].P().Y() - EPSIL) ) )
     {
          jud = true;
          for(int i = 0;i < mark[n2].size();i ++)
          if( equal(x , mark[n2][i].P().X()) && equal(y , mark[n2][i].P().Y() ) )
          {
               jud = false;
               break;
          }
          if(jud)
          {
               MyVertex now;
               now.P().Construct<double> (x , y , 0.0);
               mark[n2].push_back(now);
          }
     }
     x1 = ShellPnts[n1].P().X();
     y1 = ShellPnts[n1].P().Y();
     x2 = ShellPnts[n2].P().X();
     y2 = ShellPnts[n2].P().Y();
     x = (k1 * x1 - y1 - k2 * x2 + y2) / (k1 - k2);
     y = k2 * x - k2 * x2 + y2;
     if( ( x > min(ShellPnts[n2].P().X(),ShellPnts[n2 + 1].P().X()) + EPSIL && 
          x < max(ShellPnts[n2].P().X(),ShellPnts[n2 + 1].P().X()) - EPSIL ) ||
          ( y > min(ShellPnts[n2].P().Y(),ShellPnts[n2 + 1].P().Y()) + EPSIL &&
           y < max(ShellPnts[n2].P().Y(),ShellPnts[n2 + 1].P().Y() - EPSIL ) ) )
     {
          jud = true;
          for(int i = 0;i < mark[n2].size();i ++)
          if( equal(x , mark[n2][i].P().X()) && equal(y , mark[n2][i].P().Y()) )
          {
               jud = false;
               break;
          }
          if(jud)
          {
               MyVertex now;
               now.P().Construct<double> (x , y , 0.0);
               mark[n2].push_back(now);
          }
     }
}

void Dissection::close(vector<MyVertex>& temp)
{
     if( ! ( temp[temp.size() - 1] == temp[0] ) )
     temp.push_back(temp[0]);
}

void Dissection::rec_split()
{
     std::stack<std::vector<MyVertex>> tool; 
        tool.push(ShellPnts);
        int cnt = 0;
     while(!tool.empty())
     {
          double dist,min_dist;
          int mark_i,mark_j;
          bool jud = false;
          std::vector<MyVertex> temp = tool.top(),now;
          close(temp);
          // derepeat(temp);
          tool.pop();
          for(int i = 0;i < temp.size();i ++)
          {
               for(int j = i + 2;j + 1 < temp.size();j ++)
               {
                    if( oppo(temp[i],temp[i + 1],temp[j],temp[j + 1]) && isRectangle(temp[i],temp[i + 1],temp[j],temp[j + 1]) &&  !intersection(temp[i],temp[j + 1],temp) && !intersection(temp[i + 1],temp[j],temp) )
                    {
                         dist = parallel_distant(temp[i],temp[i + 1],temp[j],temp[j + 1]);
                         if(!jud)
                         {
                              min_dist = dist;
                              mark_i = i;
                              mark_j = j;
                              jud = true;
                         }
                         else if(min_dist > dist)
                         {
                              min_dist = dist;
                              mark_i = i;
                              mark_j = j;
                         }
                    }
               }

               if(jud)
               {
                    cnt ++;
                    now.clear();
                    now.push_back(temp[mark_i]);
                    now.push_back(temp[mark_i + 1]);
                    now.push_back(temp[mark_j]);
                    now.push_back(temp[mark_j + 1]);
                    now.push_back(temp[mark_i]);
                    res.push_back(now);
                    types.push_back(0);
                    now.clear();
                    for(int j = 0;j < temp.size();j ++)
                    {
                        now.push_back(temp[j]);
                        if(j == mark_i)
                        j = mark_j;
                    }
                    tool.push(now);
                    now.clear();
                    now.push_back(temp[mark_j]);
                    for(int j = mark_i + 1;j <= mark_j;j ++)
                    now.push_back(temp[j]);
                    tool.push(now);

                    break;
               }

          }

          if(!jud)
          {
               double k1,k2;
               for(int i = 1;i < temp.size() - 1;i ++)
               {
                    k1 = ( temp[i].P().Y() - temp[i - 1].P().Y() ) / ( temp[i].P().X() - temp[i - 1].P().X() );
                    k2 = ( temp[i + 1].P().Y() - temp[i].P().Y() ) / ( temp[i + 1].P().X() - temp[i].P().X() );
                    if(temp[i] == temp[i - 1])
                    {
                         temp.erase(temp.begin() + i);
                         i --;
                    }
                    else if( (temp[i].P().X() - temp[i - 1].P().X() == 0.00000 && temp[i + 1].P().X() - temp[i].P().X() == 0.00000) || ( k1 <= k2 + EPSIL && k1 >= k2 - EPSIL ) )
                    {
                         temp.erase(temp.begin() + i);
                         i --;
                    }
               }
               int len = temp.size();
               k1 = ( temp[1].P().Y() - temp[0].P().Y() ) / ( temp[1].P().X() - temp[0].P().X() );
               k2 = ( temp[len - 1].P().Y() - temp[len - 2].P().Y() ) / ( temp[len - 1].P().X() - temp[len - 2].P().X() );
               if( ( temp[1].P().X() - temp[0].P().X() == 0.00000 && temp[len - 1].P().X() - temp[len - 2].P().X() == 0.00000 ) || ( k1 <= k2 + EPSIL && k1 >= k2 - EPSIL ) )
               {
                    temp.erase(temp.begin());
                    temp[temp.size() - 1] = temp[0];
               }
               if(temp.size() > 3)
               {
                    res.push_back(temp);
                    types.push_back(1);
               }
          }
     }
}

void Dissection::record(vector<MyVertex> external , string s)
{
     MyMesh mesh;
     close(external);
     vcg::tri::Allocator<MyMesh>::AddVertices(mesh, external.size() - 1);
     vcg::tri::Allocator<MyMesh>::AddEdges(mesh, external.size() - 1);
     for(int i = 0;i < external.size() - 1;i ++)
     mesh.vert[i] = external[i];
     for(int i = 0;i < external.size() - 2;i ++)
     {
          mesh.edge[i].V(0) = &mesh.vert[i];
          mesh.edge[i].V(1) = &mesh.vert[i + 1];
     }
     mesh.edge[external.size() - 2].V(0) = &mesh.vert[external.size() - 2];
     mesh.edge[external.size() - 2].V(1) = &mesh.vert[0];
     mesh.UpdateDataStructure();
     WritePLY(mesh, s);
}

std::vector< std::vector<MyVertex> > Dissection::Result()
{
     preprocess();
     derepeat();
     int len = ShellPnts.size();
     if(len == 0)
     return res;
     interpolate();
     rec_split();
     vector<MyVertex> rec;
     for(int i = 0;i < res.size();i ++)
     {
          close(res[i]);
          for(int j = 0;j < res[i].size();j ++)
          rec.push_back(res[i][j]);
     }
     cout << rec.size() << endl;
     record(rec , "result.ply");
     return res;
}