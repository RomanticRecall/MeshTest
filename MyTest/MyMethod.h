#ifndef MY_METHOD_H
#define MY_METHOD_H
#include "../vcglib/vcg/complex/algorithms/nring.h"
#include "BasicType.h"
#include "Function.h"

template<typename MeshType>
class MyMethod
{
    typedef typename MeshType::VertexType VertexType;

    public:

    static std::vector<VertexType *> MyNring(int n,MeshType *m,VertexType *v)
    {
        vcg::tri::Nring<MeshType> nring(v , m);
        nring.expand(n);
        return nring.allV;
    }

    static void MyUnifrom(MeshType &m , int n)
    {
        vcg::Point3d p;
        std::vector<VertexType *> onering;
        for(int it = 0;it < n;it ++)
        {
            for(int i = 0;i < m.vn;i ++){
                p[0] = 0.0; p[1] = 0.0; p[2] = 0.0;
                onering = MyNring(1 , &m , &m.vert[i]);
                for(int j = 0;j < onering.size();j ++)
                {
                    p[0] += onering[j]->P().V(0);
                    p[1] += onering[j]->P().V(1);
                    p[2] += onering[j]->P().V(2);
                }
                p[0] /= onering.size(); p[1] /= onering.size(); p[2] /= onering.size();
                // m.vert[i].P() = p;
                m.vert[i].P().X() = p[0];
                m.vert[i].P().Y() = p[1];
                m.vert[i].P().Z() = p[2];
            }
        }
    }

};


#endif