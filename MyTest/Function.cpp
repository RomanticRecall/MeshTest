#include "Function.h"

Read::Read(){}

Read::Read(string s)
{
    MyMesh mesh;
    int loadmask = 0;
    string filename = s;
    vcg::tri::io::ImporterOBJ<MyMesh>::Open(mesh, filename.c_str(), loadmask);
}