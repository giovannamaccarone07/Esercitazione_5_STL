#pragma once

#include <iostream>
#include "PolygonalMesh.hpp"
#include "Eigen/Eigen"

using namespace std;
using namespace Eigen;

namespace PolygonalLibrary {
// Importa la polygonal mesh e testa la correttezza
bool ImportMesh(const string &filepath,
                PolygonalMesh& mesh);
//Importa le caratteristiche delle Cell0D dal file Cell0Ds.csv
bool ImportCell0Ds(const string &filename,
                   PolygonalMesh& mesh);
//Importa le caratteristiche delle Cell1D dal file Cell0Ds.csv
bool ImportCell1Ds(const string &filename,
                   PolygonalMesh& mesh);
//Importa le caratteristiche delle Cell0D dal file Cell2Ds.csv
bool ImportCell2Ds(const string &filename,
                   PolygonalMesh& mesh);
}
