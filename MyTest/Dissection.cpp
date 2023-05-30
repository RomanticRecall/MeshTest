/*
 * @Author: Master 251871605@qq.com
 * @Date: 2023-05-29 17:03:19
 * @LastEditors: Master 251871605@qq.com
 * @LastEditTime: 2023-05-30 16:36:57
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
          temp.P().Construct<double>( cos(theta) , sin(theta) , 0.0 );
          normal.push_back(temp);
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

double Dissection::parallel_distant(MyVertex p1,MyVertex p2,MyVertex p3,MyVertex p4)
{
     if(p2.P().X() - p1.P().X() < EPSIL && p2.P().X() - p1.P().X() > (-1.0) * EPSIL)
     return p3.P().X() > p1.P().X() ? p3.P().X() - p1.P().X() : p1.P().X() - p3.P().X();
     if(p2.P().Y() - p1.P().Y() < EPSIL && p2.P().Y() - p1.P().Y() > (-1.0) * EPSIL)
     return p3.P().Y() > p1.P().Y() ? p3.P().Y() - p1.P().Y() : p1.P().Y() - p3.P().Y();
     double k1,k2,x1,x2,y1,y2,x,y;
     k2 = ( p2.P().Y() - p1.P().Y() ) / ( p2.P().X() - p1.P().X() );
     k1 = (-1.0) / k2;
     x1 = p2.P().X();
     y1 = p2.P().Y();
     x2 = p3.P().X();
     y2 = p3.P().Y();
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

std::vector< std::vector<MyVertex> > Dissection::Result()
{
     preprocess();
     derepeat();
     if(ShellPnts.size() == 0)
     return res;
     interpolate();
     // rec_split();
     return res;
}