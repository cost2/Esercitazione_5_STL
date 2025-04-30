#include <iostream>
#include "PolygonalMesh.hpp"
#pragma once

using namespace std;

namespace PolygonalLibrary
{
bool ImportMesh(PolygonalMesh& mesh);

bool ImportCell0Ds(PolygonalMesh& mesh);

bool ImportCell1Ds(PolygonalMesh& mesh);

bool ImportCell2Ds(PolygonalMesh& mesh);   

bool CheckEdgeLenghts(PolygonalMesh& mesh);

bool CheckPolygonAreas(PolygonalMesh& mesh);

}