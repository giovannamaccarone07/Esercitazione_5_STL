#include "Utils.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <vector>
#include <limits>
#include <string>
#include "PolygonalMesh.hpp"
#include "Eigen/Eigen"


namespace PolygonalLibrary {

bool ImportMesh(const string& filepath,
                PolygonalMesh& mesh)
{
    if(!ImportCell0Ds("Cell0Ds.csv", mesh))
    {
        return false;
    }
    else
    {
        cout << "Cell0D marker:" << endl;
        for(auto iteratore = mesh.Celle0DMarkers.begin(); iteratore != mesh.Celle0DMarkers.end(); iteratore++) //it è un iteratore
        {
            cout << "key:\t" << iteratore -> first << "\t values:";
            for(const unsigned int id : iteratore -> second)
                cout << "\t" << id;

            cout << endl;
        }

    }

    if(!ImportCell1Ds("Cell1Ds.csv", mesh))
    {
        return false;
    }
    else
    {
        cout << "Cell1D marker:" << endl;
        for(auto iteratore = mesh.Celle1DMarkers.begin(); iteratore != mesh.Celle1DMarkers.end(); iteratore++)
        {
            cout << "key:\t" << iteratore -> first << "\t values:";
            for(const unsigned int id : iteratore -> second)
                cout << "\t" << id;

            cout << endl;
        }

    }


    if(!ImportCell2Ds("Cell2Ds.csv", mesh))
    {
        return false;
    }
    else {
        cout << "Cell2D marker:" << endl;
        for(auto iteratore = mesh.Celle2DMarkers.begin(); iteratore != mesh.Celle2DMarkers.end(); iteratore++)
        {
            cout << "key:\t" << iteratore -> first << "\t values:";
            for(const unsigned int id : iteratore -> second)
                cout << "\t" << id;

            cout << endl;
        }
    }

    //TEST LUNGHEZZA LATI
    double tol = numeric_limits<double>::epsilon();

    for(const auto& i : mesh.Celle1DVertici)
    {
        Vector2d coordinata1 = mesh.Celle0DCoordinate[i(0)];
        Vector2d coordinata2 = mesh.Celle0DCoordinate[i(1)];
        if(abs(coordinata1(0)-coordinata2(0)) < tol && abs(coordinata1(1)-coordinata2(1)) < tol)
        {
            cerr << "Ci sono dei lati di lunghezza nulla." << endl;
            return false;
        }
        else
        {
            cout << "I lati hanno lunghezza non nulla." << endl;
        }
    }

    // TEST AREA
    for(const auto& lati : mesh.Celle2DLati)
    {
        unsigned int count = 0;

        for(unsigned int i = 0; i < lati.size(); i++)

            if(i != lati.size()-1)
            {
                //Estremi, coordinate e lunghezza primo lato:
                Vector2i estremiLato1 = mesh.Celle1DVertici[lati(i)];
                Vector2d coordinateEstremo1 = mesh.Celle0DCoordinate[estremiLato1(0)];
                Vector2d coordinateEstremo2 = mesh.Celle0DCoordinate[estremiLato1(1)];
                Vector2d lunghezzaLato1 = coordinateEstremo2-coordinateEstremo1;

                //Estremi, coordinate e lunghezza secondo lato:
                Vector2i estremiLato2 = mesh.Celle1DVertici[lati(i+1)];
                Vector2d coordinateEstremo3 = mesh.Celle0DCoordinate[estremiLato2(0)];
                Vector2d coordinateEstremo4 = mesh.Celle0DCoordinate[estremiLato2(1)];
                Vector2d lunghezzaLato2 = coordinateEstremo4-coordinateEstremo3;

                //Calcolo il prodotto vettoriale:
                double determinante = lunghezzaLato1.x() * lunghezzaLato2.y() - lunghezzaLato1.y() * lunghezzaLato2.x();
                if(abs(determinante) < tol)
                {
                    count = count + 1;
                    return false;
                }
            }
            else
            {
                //Estremi, coordinate e lunghezza primo lato:
                Vector2i estremiLato1 = mesh.Celle1DVertici[lati(i)];
                Vector2d coordinateEstremo1 = mesh.Celle0DCoordinate[estremiLato1(0)];
                Vector2d coordinateEstremo2 = mesh.Celle0DCoordinate[estremiLato1(1)];
                Vector2d lunghezzaLato1 = coordinateEstremo2-coordinateEstremo1;

                //Estremi, coordinate e lunghezza secondo lato:
                Vector2i estremiLato2 = mesh.Celle1DVertici[lati(0)];
                Vector2d coordinateEstremo3 = mesh.Celle0DCoordinate[estremiLato2(0)];
                Vector2d coord_point_C = mesh.Celle0DCoordinate[estremiLato2(1)];
                Vector2d lunghezzaLato2 = coord_point_C-coordinateEstremo3;

                //Calcolo il prodotto vettoriale:
                double cross_prod = lunghezzaLato1.x() * lunghezzaLato2.y() - lunghezzaLato1.y() * lunghezzaLato2.x();
                if(abs(cross_prod) < tol)
                {
                    count = count + 1;
                    return false;
                }
            }
        if(count == lati.size())
        {
            cerr << "La mesh contiene poligoni con area nulla." << endl;
            return false;
        }
    }

    return true;
}


bool ImportCell0Ds(const string &filename,
                   PolygonalMesh& mesh)
{

    ifstream file;
    file.open(filename);

    if(file.fail()){
        cerr << "Impossibile aprire il file contenente le celle 0D." << endl;
        return false;
    }

    list<string> listLines;
    string line;
    while (getline(file, line)){
        listLines.push_back(line);
    }


    listLines.pop_front();

    mesh.NumeroCelle0D = listLines.size();

    if (mesh.NumeroCelle0D == 0)
    {
        cerr << "La mesh non contiene celle 0D." << endl;
        return false;
    }

    mesh.Celle0DId.reserve(mesh.NumeroCelle0D);
    mesh.Celle0DCoordinate.reserve(mesh.NumeroCelle0D);

    for (const string& line : listLines)
    {
        istringstream converter(line);
        string b;
        unsigned int id, marker;
        Vector2d coord;

        converter >> id >> b >> marker >> b >> coord(0) >> b >> coord(1);

        mesh.Celle0DId.push_back(id);
        mesh.Celle0DCoordinate.push_back(coord);

        if( marker != 0)
        {
            auto ret = mesh.Celle0DMarkers.insert({marker, {id}});
            if(!ret.second)
                (ret.first)->second.push_back(id);
        }

    }
    file.close();

    return true;
}

bool ImportCell1Ds(const string &filename,
                   PolygonalMesh& mesh)
{

    ifstream file;
    file.open(filename);

    if(file.fail()){
        cerr << "Impossibile aprire il file contenente le celle 1D." << endl;
        return false;
    }

    list<string> listLines;
    string line;

    while (getline(file, line)){
        listLines.push_back(line);
    }

    listLines.pop_front();

    mesh.NumeroCelle1D = listLines.size();

    if (mesh.NumeroCelle1D == 0)
    {
        cerr << "La mesh non contiene celle 1D." << endl;
        return false;
    }

    mesh.Celle1DId.reserve(mesh.NumeroCelle1D);
    mesh.Celle1DVertici.reserve(mesh.NumeroCelle1D);

    for (const string& line : listLines)
    {
        istringstream converter(line);
        char b;
        unsigned int id, marker;
        Vector2i vertices;

        converter >> id >> b >> marker >> b >> vertices(0) >> b >> vertices(1);

        mesh.Celle1DId.push_back(id);
        mesh.Celle1DVertici.push_back(vertices);

        if( marker != 0)
        {
            auto ret = mesh.Celle1DMarkers.insert({marker, {id}});
            if(!ret.second)
                (ret.first)->second.push_back(id);
        }
    }

    file.close();

    return true;
}

bool ImportCell2Ds(const string &filename,
                   PolygonalMesh& mesh)
{

    ifstream file;
    file.open(filename);

    if(file.fail()){
        cerr << "Impossibile aprire il file contenente le celle 2D." << endl;
        return false;
    }

    list<string> listLines;
    string line;

    while (getline(file, line)){
        listLines.push_back(line);
    }

    listLines.pop_front();

    mesh.NumeroCelle2D = listLines.size();

    if (mesh.NumeroCelle2D == 0)
    {
        cerr << "La mesh non contiene celle 2D." << endl;
        return false;
    }

    mesh.Celle2DId.reserve(mesh.NumeroCelle2D);
    mesh.Celle2DVertici.reserve(mesh.NumeroCelle2D);
    mesh.Celle2DLati.reserve(mesh.NumeroCelle2D);


    for (const string& line : listLines)
    {
        istringstream converter(line);

        char b;
        unsigned int marker;
        unsigned int NumeroVertici;
        unsigned int NumeroLati;
        unsigned int id;



        VectorXi vertici;
        VectorXi lati;
        converter >> id >> b >> marker >> b >> NumeroVertici;

        vertici.resize(NumeroVertici);
        for(unsigned int i = 0; i < 3; i++)
            converter >> b >> vertici[i];


        converter >> b >> NumeroLati;
        lati.resize(NumeroLati);
        for(unsigned int i = 0; i < 3; i++)
            converter >> b >> lati[i];

        mesh.Celle2DId.push_back(id);
        mesh.Celle2DVertici.push_back(vertici);
        mesh.Celle2DLati.push_back(lati);

    }
    file.close();
    return true;
}

}
