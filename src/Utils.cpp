#include "Utils.hpp"
#include "UCDUtilities.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include "Eigen/Eigen"
#include <cmath>
#include <algorithm>
#include <string>
#include <vector>

namespace PolygonalLibrary

{
	
bool ImportMesh(PolygonalMesh& mesh)
    {
        unsigned int numFilesImported = 0;
        if (ImportCell0Ds(mesh)) {numFilesImported ++;}
        if (ImportCell1Ds(mesh)) {numFilesImported ++;}
        if (ImportCell2Ds(mesh)) {numFilesImported ++;}

        return numFilesImported == 3;
    }
	

bool ImportCell0Ds(PolygonalMesh& mesh)
{
    ifstream InputCell0Ds("./Cell0Ds.csv");
    if (!InputCell0Ds.is_open())
    {
        cerr << "Errore nell'apertura del file Cell0Ds.csv" << endl;
        return false;
    }
    string line;
    list<string> size;
        while (getline(InputCell0Ds, line))
    {
        size.push_back(line);
    }
    InputCell0Ds.close();
    size.pop_front();
    mesh.NumCell0Ds = size.size();

    if (mesh.NumCell0Ds == 0)
    {
        cerr << "Non ci sono cell 0D" << endl;
        return false;
    }
    mesh.Cell0DCoordinates = Eigen::MatrixXd::Zero(3, mesh.NumCell0Ds); 

    for (string& list : size) 
    {
        replace(list.begin(), list.end(), ';', ' ');
        istringstream streaminput(list);
        unsigned int id;
		unsigned int marker;
        streaminput >> id >> marker >> mesh.Cell0DCoordinates(0,id) >> mesh.Cell0DCoordinates(1,id) ;
    if (marker != 0)
    {
        auto j = mesh.MarkerCell0Ds.find(marker);
        if (j != mesh.MarkerCell0Ds.end())
              (*j).second.push_back(id);
        else
            mesh.MarkerCell0Ds.insert({marker, {id}});
        }

    }
return true;
}


bool ImportCell1Ds(PolygonalMesh& mesh)
    {
        {
            ifstream inputCell1Ds("./Cell1Ds.csv");
            if (!inputCell1Ds.is_open())
            {
                cerr << "Errore nell'apertura del file Cell1Ds.csv" << endl;
                return false;
            }
            string line;
            list<string> size;
            while (getline(inputCell1Ds, line))
            {
                size.push_back(line);
            }
            inputCell1Ds.close();
            size.pop_front();
            mesh.NumCell1Ds = size.size();
            if (mesh.NumCell1Ds == 0)
            {
                cerr << "Non ci sono cell 1D" << endl;
                return false;
            }
            mesh.Cell1DVertices = MatrixXi::Zero(2, mesh.NumCell1Ds); ; 
            for (string& list : size) 
            {
              replace(list.begin(), list.end(), ';', ' ');
              istringstream streaminput(list);
              unsigned int id,marker;
              streaminput >> id >> marker >> mesh.Cell1DVertices(0,id)>> mesh.Cell1DVertices(1,id); 
              if (marker != 0)
              {
                auto j = mesh.MarkerCell1Ds.find(marker);
                if (j != mesh.MarkerCell1Ds.end())
                    (*j).second.push_back(id);
                else
                    mesh.MarkerCell1Ds.insert({marker, {id}});
              }
            } 
        }
        return true;
    }   

bool ImportCell2Ds(PolygonalMesh& mesh)
    {
        ifstream file_Cell2Ds("./Cell2Ds.csv");
        if (!file_Cell2Ds.is_open())
        {
            cerr << "Errore nell'apertura del file Cell2Ds.csv" << endl;
            return false;
        }
        string line;
        list<string> size;
        while (getline(file_Cell2Ds, line))
        {
            size.push_back(line);
        }
        file_Cell2Ds.close();
        size.pop_front();
        mesh.NumCell2Ds = size.size();
        if (mesh.NumCell2Ds == 0)
        {
            cerr << "Non ci sono cell 2D" << endl;
            return false;
        }

        for (string& list : size)
        
        {
          replace(list.begin(), list.end(), ';', ' ');
          istringstream streaminput(list);
          unsigned int id,marker,NumCell0DsCell1Ds;
          streaminput >> id >> marker >> NumCell0DsCell1Ds;
          if (marker != 0)
              {
                auto j = mesh.MarkerCell2Ds.find(marker);
                if (j != mesh.MarkerCell2Ds.end())
                    (*j).second.push_back(id);
                else
                    mesh.MarkerCell2Ds.insert({marker, {id}});
              }

          mesh.Cell2DVertices.reserve(mesh.NumCell2Ds);
          vector<int> vectorline;
          vectorline.resize(NumCell0DsCell1Ds);
          for (unsigned int j = 0; j < NumCell0DsCell1Ds ; j++)
          {
            streaminput >> vectorline[j];
          }
          mesh.Cell2DVertices.push_back(vectorline);
          
                 
          streaminput >> NumCell0DsCell1Ds;

          mesh.Cell2DEdges.reserve(mesh.NumCell2Ds);    
          vector<int> linevector;
          linevector.reserve(NumCell0DsCell1Ds);
          for (unsigned int j = 0; j < NumCell0DsCell1Ds ; j++)
          {
            streaminput >> linevector[j];
          }

          mesh.Cell2DEdges.push_back(linevector);
          
        }
        
    return true;
    }

bool CheckEdgeLenghts(PolygonalMesh& mesh){

    for (unsigned int i = 0; i < mesh.NumCell1Ds; i++)
    {
        unsigned int Cell0D1 = mesh.Cell1DVertices(0,i);
        unsigned int Cell0D2 = mesh.Cell1DVertices(1,i);
        double OneCoordinatesCell0D1 = mesh.Cell0DCoordinates(0,Cell0D1);
        double SecondCoordinatesCell0D1 = mesh.Cell0DCoordinates(1,Cell0D1);
        double OneCoordinatesCell0D2 = mesh.Cell0DCoordinates(0,Cell0D2);
        double SecondCoordinatesCell0D2 = mesh.Cell0DCoordinates(1,Cell0D2);
        double lenght_value = sqrt(pow(OneCoordinatesCell0D2 - OneCoordinatesCell0D1, 2) +pow(SecondCoordinatesCell0D2 - SecondCoordinatesCell0D1, 2));
        double tolerance = 1e-6; 
        if (lenght_value < tolerance)
        {
            return false;
        }
    }
    return true;

}

bool CheckPolygonAreas(PolygonalMesh& mesh){

    {
        for (unsigned int j = 0; j < mesh.NumCell2Ds; j++)
        {
        double Areas=0;
            for (unsigned int i = 0; i <  mesh.Cell2DVertices[j].size() ; i ++)
            {   
       
                int oneIndex = mesh.Cell2DVertices[j][i];
                int twoIndex;
                if (i + 1 == mesh.Cell2DVertices[j].size())
                {
                    twoIndex = mesh.Cell2DVertices[j][0];
                }
                else
                {
                    twoIndex = mesh.Cell2DVertices[j][i+1];
                }
                double OneCoordinatesCell0D1 = mesh.Cell0DCoordinates(0,oneIndex);
                double TwoCoordinatesCell0D1 = mesh.Cell0DCoordinates(1,oneIndex);
                double OneCoordinatesCell0D2 = mesh.Cell0DCoordinates(0,twoIndex);
                double TwoCoordinatesCell0D2 = mesh.Cell0DCoordinates(1,twoIndex);
                Areas += (OneCoordinatesCell0D1*TwoCoordinatesCell0D2 - TwoCoordinatesCell0D1*OneCoordinatesCell0D2);
            }
			
        Areas = abs(Areas)/2;
        double tolerance = 1e-8; 
		
        if (Areas < tolerance)
        {
        return false;
        }
        }
        
    
    return true;
    
    }

}

}