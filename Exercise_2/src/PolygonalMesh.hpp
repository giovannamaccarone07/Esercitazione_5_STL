#pragma once

#include <iostream>
#include "Eigen/Eigen"

using namespace std;
using namespace Eigen;


namespace PolygonalLibrary {

struct PolygonalMesh
{
    //CELLE0D:
    unsigned int NumeroCelle0D = 0;
    //vector per memorizzare gli id:
    vector<unsigned int> Celle0DId = {};
    //vector per memorizzare le coordinate:
    vector<Vector2d> Celle0DCoordinate = {};
    //map per memorizzare i marker:
    map<unsigned int, list<unsigned int>> Celle0DMarkers = {};

    //CELLE1D:
    unsigned int NumeroCelle1D = 0;
    //vector per memorizzare gli id:
    vector<unsigned int> Celle1DId = {};
    //vector per memorizzare le coordinate:
    vector<Vector2i> Celle1DVertici = {};
    //map per memorizzare i marker:
    map<unsigned int, list<unsigned int>> Celle1DMarkers = {};

    //CELLE2D:
    unsigned int NumeroCelle2D = 0;
    //vector per memorizzare gli id:
    vector<unsigned int> Celle2DId = {};
    //vector per memorizzare i vertici:
    vector<VectorXi> Celle2DVertici = {};
    //vector per memorizzare i lati:
    vector<VectorXi> Celle2DLati = {};

};

}
