#include "Shape.h"

Shape::Shape(){}

Shape::~Shape(){}

Shape::Shape(std::vector<Point>& vertices , bool type):ShellPnts(vertices),type(type)
{
    vector<Point> temp;
    for(int i = 0; i < vertices.size() - 1;i ++)
    {
        temp.clear();
        temp.push_back(vertices[i]);
        temp.push_back(vertices[i + 1]);
        Edges.push_back(temp);
    }
}

void Shape::Info()
{
    std::cout << "this shape have " << ShellPnts.size() << " points" << std::endl;
    for(int i = 0;i < ShellPnts.size();i ++)
    std::cout << ShellPnts[i].X() << " " << ShellPnts[i].Y() << std::endl << std::endl;
}

void Shape::AdjInfo()
{
    int cnt = 0;
    for(auto it = AdjEdges.begin() ; it != AdjEdges.end() ; it ++)
    {
        cnt ++;
    }
    cout << "adjcent to " << cnt << " shapes" << std::endl << std::endl;
}

std::vector<std::vector<Point>> Shape::GetEdges()
{
    return Edges;
}

std::vector<Point> Shape::GetVertices()
{
    return ShellPnts;
}

bool Shape::AddAdjs(Shape * adjshape)
{
    bool tes = false;
    AdjShapes.push_back(adjshape);
    vector<int> tool;
    std::vector<Point> adjvs = adjshape->GetVertices();
    for(int i = 0;i < ShellPnts.size() - 1;i ++)
    {
        for(int j = 0;j < adjvs.size() - 1;j ++)
        {
            if(ShellPnts[i] == adjvs[j] && ShellPnts[i + 1] == adjvs[j + 1])
            {
                tes = true;
                if(AdjEdges.find(adjshape) == AdjEdges.end()){
                    tool.clear(); AdjEdges[adjshape] = tool;
                }
                else
                AdjEdges[adjshape].push_back(i);
                break;
            }
            else if(ShellPnts[i] == adjvs[j + 1] && ShellPnts[i + 1] == adjvs[j])
            {
                tes = true;
                if(AdjEdges.find(adjshape) == AdjEdges.end()){
                    tool.clear(); AdjEdges[adjshape] = tool;
                }
                else
                AdjEdges[adjshape].push_back(i);
                break;
            }
        }
    }
    return tes;
}

void Shape::interpolation(vector<Point> &edge,Real sz)
{
    Point p1 = edge[0] , p2 = edge[1];
    Point d = p1 - p2;
    Real distance = sqrt( ( d * d ).toDouble() );
    if(sz == 0.000000) return ;
    int sn = (int)(distance / sz).toDouble();
    if(sn == 0) return ;
    for(int i = 1;i < sn;i ++)
    edge.insert( edge.begin() + 1 , p2 + ( d / Real(sn) ) * (double)i );
}

void Shape::interpolation(vector<Point> &edge,int sn)
{

}

void Shape::Steiner(double sz)
{
    if(sz > 0.000000)
    for(int i = 0;i < Edges.size();i ++)
    interpolation(Edges[i] , Real(sz));
}

void Shape::Steiner(int sn)
{

}

bool Shape::Type()
{
    return type;
}