/*
 * @Author: Master 251871605@qq.com
 * @Date: 2023-05-29 17:03:19
 * @LastEditors: Master 251871605@qq.com
 * @LastEditTime: 2023-06-05 22:10:02
 * @FilePath: \MeshTest\MyTest\Dissection.cpp
 * @Description: 
 * 
 * Copyright (c) 2023 by 251871605@qq.com, All Rights Reserved. 
 */
#include "Dissection.h"

Dissection::Dissection():filename(""){}

Dissection::Dissection(std::vector<MyVertex>& poly , string s)
{
     ShellPnts.clear();
     for(int i = 0; i < poly.size();i ++)
     ShellPnts.push_back(poly[i]);
     for(int i = s.length();i >= 0;i --)
     {
          if(s[i] == '/')
          {
               filename = s.substr(i + 1, s.length() - i - 1);
               break;
          }
          if(i == 0)
          filename == s;
     }
     record(poly);
}

Dissection::~Dissection(){}

void Dissection::preprocess()
{
     vector<MyVertex> tool;
     Real dx,dy,x,y,mod;
     MyVertex temp;
     bool clock_wise = clockwise();
     for(int i = 0;i < ShellPnts.size() - 1;i ++)
     {
          dx = ShellPnts[i + 1].P().X() - ShellPnts[i].P().X();
          dy = ShellPnts[i + 1].P().Y() - ShellPnts[i].P().Y();
          if(dy < 0.00001 && dy > -0.00001)
          {
               if(clock_wise)
               {
                    if(dx < -0.00001) { x = 0.0; y = 1.0; }
                    else if(dx > 0.00001) { x = 0.0; y = -1.0; }
               }
               else
               {
                    if(dx < -0.00001) { x = -1.0; y = 0.0; }
                    else if(dx > 0.00001) { x = 0.0; y = 1.0; }
               }
               temp.P() = vcg::Point3d::Construct<double>( x.toDouble() , y.toDouble() , 0.0 );
          }
          else
          {
               x = 1.0;
               y = -dx / dy;
               if(clock_wise)
               {
                    if(dy < -0.00001)
                    { dx *= -1.0 ; dy *= -1.0; }
               }
               else
               {
                    if(dy > 0.00001)
                    { dx *= -1.0 ; dy *= -1.0; }
               }
               mod = sqrt( ( x * x + y * y ).toDouble() );
               temp.P() = vcg::Point3d::Construct<double>( (x/mod).toDouble() , (y/mod).toDouble() , 0.0 );
          }
          normal.push_back( temp );

          Real inner_conductor = Real( ( ShellPnts[i + 1] - ShellPnts[i] ).P().X() ) * Real( temp.P().X() ) + 
                                   Real( ( ShellPnts[i + 1] - ShellPnts[i] ).P().Y() ) * Real( temp.P().Y() );
          std::cout << ( ShellPnts[i + 1] - ShellPnts[i] ).P().X() << " " << ( ShellPnts[i + 1] - ShellPnts[i] ).P().Y() << std::endl;
          std::cout << temp.P().X() << " " << temp.P().Y() << std::endl;
          std::cout << inner_conductor << std::endl;
          std::cout << std::endl;
     }
}

void Dissection::derepeat()
{
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
     Real min_x , dy;
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
     if( dy < EPSILON && dy > EPSILON * (-1.0) )
     {
          dy = ShellPnts[min_subscript].P().Y() - ShellPnts[ (min_subscript - 1 + len - 1) % (len - 1) ].P().Y();
          if(dy > EPSILON)
          return true;
          else
          return false;
     }
     else
     {
          if(dy > EPSILON)
          return true;
          else
          return false;
     }
}

bool Dissection::point_on_edge(MyVertex e1,MyVertex e2,MyVertex v)
{
     Real d1 = sqrt( (e1 - e2) * (e1 - e2) );
     Real d2 = sqrt( (e1 - v) * (e1 - v) );
     Real d3 = sqrt( (e2 - v) * (e2 - v) );
     if( ( d2 < EPSILON && d2 > -EPSILON ) || ( d3 < EPSILON && d3 > -EPSILON ) )
     return false;
     return ( d1 < d2 + d3 + EPSILON && d1 > d2 + d3 - EPSILON ) ;
}

bool Dissection::oppo(int n1,int n2)
{
     if(normal[n1].P().Y() < EPSILON && normal[n1].P().Y() > EPSILON * (-1.0) && normal[n2].P().Y() < EPSILON && normal[n2].P().Y() > EPSILON * (-1.0) )
     {
          if(ShellPnts[n1].P().X() < ShellPnts[n2].P().X() && normal[n1].P().X() > EPSILON && normal[n2].P().X() < EPSILON * (-1.0) )
          return true;
          else if(ShellPnts[n1].P().X() > ShellPnts[n2].P().X() && normal[n1].P().X() < (-1.0) * EPSILON && normal[n2].P().X() > EPSILON)
          return true;
          return false;
     }

     Real x1,x2,y1,y2,k,yb1,yb2;
     k = ( ShellPnts[n1 + 1].P().Y() - ShellPnts[n1].P().Y() ) / ( ShellPnts[n1 + 1].P().X() - ShellPnts[n1].P().X() );
     
     x1 = ShellPnts[n1].P().X();
     y1 = ShellPnts[n1].P().Y();
     x2 = ShellPnts[n2].P().X();
     y2 = ShellPnts[n2].P().Y();
     yb1 = y1 - k * x1;
     yb2 = y2 - k * x2;

     if(yb1 > yb2 && normal[n1].P().Y() < (-1.0) * EPSILON && normal[n2].P().Y() > EPSILON)
     return true;
     if(yb1 < yb2 && normal[n1].P().Y() > EPSILON && normal[n2].P().Y() < (-1.0) * EPSILON )
     return true;
     return false;
     
}

bool Dissection::oppo(MyVertex v1,MyVertex v2,MyVertex v3,MyVertex v4)
{
     Real dx,dy,theta1,theta2,k1,k2;
     dx = v2.P().X() - v1.P().X();
     dy = v2.P().Y() - v1.P().Y();

     if(dy < EPSILON && dy > (-1.0) * EPSILON)
     {
          if(dx > 0)
          theta1 = 0.0;
          else
          theta1 = M_PI;
     }
     else
     {
          if(dx < EPSILON && dx > (-1.0) * EPSILON)
          {
               if(dy > EPSILON)
               theta1 = M_PI_2;
               else
               theta1 = M_PI_2 * 3.0;
          }
          else if(dx > EPSILON)
          theta1 = atan( (dy/dx).toDouble() );
          else
          theta1 = atan( (dy/dx).toDouble() ) + M_PI;
     }
     
     if(!clockwise())
     theta1 += M_PI_2;
     else
     theta1 += 3.0 * M_PI_2;
     
     while(theta1 >= 2.0 * M_PI)
     theta1 -= 2.0 * M_PI;
     dx = v4.P().X() - v3.P().X();
     dy = v4.P().Y() - v3.P().Y(); 
     if(dy < EPSILON && dy > (-1.0) * EPSILON)
     {
          if(dx > 0)
          theta2 = 0.0;
          else
          theta2 = M_PI;
     }
     else
     {
          if(dx < EPSILON && dx > (-1.0) * EPSILON)
          {
               if(dy > EPSILON)
               theta2 = M_PI_2;
               else
               theta2 = M_PI_2 * 3.0;
          }
          else if(dx > EPSILON)
          theta2 = atan( (dy/dx).toDouble());
          else
          theta2 = atan( (dy/dx).toDouble()) + M_PI;
     }
     if(!clockwise())
     theta2 += M_PI_2;
     else
     theta2 += 3.0 * M_PI_2;
     while(theta2 >= 2.0 * M_PI)
     theta2 -= 2.0 * M_PI;
     MyVertex d1,d2;

     d1.P() = vcg::Point3d::Construct<double>( cos(theta1.toDouble()) , sin(theta1.toDouble()) , 0.0 );
     d2.P() = vcg::Point3d::Construct<double>( cos(theta2.toDouble()) , sin(theta2.toDouble()) , 0.0 );
     if( !equal( d1.P().X(),d2.P().Y() * (-1.0) ) && !equal( d1.P().Y() ,d2.P().Y() * (-1.0) ) )
     return false;
     if(d1.P().Y() < EPSILON && d1.P().Y() > (-1.0) * EPSILON && d2.P().Y() < EPSILON && d2.P().Y() > (-1.0) * EPSILON)
     {
          if(v1.P().X() < v3.P().X() && d1.P().X() > EPSILON && d2.P().X() < (-1.0) * EPSILON)
          return true;
          else if(v1.P().X() > v3.P().X() && d1.P().X() < (-1.0) * EPSILON && d2.P().X() > EPSILON)
          return true;
          return false;
     }
     
     Real x1,x2,y1,y2,k,yb1,yb2;
     k = ( v2.P().Y() - v1.P().Y() ) / ( v2.P().X() - v1.P().X() );
        x1 = v1.P().X();
        y1 = v1.P().Y();
        x2 = v3.P().X();
        y2 = v3.P().Y();
        yb1 = y1 - k * x1;
        yb2 = y2 - k * x2;
        if(yb1 > yb2 && d1.P().Y() < (-1.0) * EPSILON && d2.P().Y() > EPSILON)
        return true;
        if(yb1 < yb2 && d1.P().Y() > EPSILON && d2.P().Y() < (-1.0) * EPSILON)
        return true;
        return false;
}

bool Dissection::interleave(int n1,int n2)
{
     Real k1,k2,x1,x2,y1,y2,x,y;
     if(normal[n1].P().Y() < EPSILON && normal[n1].P().Y() > (-1.0) * EPSILON)
     {
          if( ShellPnts[n1].P().Y() > min( ShellPnts[n2].P().Y() , ShellPnts[n2 + 1].P().Y() ) + EPSILON &&
               ShellPnts[n1].P().Y() < max( ShellPnts[n2].P().Y() , ShellPnts[n2 + 1].P().Y() ) - EPSILON )
          return true;
          if( ShellPnts[n1 + 1].P().Y() > min( ShellPnts[n2].P().Y() , ShellPnts[n2 + 1].P().Y() ) + EPSILON &&
               ShellPnts[n1 + 1].P().Y() < max( ShellPnts[n2].P().Y() , ShellPnts[n2 + 1].P().Y() ) - EPSILON )
          return true;
          else
          return false;
     }
     if(normal[n1].P().X() < EPSILON && normal[n1].P().X() > (-1.0) * EPSILON && normal[n2].P().X() < EPSILON && normal[n2].P().X() > (-1.0) * EPSILON)
     {
          if(ShellPnts[n1].P().X() > min( ShellPnts[n2].P().X() , ShellPnts[n2 + 1].P().X() ) + EPSILON && 
               ShellPnts[n1].P().X() < max( ShellPnts[n2].P().X() , ShellPnts[n2 + 1].P().X() ) - EPSILON )
          return true;
          if(ShellPnts[n1 + 1].P().X() > min( ShellPnts[n2].P().X() , ShellPnts[n2 + 1].P().X() ) + EPSILON &&
               ShellPnts[n1 + 1].P().X() < max( ShellPnts[n2].P().X() , ShellPnts[n2 + 1].P().X() ) - EPSILON  )
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

     if( ( x > min( ShellPnts[n2].P().X() , ShellPnts[n2 + 1].P().X() ) + EPSILON &&
          x < max(ShellPnts[n2].P().X() , ShellPnts[n2 + 1].P().X() ) - EPSILON ) || 
          ( y > min(ShellPnts[n2].P().Y() , ShellPnts[n2 + 1].P().Y() ) + EPSILON &&
          y < max(ShellPnts[n2].P().Y() , ShellPnts[n2 + 1].P().Y() ) - EPSILON ) )
     return true;

     x1 = ShellPnts[n1].P().X();
     y1 = ShellPnts[n1].P().Y();
     x2 = ShellPnts[n2].P().X();
     y2 = ShellPnts[n2].P().Y();
     x = (k1 * x1 - y1 - k2 * x2 + y2) / (k1 - k2);
     y = k2 * x - k2 * x2 + y2;
     if( ( x > min( ShellPnts[n2].P().X() , ShellPnts[n2 + 1].P().X() ) + EPSILON &&
          x < max(ShellPnts[n2].P().X() , ShellPnts[n2 + 1].P().X() ) - EPSILON ) || 
          ( y > min(ShellPnts[n2].P().Y() , ShellPnts[n2 + 1].P().Y() ) + EPSILON &&
          y < max(ShellPnts[n2].P().Y() , ShellPnts[n2 + 1].P().Y() ) - EPSILON ) )
     return true;
     return false;
}

bool Dissection::equal(Real d1,Real d2)
{
     if(d1 - d2 > EPSILON || d1 - d2 < (-1.0) * EPSILON)
     return false;
     return true;
}

bool Dissection::isRectangle(MyVertex &v1,MyVertex &v2,MyVertex &v3,MyVertex &v4)
{
     Real v1x = v1.P().X() , v1y = v1.P().Y() , v2x = v2.P().X() , v2y = v2.P().Y() ,
          v3x = v3.P().X() , v3y = v3.P().Y() , v4x = v4.P().X() , v4y = v4.P().Y();
     // Real k = ( v2.P().Y() - v1.P().Y() ) / ( v2.P().X() - v1.P().X() );
     Real l1 = sqrt( ( ( v2y - v1y ) * (v2y - v1y) + (v2x - v1x) * (v2x - v1x) ).toDouble() );
     Real l2 = sqrt( ( ( v4y - v3y) * (v4y - v3y) + (v4x - v3x) * (v4x - v3x) ).toDouble() );
     if(l1 < l2 - EPSILON || l1 > l2 + EPSILON)
     return false;
     Real inner_conductor = Real(v2x - v3x) * Real(v4x - v3x) + Real(v2y - v3y) * Real(v4y - v3y);
     if(inner_conductor > EPSILON || inner_conductor < -EPSILON)
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
                    (temp[i].P().Y() - v2.P().Y()) * (temp[i].P().X() - temp[i + 1].P().X()) ) <= EPSILON &&
                    ( (v1.P().X() - temp[i].P().X()) * (v1.P().Y() - v2.P().Y()) - 
                    (v1.P().Y() - temp[i].P().Y()) * (v1.P().X() - v2.P().X()) ) * 
                    ( (v1.P().X() - temp[i + 1].P().X()) * (v1.P().Y() - v2.P().Y()) - 
                    (v1.P().Y() - temp[i + 1].P().Y()) * (v1.P().X() - v2.P().X()) ) <= EPSILON )
               return true;
          }
     }
     return false;
}

Real Dissection::parallel_distant(MyVertex v1,MyVertex v2,MyVertex v3,MyVertex v4)
{
     if(v2.P().X() - v1.P().X() < EPSILON && v2.P().X() - v1.P().X() > (-1.0) * EPSILON)
     return v3.P().X() > v1.P().X() ? v3.P().X() - v1.P().X() : v1.P().X() - v3.P().X();
     if(v2.P().Y() - v1.P().Y() < EPSILON && v2.P().Y() - v1.P().Y() > (-1.0) * EPSILON)
     return v3.P().Y() > v1.P().Y() ? v3.P().Y() - v1.P().Y() : v1.P().Y() - v3.P().Y();
     Real k1,k2,x1,x2,y1,y2,x,y;
     k2 = ( v2.P().Y() - v1.P().Y() ) / ( v2.P().X() - v1.P().X() );
     k1 = (-1.0) / k2;
     x1 = v2.P().X();
     y1 = v2.P().Y();
     x2 = v3.P().X();
     y2 = v3.P().Y();
     x = (k1 * x1 - y1 - k2 * x2 + y2) / ( k1 - k2 );
     y = k2 * x - k2 * x1 + y1;
     return sqrt( ( (x - x1) * (x - x1) + (y - y1) * (y - y1) ).toDouble() );
}

void Dissection::interpolate()
{
     std::vector<MyVertex> temp;
     std::vector<std::vector<MyVertex>> mark(normal.size());
     bool jud = false;
     int mark_j,cnt = 0;
     Real mn_distant,k;
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
     Real k1,k2,x1,x2,y1,y2,x,y;
     k1 = Real(normal[n1].P().Y()) / Real(normal[n1].P().X());
     k2 = (-1.0) / k1;
     bool jud;
     if(normal[n1].P().Y() < EPSILON && normal[n1].P().Y() > (-1.0) * EPSILON)
     {
          if( ShellPnts[n1 + 1].P().Y() > min( ShellPnts[n2].P().Y() , ShellPnts[n2 + 1].P().Y() ) + EPSILON  && ShellPnts[n1 + 1].P().Y() < max( ShellPnts[n2].P().Y() , ShellPnts[n2 + 1].P().Y() ) - EPSILON )
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
                    now.P() = vcg::Point3d::Construct<double> (ShellPnts[n2].P().X() , ShellPnts[n1 + 1].P().Y() , 0.0);
                    mark[n2].push_back(now);
               }
          }
          if( ShellPnts[n1].P().Y() > min( ShellPnts[n2].P().Y() , ShellPnts[n2 + 1].P().Y() ) + EPSILON  && ShellPnts[n1].P().Y() < max( ShellPnts[n2].P().Y() , ShellPnts[n2 + 1].P().Y() ) - EPSILON )
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
                    now.P() = vcg::Point3d::Construct<double> (ShellPnts[n2].P().X() , ShellPnts[n1].P().Y() , 0.0);
                    mark[n2].push_back(now);
               }
          }
          return ;
     }
     if(normal[n1].P().X() < EPSILON && normal[n1].P().X() > (-1.0) * EPSILON && normal[n2].P().X() < EPSILON && normal[n2].P().X() > (-1.0) * EPSILON)
     {
          if( ShellPnts[n1 + 1].P().X() > min( ShellPnts[n2].P().X() , ShellPnts[n2 + 1].P().X() ) + EPSILON && ShellPnts[n1 + 1].P().X() < max( ShellPnts[n2].P().X() , ShellPnts[n2 + 1].P().X() ) - EPSILON )
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
                    now.P() = vcg::Point3d::Construct<double> (ShellPnts[n1 + 1].P().X() , ShellPnts[n2].P().Y() , 0.0);
                    mark[n2].push_back(now);
               }
          }
          if( ShellPnts[n1].P().X() > min( ShellPnts[n2].P().X() , ShellPnts[n2 + 1].P().X() ) + EPSILON && ShellPnts[n1].P().X() < max( ShellPnts[n2].P().X() , ShellPnts[n2 + 1].P().X() ) - EPSILON )
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
                    now.P() = vcg::Point3d::Construct<double> (ShellPnts[n1].P().X() , ShellPnts[n2].P().Y() , 0.0);
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

     if( ( x > min(ShellPnts[n2].P().X(),ShellPnts[n2 + 1].P().X()) + EPSILON && 
          x < max(ShellPnts[n2].P().X(),ShellPnts[n2 + 1].P().X()) - EPSILON ) || 
          ( y > min(ShellPnts[n2].P().Y(),ShellPnts[n2 + 1].P().Y()) + EPSILON &&
          y < max(ShellPnts[n2].P().Y(),ShellPnts[n2 + 1].P().Y() - EPSILON) ) )
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
               now.P() = vcg::Point3d::Construct<double> (x.toDouble() , y.toDouble() , 0.0);
               
               mark[n2].push_back(now);
          }
     }
     x1 = ShellPnts[n1].P().X();
     y1 = ShellPnts[n1].P().Y();
     x2 = ShellPnts[n2].P().X();
     y2 = ShellPnts[n2].P().Y();
     x = (k1 * x1 - y1 - k2 * x2 + y2) / (k1 - k2);
     y = k2 * x - k2 * x2 + y2;
     
     if( ( x > min(ShellPnts[n2].P().X(),ShellPnts[n2 + 1].P().X()) + EPSILON && 
          x < max(ShellPnts[n2].P().X(),ShellPnts[n2 + 1].P().X()) - EPSILON ) ||
          ( y > min(ShellPnts[n2].P().Y(),ShellPnts[n2 + 1].P().Y()) + EPSILON &&
           y < max(ShellPnts[n2].P().Y(),ShellPnts[n2 + 1].P().Y() - EPSILON ) ) )
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
               now.P() = vcg::Point3d::Construct<double> (x.toDouble() , y.toDouble() , 0.0);
               
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
     vector<vector<MyVertex>> last_res;
     std::vector<MyVertex> temp,now;
     while(!tool.empty())
     {
          Real dist,min_dist;
          int mark_i,mark_j;
          bool jud = false;
          temp.clear();  now.clear();
          temp = tool.top();
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
                    // now.push_back(temp[mark_i]);
                    // res.push_back(now);
                    last_res = postprocess(now);
                    for(int i = 0;i < last_res.size();i ++)
                    {
                         close(last_res[i]);
                         res.push_back(last_res[i]);
                         types.push_back(0);
                    }
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
               Real k1,k2;
               for(int i = 1;i < temp.size() - 1;i ++)
               {
                    k1 = ( temp[i].P().Y() - temp[i - 1].P().Y() ) / ( temp[i].P().X() - temp[i - 1].P().X() );
                    k2 = ( temp[i + 1].P().Y() - temp[i].P().Y() ) / ( temp[i + 1].P().X() - temp[i].P().X() );
                    if(temp[i] == temp[i - 1])
                    {
                         temp.erase(temp.begin() + i);
                         i --;
                    }
                    else if( (temp[i].P().X() - temp[i - 1].P().X() == 0.00000 && temp[i + 1].P().X() - temp[i].P().X() == 0.00000) || ( k1 <= k2 + EPSILON && k1 >= k2 - EPSILON ) )
                    {
                         temp.erase(temp.begin() + i);
                         i --;
                    }
               }
               int len = temp.size();
               k1 = ( temp[1].P().Y() - temp[0].P().Y() ) / ( temp[1].P().X() - temp[0].P().X() );
               k2 = ( temp[len - 1].P().Y() - temp[len - 2].P().Y() ) / ( temp[len - 1].P().X() - temp[len - 2].P().X() );
               if( ( temp[1].P().X() - temp[0].P().X() == 0.00000 && temp[len - 1].P().X() - temp[len - 2].P().X() == 0.00000 ) || ( k1 <= k2 + EPSILON && k1 >= k2 - EPSILON ) )
               {
                    temp.erase(temp.begin());
                    temp[temp.size() - 1] = temp[0];
               }
               if(temp.size() > 3)
               {
                    close(temp);
                    res.push_back(temp);
                    types.push_back(1);
               }
          }
     }
}

vector<vector<MyVertex>> Dissection::postprocess(vector<MyVertex>& raw)
{
     bool mark = false;
     vector<vector<MyVertex>> now_res,next_res;
     vector<MyVertex> temp;
     MyVertex np;
     for(int i = 0;i < raw.size();i ++)
     {
          for(int j = 0;j < ShellPnts.size();j ++)
          {
               if( point_on_edge( raw[i] , raw[(i + 1) % 4] , ShellPnts[j] ) )
               {
                    mark = true;

                    if( ( raw[i] - raw[(i + 1) % 4] ).P().X() < EPSILON && ( raw[i] - raw[(i + 1) % 4] ).P().X() > -EPSILON )
                    np.P() = vcg::Point3d::Construct<double>( raw[(i + 2) % 4].P().X() , ShellPnts[j].P().Y() , 0.0 );
                    else if( ( raw[i] - raw[(i + 1) % 4] ).P().Y() < EPSILON && ( raw[i] - raw[(i + 1) % 4] ).P().Y() > -EPSILON )
                    np.P() = vcg::Point3d::Construct<double>( ShellPnts[j].P().X() , raw[(i + 2) % 4].P().Y() , 0.0 );
                    else
                    {
                         Real k = ( raw[(i + 1) % 4] - raw[i] ).P().Y() / ( raw[(i + 1) % 4] - raw[i] ).P().X();
                         Real b1 = ShellPnts[j].P().Y() + ShellPnts[j].P().X() / k;
                         Real b2 = raw[(i + 2) % 4].P().Y() - raw[(i + 2) % 4].P().X() * k;
                         Real x = k * (b2 - b1) / (k * k + 1);
                         Real y = k * x + b2;
                         np.P() = vcg::Point3d::Construct<double>( x.toDouble() , y.toDouble() , 0.0 );
                    }

                    temp.clear();
                    temp.push_back( raw[i] );  temp.push_back( ShellPnts[j] );
                    temp.push_back( np ); temp.push_back( raw[(i + 3) % 4] );

                    next_res.clear(); next_res = postprocess(temp);
                    for(int i = 0;i < next_res.size();i ++) now_res.push_back(next_res[i]);

                    temp.clear();
                    temp.push_back( ShellPnts[j] ); temp.push_back( raw[(i + 1) % 4] );
                    temp.push_back( raw[(i + 2) % 4] ); temp.push_back( np );

                    next_res.clear(); next_res = postprocess(temp);
                    for(int i = 0;i < next_res.size();i ++) now_res.push_back(next_res[i]);

                    break;

               }
          }
          if(mark)
          break;
     }
     
     if(!mark)
     now_res.push_back(raw);

     return now_res;
}

void Dissection::Info()
{
     // cout << "now have " << ShellPnts.size() << " points" << endl;
     // for(int i = 0;i < ShellPnts.size();i ++)
     // cout << ShellPnts[i].P().X() << " " << ShellPnts[i].P().Y() << endl;
     cout << "now have " << normal.size() << " normals" << endl;
     for(int i = 0;i < normal.size();i ++)
     cout << i << ": " << normal[i].P().X() << " " << normal[i].P().Y() << endl;
}

void Dissection::record(vector<MyVertex> external)
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
     std::cout << "vn: " << mesh.vn << " en: " << mesh.en << " fn: " << mesh.fn << std::endl;
     string s = "../source/ply_source/" + filename.substr(0,filename.size() - 4) + "_original.ply";
     WritePLY(mesh, s);
}

void Dissection::record(vector<vector<MyVertex>> external)
{
     MyMesh mesh;
     int vn = 0,en;
     for(int i = 0;i < external.size();i ++)
     {
          close(external[i]);
          vn += external[i].size() - 1;
          en = vn;
     }
     vcg::tri::Allocator<MyMesh>::AddVertices(mesh , vn);
     vcg::tri::Allocator<MyMesh>::AddEdges(mesh , en);
     int shift = 0;
     for(int i = 0;i < external.size();i ++)
     {
          for(int j = 0;j < external[i].size() - 1;j ++)
               mesh.vert[shift + j] = external[i][j];
          shift += external[i].size() - 1;
     }
     shift = 0;
     for(int i = 0;i < external.size();i ++)
     {
          for(int j = 0;j < external[i].size() - 2;j ++)
          {
               mesh.edge[shift + j].V(0) = &mesh.vert[shift + j];
               mesh.edge[shift + j].V(1) = &mesh.vert[shift + j + 1];
          }
          mesh.edge[shift + external[i].size() - 2].V(0) = &mesh.vert[shift + external[i].size() - 2];
          mesh.edge[shift + external[i].size() - 2].V(1) = &mesh.vert[shift];
          shift += external[i].size() - 1;
     }
     mesh.UpdateDataStructure();
     string s = "../source/ply_result/" + filename.substr(0,filename.size() - 4) + "_result.ply";
     WritePLY(mesh, s);
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
     std::cout << "vn: " << mesh.vn << " en: " << mesh.en << " fn: " << mesh.fn << std::endl;
     WritePLY(mesh, s);
}

std::vector< std::vector<MyVertex> > Dissection::Result()
{
     preprocess();
     derepeat();
     int len = ShellPnts.size();
     if(len == 0)
     return res;
     // Info();
     interpolate();
     std::cout << "vn: " << ShellPnts.size() << " en: " << ShellPnts.size() << std::endl;
     record(ShellPnts , "processing.ply");
     // Info();
     rec_split();
     record(res);
     return res;
}