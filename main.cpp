#include <iostream>
#include "PolygonalMesh.hpp"
#include "Utils.hpp"
#include "UCDUtilities.hpp"

using namespace std;
using namespace Eigen;
using namespace PolygonalLibrary;


int main()
{
    PolygonalMesh mesh;

    if(!ImportMesh(mesh))
    {
        cerr << "File non trovato" << endl;
        return 1;
    }
   
    cout << "Mesh importata" << endl;
   

    Gedim::UCDUtilities utilities;
    utilities.ExportPoints("./Cell0Ds.inp",
                            mesh.Cell0DCoordinates);
    utilities.ExportSegments("./Cell1Ds.inp",
                              mesh.Cell0DCoordinates,
                              mesh.Cell1DVertices);
                              
    if (!CheckEdgeLenghts(mesh))
    {
        cerr << "Errore: almeno un lato ha lughezza pari a zero" << endl;
        return 1;
    }
    
    cout << "Ogni bordo ha una lunghezza diversa da zero" << endl;
   
    
    if (!CheckPolygonAreas(mesh))
    {
        cerr << "Errore: almeno un poligono ha zero 'PolygonAreas'" << endl;
        return 1;
    }
    
    cout << "Ogni poligono ha 'PolygonAreas' diverse da zero" << endl;
    
    

return 0;

}