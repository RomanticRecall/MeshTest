/*
 * @Author: Master 251871605@qq.com
 * @Date: 2023-05-29 17:03:19
 * @LastEditors: Master 251871605@qq.com
 * @LastEditTime: 2023-07-12 18:34:33
 * @FilePath: \MeshTest\MyTest\Dissection.cpp
 * @Description: 
 * 
 * Copyright (c) 2023 by 251871605@qq.com, All Rights Reserved. 
 */
#include "Dissection.h"

Dissection::Dissection():filename(""){}

Dissection::Dissection(std::vector<Point>& poly , string s)
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

Point Dissection::Normal(Point p1 , Point p2)
{
     Real dx,dy,x,y,mod;
     Point temp;
     bool clock_wise = clockwise();
     dx = p2.X() - p1.X();
     dy = p2.Y() - p1.Y();
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
          temp = Point( x , y , 0.0 );
     }
     else
     {
          x = 1.0;
          y = (-1.0) * dx / dy;
          if(clock_wise)
          {
               if(dy < -0.00001)
               { x *= -1.0 ; y *= -1.0; }
          }
          else
          {
               if(dy > 0.00001)
               { x *= -1.0 ; y *= -1.0; }
          }
          mod = sqrt( ( x * x + y * y ).toDouble() );
          temp = Point( (x/mod) , (y/mod) , 0.0 );
     }
     return temp;
}

void Dissection::preprocess()
{
     vector<Point> tool;
     Point temp;
     for(int i = 0;i < ShellPnts.size() - 1;i ++)
     {
          Point temp = Normal(ShellPnts[i] , ShellPnts[i + 1]);
          normal.push_back( temp );

          // Real inner_conductor = Real( ( ShellPnts[i + 1] - ShellPnts[i] ).X() ) * Real( temp.X() ) + 
          //                          Real( ( ShellPnts[i + 1] - ShellPnts[i] ).Y() ) * Real( temp.Y() );
          // std::cout << ( ShellPnts[i + 1] - ShellPnts[i] ).X() << " " << ( ShellPnts[i + 1] - ShellPnts[i] ).Y() << std::endl;
          // std::cout << temp.X() << " " << temp.Y() << std::endl;
          // std::cout << inner_conductor << std::endl;
          // std::cout << std::endl;
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
               min_x = ShellPnts[i].X();
               min_subscript = i;
               first = false;
          }
          else if( ShellPnts[i].X() < min_x )
          {
               min_x = ShellPnts[i].X();
               min_subscript = i;
          }
     }
     int len = ShellPnts.size();
     dy = ShellPnts[ (min_subscript + 1) % (len - 1) ].Y() - ShellPnts[min_subscript].Y();
     if( dy < EPSILON && dy > EPSILON * (-1.0) )
     {
          dy = ShellPnts[min_subscript].Y() - ShellPnts[ (min_subscript - 1 + len - 1) % (len - 1) ].Y();
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

bool Dissection::clockwise(vector<Point>& sp)
{
     Real min_x , dy;
     bool first = true;
     int min_subscript = 0;
     for(int i = 0;i < sp.size();i ++)
     {
          if(first)
          {
               min_x = sp[i].X();
               min_subscript = i;
               first = false;
          }
          else if( sp[i].X() < min_x )
          {
               min_x = sp[i].X();
               min_subscript = i;
          }
     }
     int len = sp.size();
     dy = sp[ (min_subscript + 1) % (len - 1) ].Y() - sp[min_subscript].Y();
     if( dy < EPSILON && dy > EPSILON * (-1.0) )
     {
          dy = sp[min_subscript].Y() - sp[ (min_subscript - 1 + len - 1) % (len - 1) ].Y();
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

bool Dissection::point_on_edge(Point e1,Point e2,Point v)
{
     Real d1 = sqrt( ( (e1 - e2) * (e1 - e2) ).toDouble() );
     Real d2 = sqrt( ( (e1 - v) * (e1 - v) ).toDouble() );
     Real d3 = sqrt( ( (e2 - v) * (e2 - v) ).toDouble() );
     if( ( d2 < EPSILON && d2 > -EPSILON ) || ( d3 < EPSILON && d3 > -EPSILON ) )
     return false;
     return ( d1 < d2 + d3 + EPSILON && d1 > d2 + d3 - EPSILON ) ;
}

bool Dissection::oppo(int n1,int n2)
{
     return oppo(ShellPnts[n1], ShellPnts[n1 + 1] , ShellPnts[n2] , ShellPnts[n2 + 1]);
     // if(normal[n1].Y() < EPSILON && normal[n1].Y() > EPSILON * (-1.0) && normal[n2].Y() < EPSILON && normal[n2].Y() > EPSILON * (-1.0) )
     // {
     //      if(ShellPnts[n1].X() < ShellPnts[n2].X() && normal[n1].X() > EPSILON && normal[n2].X() < EPSILON * (-1.0) )
     //      return true;
     //      else if(ShellPnts[n1].X() > ShellPnts[n2].X() && normal[n1].X() < (-1.0) * EPSILON && normal[n2].X() > EPSILON)
     //      return true;
     //      return false;
     // }

     // Real x1,x2,y1,y2,k,yb1,yb2;
     // k = ( ShellPnts[n1 + 1].Y() - ShellPnts[n1].Y() ) / ( ShellPnts[n1 + 1].X() - ShellPnts[n1].X() );
     
     // x1 = ShellPnts[n1].X();
     // y1 = ShellPnts[n1].Y();
     // x2 = ShellPnts[n2].X();
     // y2 = ShellPnts[n2].Y();
     // yb1 = y1 - k * x1;
     // yb2 = y2 - k * x2;

     // if(yb1 > yb2 && normal[n1].Y() < (-1.0) * EPSILON && normal[n2].Y() > EPSILON)
     // return true;
     // if(yb1 < yb2 && normal[n1].Y() > EPSILON && normal[n2].Y() < (-1.0) * EPSILON )
     // return true;
     // return false;
     
}

bool Dissection::oppo(Point v1,Point v2,Point v3,Point v4)
{
     Point d1,d2;
     Real theta1,theta2;
     d1 = Normal(v1 , v2);
     d2 = Normal(v3 , v4);

     if( d1 != d2 * (-1.0) )
     return false;
     if(d1.Y() < EPSILON && d1.Y() > (-1.0) * EPSILON)
     {
          if(v1.X() < v3.X() && d1.X() > EPSILON && d2.X() < (-1.0) * EPSILON)
          return true;
          else if(v1.X() > v3.X() && d1.X() < (-1.0) * EPSILON && d2.X() > EPSILON)
          return true;
          return false;
     }
     
     Real x1,x2,y1,y2,k,yb1,yb2;
     k = ( v2.Y() - v1.Y() ) / ( v2.X() - v1.X() );
     x1 = v1.X();
     y1 = v1.Y();
     x2 = v3.X();
     y2 = v3.Y();
     yb1 = y1 - k * x1;
     yb2 = y2 - k * x2;
     if(yb1 > yb2 && d1.Y() < (-1.0) * EPSILON && d2.Y() > EPSILON)
     return true;
     if(yb1 < yb2 && d1.Y() > EPSILON && d2.Y() < (-1.0) * EPSILON)
     return true;
     return false;
}

bool Dissection::interleave(int n1,int n2)
{
     Real k1,k2,x1,x2,y1,y2,x,y;
     if(normal[n1].Y() < EPSILON && normal[n1].Y() > (-1.0) * EPSILON)
     {
          if( ShellPnts[n1].Y() > min( ShellPnts[n2].Y() , ShellPnts[n2 + 1].Y() ) + EPSILON &&
               ShellPnts[n1].Y() < max( ShellPnts[n2].Y() , ShellPnts[n2 + 1].Y() ) - EPSILON )
          return true;
          if( ShellPnts[n1 + 1].Y() > min( ShellPnts[n2].Y() , ShellPnts[n2 + 1].Y() ) + EPSILON &&
               ShellPnts[n1 + 1].Y() < max( ShellPnts[n2].Y() , ShellPnts[n2 + 1].Y() ) - EPSILON )
          return true;
          else
          return false;
     }
     if(normal[n1].X() < EPSILON && normal[n1].X() > (-1.0) * EPSILON && normal[n2].X() < EPSILON && normal[n2].X() > (-1.0) * EPSILON)
     {
          if(ShellPnts[n1].X() > min( ShellPnts[n2].X() , ShellPnts[n2 + 1].X() ) + EPSILON && 
               ShellPnts[n1].X() < max( ShellPnts[n2].X() , ShellPnts[n2 + 1].X() ) - EPSILON )
          return true;
          if(ShellPnts[n1 + 1].X() > min( ShellPnts[n2].X() , ShellPnts[n2 + 1].X() ) + EPSILON &&
               ShellPnts[n1 + 1].X() < max( ShellPnts[n2].X() , ShellPnts[n2 + 1].X() ) - EPSILON  )
          return true;
          return false;
     }
     k1 = normal[n1].Y() / normal[n1].X();
     k2 = (-1.0) / k1;
     x1 = ShellPnts[n1 + 1].X();
     y1 = ShellPnts[n1 + 1].Y();
     x2 = ShellPnts[n2].X();
     y2 = ShellPnts[n2].Y();
     x = (k1 * x1 - y1 - k2 * x2 + y2) / (k1 - k2);
     y = k2 * x - k2 * x2 + y2;

     if( ( x > min( ShellPnts[n2].X() , ShellPnts[n2 + 1].X() ) + EPSILON &&
          x < max(ShellPnts[n2].X() , ShellPnts[n2 + 1].X() ) - EPSILON ) || 
          ( y > min(ShellPnts[n2].Y() , ShellPnts[n2 + 1].Y() ) + EPSILON &&
          y < max(ShellPnts[n2].Y() , ShellPnts[n2 + 1].Y() ) - EPSILON ) )
     return true;

     x1 = ShellPnts[n1].X();
     y1 = ShellPnts[n1].Y();
     x2 = ShellPnts[n2].X();
     y2 = ShellPnts[n2].Y();
     x = (k1 * x1 - y1 - k2 * x2 + y2) / (k1 - k2);
     y = k2 * x - k2 * x2 + y2;
     if( ( x > min( ShellPnts[n2].X() , ShellPnts[n2 + 1].X() ) + EPSILON &&
          x < max(ShellPnts[n2].X() , ShellPnts[n2 + 1].X() ) - EPSILON ) || 
          ( y > min(ShellPnts[n2].Y() , ShellPnts[n2 + 1].Y() ) + EPSILON &&
          y < max(ShellPnts[n2].Y() , ShellPnts[n2 + 1].Y() ) - EPSILON ) )
     return true;
     return false;
}

bool Dissection::equal(Real d1,Real d2)
{
     if(d1 - d2 > EPSILON || d1 - d2 < (-1.0) * EPSILON)
     return false;
     return true;
}

bool Dissection::isRectangle(Point &v1,Point &v2,Point &v3,Point &v4)
{
     Real v1x = v1.X() , v1y = v1.Y() , v2x = v2.X() , v2y = v2.Y() ,
          v3x = v3.X() , v3y = v3.Y() , v4x = v4.X() , v4y = v4.Y();
     // Real k = ( v2.Y() - v1.Y() ) / ( v2.X() - v1.X() );
     Real l1 = sqrt( ( ( v2y - v1y ) * (v2y - v1y) + (v2x - v1x) * (v2x - v1x) ).toDouble() );
     Real l2 = sqrt( ( ( v4y - v3y) * (v4y - v3y) + (v4x - v3x) * (v4x - v3x) ).toDouble() );
     if(l1 < l2 - EPSILON || l1 > l2 + EPSILON)
     return false;
     Real inner_conductor = Real(v2x - v3x) * Real(v4x - v3x) + Real(v2y - v3y) * Real(v4y - v3y);
     if(inner_conductor < -0.05 || inner_conductor > 0.05)
     return false;

     return true;
}

bool Dissection::intersection(Point &v1,Point &v2,vector<Point>& temp)
{
     for(int i = 0; i < temp.size();i ++)
     {
          if( (temp[i] != v1 && temp[i + 1] != v2) && temp[i] != v2 && temp[i + 1] != v1 )
          {
               if(max(v1.X(),v2.X()) < min(temp[i].X(),temp[i + 1].X()))
               continue;
               if(max(v1.Y(),v2.Y()) < min(temp[i].Y(),temp[i + 1].Y()))
               continue;
               if(min(v1.X(),v2.X()) > max(temp[i].X(),temp[i + 1].X()))
               continue;
               if(min(v1.Y(),v2.Y()) > max(temp[i].Y(),temp[i + 1].Y()))
               continue;
               if( ( (temp[i].X() - v1.X()) * (temp[i].Y() - temp[i + 1].Y()) - 
                    (temp[i].Y() - v1.Y()) * (temp[i].X() - temp[i + 1].X()) ) * 
                    ( (temp[i].X() - v2.X()) * (temp[i].Y() - temp[i + 1].Y()) - 
                    (temp[i].Y() - v2.Y()) * (temp[i].X() - temp[i + 1].X()) ) <= EPSILON &&
                    ( (v1.X() - temp[i].X()) * (v1.Y() - v2.Y()) - 
                    (v1.Y() - temp[i].Y()) * (v1.X() - v2.X()) ) * 
                    ( (v1.X() - temp[i + 1].X()) * (v1.Y() - v2.Y()) - 
                    (v1.Y() - temp[i + 1].Y()) * (v1.X() - v2.X()) ) <= EPSILON )
               return true;
          }
     }
     return false;
}

Real Dissection::parallel_distant(Point v1,Point v2,Point v3,Point v4)
{
     if(v2.X() - v1.X() < EPSILON && v2.X() - v1.X() > (-1.0) * EPSILON)
     return v3.X() > v1.X() ? v3.X() - v1.X() : v1.X() - v3.X();
     if(v2.Y() - v1.Y() < EPSILON && v2.Y() - v1.Y() > (-1.0) * EPSILON)
     return v3.Y() > v1.Y() ? v3.Y() - v1.Y() : v1.Y() - v3.Y();
     Real k1,k2,x1,x2,y1,y2,x,y;
     k2 = ( v2.Y() - v1.Y() ) / ( v2.X() - v1.X() );
     k1 = (-1.0) / k2;
     x1 = v2.X();
     y1 = v2.Y();
     x2 = v3.X();
     y2 = v3.Y();
     x = (k1 * x1 - y1 - k2 * x2 + y2) / ( k1 - k2 );
     y = k2 * x - k2 * x1 + y1;
     return sqrt( ( (x - x1) * (x - x1) + (y - y1) * (y - y1) ).toDouble() );
}

void Dissection::interpolate()
{
     std::vector<Point> temp;
     std::vector<std::vector<Point>> mark(normal.size());
     bool jud = false;
     int mark_j,cnt = 0;
     Real mn_distant,k;
     for(int i = 0; i < normal.size();i ++)
     {
          jud = false;
          for(int j = 0;j < normal.size();j ++)
          {
               if( i != j && normal[i] == normal[j] * (-1.0) && ( interleave(i,j) || interleave(j,i) ) && oppo(i,j) )
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
     vector<Point> temp_pnts,temp_normal;
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

void Dissection::interbreak(int n1,int n2,vector<vector<Point>>& mark)
{
     Real k1,k2,x1,x2,y1,y2,x,y;
     bool jud;
     if(normal[n1].Y() < EPSILON && normal[n1].Y() > (-1.0) * EPSILON)
     {
          if( ShellPnts[n1 + 1].Y() > min( ShellPnts[n2].Y() , ShellPnts[n2 + 1].Y() ) + EPSILON  && ShellPnts[n1 + 1].Y() < max( ShellPnts[n2].Y() , ShellPnts[n2 + 1].Y() ) - EPSILON )
          {
               jud = true;
               for(int i = 0;i < mark[n2].size();i ++)
               if(equal( ShellPnts[n2].X() , mark[n2][i].X() ) && equal( ShellPnts[n1 + 1].Y() , mark[n2][i].Y() ) )
               {
                    jud = false;
                    break;
               }
               if(jud)
               {
                    Point now;
                    now = Point (ShellPnts[n2].X() , ShellPnts[n1 + 1].Y() , 0.0);
                    mark[n2].push_back(now);
               }
          }
          if( ShellPnts[n1].Y() > min( ShellPnts[n2].Y() , ShellPnts[n2 + 1].Y() ) + EPSILON  && ShellPnts[n1].Y() < max( ShellPnts[n2].Y() , ShellPnts[n2 + 1].Y() ) - EPSILON )
          {
               jud = true;
               for(int i = 0;i < mark[n2].size();i ++)
               if(equal(ShellPnts[n2].X() , mark[n2][i].X()) && equal(ShellPnts[n1].Y() , mark[n2][i].Y()) )
               {
                    jud = false;
                    break;
               }
               if(jud)
               {
                    Point now;
                    now = Point (ShellPnts[n2].X() , ShellPnts[n1].Y() , 0.0);
                    mark[n2].push_back(now);
               }
          }
          return ;
     }
     if(normal[n1].X() < EPSILON && normal[n1].X() > (-1.0) * EPSILON && normal[n2].X() < EPSILON && normal[n2].X() > (-1.0) * EPSILON)
     {
          if( ShellPnts[n1 + 1].X() > min( ShellPnts[n2].X() , ShellPnts[n2 + 1].X() ) + EPSILON && ShellPnts[n1 + 1].X() < max( ShellPnts[n2].X() , ShellPnts[n2 + 1].X() ) - EPSILON )
          {
               jud = true;
               for(int i = 0;i < mark[n2].size();i ++)
               if( equal(ShellPnts[n1 + 1].X() , mark[n2][i].X()) && equal(ShellPnts[n2].Y() , mark[n2][i].Y()) )
               {
                    jud = false;
                    break;
               }
               if(jud)
               {
                    Point now;
                    now = Point (ShellPnts[n1 + 1].X() , ShellPnts[n2].Y() , 0.0);
                    mark[n2].push_back(now);
               }
          }
          if( ShellPnts[n1].X() > min( ShellPnts[n2].X() , ShellPnts[n2 + 1].X() ) + EPSILON && ShellPnts[n1].X() < max( ShellPnts[n2].X() , ShellPnts[n2 + 1].X() ) - EPSILON )
          {
               jud = true;
               for(int i = 0;i < mark[n2].size();i ++)
               if( equal(ShellPnts[n1].X() , mark[n2][i].X()) && equal(ShellPnts[n2].Y() , mark[n2][i].Y()) )
               {
                    jud = false;
                    break;
               }
               if(jud)
               {
                    Point now;
                    now = Point (ShellPnts[n1].X() , ShellPnts[n2].Y() , 0.0);
                    mark[n2].push_back(now);
               }
          }
          return ;
     }
     
     k1 = Real(normal[n1].Y()) / Real(normal[n1].X());
     k2 = (-1.0) / k1;

     x1 = ShellPnts[n1 + 1].X();
     y1 = ShellPnts[n1 + 1].Y();
     x2 = ShellPnts[n2].X();
     y2 = ShellPnts[n2].Y();
     x = (k1 * x1 - y1 - k2 * x2 + y2) / (k1 - k2);
     y = k2 * x - k2 * x2 + y2;

     if( ( x > min(ShellPnts[n2].X(),ShellPnts[n2 + 1].X()) + EPSILON && 
          x < max(ShellPnts[n2].X(),ShellPnts[n2 + 1].X()) - EPSILON ) || 
          ( y > min(ShellPnts[n2].Y(),ShellPnts[n2 + 1].Y()) + EPSILON &&
          y < max(ShellPnts[n2].Y(),ShellPnts[n2 + 1].Y() - EPSILON) ) )
     {
          jud = true;
          for(int i = 0;i < mark[n2].size();i ++)
          if( equal(x , mark[n2][i].X()) && equal(y , mark[n2][i].Y() ) )
          {
               jud = false;
               break;
          }
          if(jud)
          {
               Point now;
               now = Point (x , y , 0.0);
               
               mark[n2].push_back(now);
          }
     }
     x1 = ShellPnts[n1].X();
     y1 = ShellPnts[n1].Y();
     x2 = ShellPnts[n2].X();
     y2 = ShellPnts[n2].Y();
     x = (k1 * x1 - y1 - k2 * x2 + y2) / (k1 - k2);
     y = k2 * x - k2 * x2 + y2;
     
     if( ( x > min(ShellPnts[n2].X(),ShellPnts[n2 + 1].X()) + EPSILON && 
          x < max(ShellPnts[n2].X(),ShellPnts[n2 + 1].X()) - EPSILON ) ||
          ( y > min(ShellPnts[n2].Y(),ShellPnts[n2 + 1].Y()) + EPSILON &&
           y < max(ShellPnts[n2].Y(),ShellPnts[n2 + 1].Y() - EPSILON ) ) )
     {
          jud = true;
          for(int i = 0;i < mark[n2].size();i ++)
          if( equal(x , mark[n2][i].X()) && equal(y , mark[n2][i].Y()) )
          {
               jud = false;
               break;
          }
          if(jud)
          {
               Point now;
               now = Point (x , y , 0.0);
               
               mark[n2].push_back(now);
          }
     }
}

void Dissection::close(vector<Point>& temp)
{
     if( ! ( temp[temp.size() - 1] == temp[0] ) )
     temp.push_back(temp[0]);
}

void Dissection::rec_split()
{
     std::stack<std::vector<Point>> tool; 
     tool.push(ShellPnts);
     int cnt = 0;
     vector<vector<Point>> last_res;
     std::vector<Point> temp,now;
     while(!tool.empty())
     {
          Real dist,min_dist;
          int mark_i,mark_j;
          bool jud = false;
          temp.clear();  now.clear();
          temp = tool.top();
          close(temp);
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
                    // last_res = postprocess(now);
                    // for(int j = 0;j < last_res.size();j ++)
                    // {
                    //      close(last_res[j]);
                    //      res.push_back(last_res[j]);
                    //      types.push_back(0);
                    // }
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
                    if( temp[i].X() - temp[i - 1].X() < EPSILON && temp[i].X() - temp[i - 1].X() > -EPSILON )
                    {
                         if(temp[i + 1].X() - temp[i].X() < EPSILON && temp[i + 1].X() - temp[i].X() > -EPSILON)
                         {
                              temp.erase(temp.begin() + i);
                              i --;
                         }
                         else
                         continue;
                    }
                    else if(temp[i + 1].X() - temp[i].X() < EPSILON && temp[i + 1].X() - temp[i].X() > -EPSILON)
                    continue;
                    else if(temp[i] == temp[i - 1])
                    {
                         temp.erase(temp.begin() + i);
                         i --;
                    }
                    else
                    {
                         k1 = ( temp[i].Y() - temp[i - 1].Y() ) / ( temp[i].X() - temp[i - 1].X() );
                         k2 = ( temp[i + 1].Y() - temp[i].Y() ) / ( temp[i + 1].X() - temp[i].X() );
                         if( k1 <= k2 + EPSILON && k1 >= k2 - EPSILON )
                         {
                              temp.erase(temp.begin() + i);
                              i --;
                         }
                    }
               }
               int len = temp.size();
               if( temp[1].X() - temp[0].X() < EPSILON && temp[1].X() - temp[0].X() > -EPSILON )
               {
                    if( temp[len - 1].X() - temp[len - 2].X() < EPSILON && temp[len - 1].X() - temp[len - 2].X() > -EPSILON )
                    {
                         temp.erase(temp.begin());
                         temp[temp.size() - 1] = temp[0];
                    }
               }
               else if(temp[len - 1].X() - temp[len - 2].X() < EPSILON && temp[len - 1].X() - temp[len - 2].X() > -EPSILON);
               else
               {
                    k1 = ( temp[1].Y() - temp[0].Y() ) / ( temp[1].X() - temp[0].X() );
                    k2 = ( temp[len - 1].Y() - temp[len - 2].Y() ) / ( temp[len - 1].X() - temp[len - 2].X() );
                    if( k1 <= k2 + EPSILON && k1 >= k2 - EPSILON )
                    {
                         temp.erase(temp.begin());
                         temp[temp.size() - 1] = temp[0];
                    }

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

vector<vector<Point>> Dissection::postprocess(vector<Point>& raw)
{
     bool mark = false;
     vector<vector<Point>> now_res,next_res;
     vector<Point> temp;
     Point np;
     for(int i = 0;i < raw.size();i ++)
     {
          std::cout << "i = " << i << std::endl;
          for(int j = 0;j < ShellPnts.size();j ++)
          {
               if( point_on_edge( raw[i] , raw[(i + 1) % 4] , ShellPnts[j] ) )
               {
                    mark = true;

                    if( ( raw[i] - raw[(i + 1) % 4] ).X() < EPSILON && ( raw[i] - raw[(i + 1) % 4] ).X() > -EPSILON )
                    np = Point( raw[(i + 2) % 4].X() , ShellPnts[j].Y() , 0.0 );
                    else if( ( raw[i] - raw[(i + 1) % 4] ).Y() < EPSILON && ( raw[i] - raw[(i + 1) % 4] ).Y() > -EPSILON )
                    np = Point( ShellPnts[j].X() , raw[(i + 2) % 4].Y() , 0.0 );
                    else
                    {
                         Real k = ( raw[(i + 1) % 4] - raw[i] ).Y() / ( raw[(i + 1) % 4] - raw[i] ).X();
                         Real b1 = ShellPnts[j].Y() + ShellPnts[j].X() / k;
                         Real b2 = raw[(i + 2) % 4].Y() - raw[(i + 2) % 4].X() * k;
                         Real x = k * (b2 - b1) / (k * k + 1);
                         Real y = k * x + b2;
                         np = Point( x , y , 0.0 );
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
     // cout << ShellPnts[i].X() << " " << ShellPnts[i].Y() << endl;
     cout << "now have " << normal.size() << " normals" << endl;
     for(int i = 0;i < normal.size();i ++)
     cout << i << ": " << normal[i].X() << " " << normal[i].Y() << endl;
}

void Dissection::record(vector<Point> external)
{
     MyMesh mesh;
     close(external);
     vcg::tri::Allocator<MyMesh>::AddVertices(mesh, external.size() - 1);
     vcg::tri::Allocator<MyMesh>::AddEdges(mesh, external.size() - 1);
     for(int i = 0;i < external.size() - 1;i ++)
     {
          mesh.vert[i].P().X() = external[i].X().toDouble();
          mesh.vert[i].P().Y() = external[i].Y().toDouble();
          mesh.vert[i].P().Z() = 0.0;
     }
     for(int i = 0;i < external.size() - 2;i ++)
     {
          mesh.edge[i].V(0) = &mesh.vert[i];
          mesh.edge[i].V(1) = &mesh.vert[i + 1];
     }
     mesh.edge[external.size() - 2].V(0) = &mesh.vert[external.size() - 2];
     mesh.edge[external.size() - 2].V(1) = &mesh.vert[0];
     mesh.UpdateDataStructure();
     string s = "../source/ply_source/" + filename.substr(0,filename.size() - 4) + "_original.ply";
     WritePLY(mesh, s);
}

void Dissection::record(vector<vector<Point>> external)
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
          {
               mesh.vert[shift + j].P().X() = external[i][j].X().toDouble();
               mesh.vert[shift + j].P().Y() = external[i][j].Y().toDouble();
               mesh.vert[shift + j].P().Z() = 0.0;
          }
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

void Dissection::record(vector<Point> external , string s)
{
     MyMesh mesh;
     close(external);
     vcg::tri::Allocator<MyMesh>::AddVertices(mesh, external.size() - 1);
     vcg::tri::Allocator<MyMesh>::AddEdges(mesh, external.size() - 1);
     for(int i = 0;i < external.size() - 1;i ++)
     {
          mesh.vert[i].P().X() = external[i].X().toDouble();
          mesh.vert[i].P().Y() = external[i].Y().toDouble();
          mesh.vert[i].P().Z() = 0.0;
     }
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

std::vector< std::vector<Point> > Dissection::Result()
{
     preprocess();
     derepeat();
     int len = ShellPnts.size();
     if(len == 0)
     return res;
     interpolate();
     record(ShellPnts , "processing.ply");
     rec_split();
     record(res);
     return res;
}

std::vector<bool> Dissection::Types()
{
     return types;
}