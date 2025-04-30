#include <iostream>
#include <map>
#include <vector>
#include "Eigen/Eigen"
#pragma once

using namespace std;
using namespace Eigen;

namespace PolygonalLibrary
{

    struct PolygonalMesh
	{
        unsigned int NumCell0Ds;
        unsigned int NumCell1Ds;
        unsigned int NumCell2Ds;

        map < unsigned int, list < unsigned int >> MarkerCell0Ds;
        map < unsigned int, list < unsigned int >> MarkerCell1Ds;
        map < unsigned int, list < unsigned int >> MarkerCell2Ds;

        Eigen::MatrixXd Cell0DCoordinates;
        Eigen::MatrixXi Cell1DVertices; 
        vector < vector < int >> Cell2DVertices;
        vector < vector < int >> Cell2DEdges;
    };

};
