#include <iostream>
#include "PolygonalMesh.hpp"
#include "Utils.hpp"

using namespace std;
using namespace Eigen;
using namespace PolygonalLibrary;

int main()
{
    PolygonalMesh mesh;

    string filepath = "Mesh";
    if(!ImportMesh(filepath, mesh))
    {
        cerr << "Errore nella mesh." << endl;
        return 1;
    }
    else
    {
        cout << "Mesh valida." << endl;
    }
    return 0;
}
