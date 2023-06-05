#include "Shape.h"

Shape::Shape(){}

Shape::~Shape(){}

Shape::Shape(std::vector<MyVertex>& vertices):ShellPnts(vertices){}

void Shape::Info()
{
    std::cout << "this shape have " << ShellPnts.size() << " points" << std::endl;
    for(int i = 0;i < ShellPnts.size();i ++)
    std::cout << ShellPnts[i].P().X() << " " << ShellPnts[i].P().Y() << std::endl << std::endl;
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

std::vector<MyVertex> Shape::GetVertices()
{
    return ShellPnts;
}

bool Shape::AddAdjs(Shape * adjshape)
{
    bool tes = false;
    AdjShapes.push_back(adjshape);
    vector<int> tool;
    std::vector<MyVertex> adjvs = adjshape->GetVertices();
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
            }
        }
    }
    return tes;
}