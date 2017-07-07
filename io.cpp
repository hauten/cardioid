#include "io.h"

void printSurfVTK(Mesh *mesh, std::ostream &out){
   out <<
       "# vtk DataFile Version 3.0\n"
       "Generated by MFEM\n"
       "ASCII\n"
       "DATASET UNSTRUCTURED_GRID\n";
   
   int NumOfVertices=mesh->GetNV(); 
   int spaceDim=3;
   
    out << "POINTS " << NumOfVertices << " double\n";
    for (int i = 0; i < NumOfVertices; i++)
    {
       const double* coord=mesh->GetVertex(i);
       for(int j=0; j<spaceDim; j++){
           out << coord[j] << " ";
       }
       out << '\n';
    }
    
    int NumOfElements=mesh->GetNBE();
      int size = 0;
      for (int i = 0; i < NumOfElements; i++)
      {
         const Element *ele = mesh->GetBdrElement(i); 
         size += ele->GetNVertices() + 1;
      }
      
      out << "CELLS " << NumOfElements << ' ' << size << '\n';
      for (int i = 0; i < NumOfElements; i++)
      {
         const Element *ele = mesh->GetBdrElement(i);
         const int *v = ele->GetVertices();
         const int nv = ele->GetNVertices();
         out << nv;
         for (int j = 0; j < nv; j++)
         {
            out << ' ' << v[j];
         }
         out << '\n';
      } 
      
   out << "CELL_TYPES " << NumOfElements << '\n';
   for (int i = 0; i < NumOfElements; i++)
   {
      const Element *ele = mesh->GetBdrElement(i);
      int vtk_cell_type = 5;
      {
         switch (ele->GetGeometryType())
         {
            case Geometry::TRIANGLE:     vtk_cell_type = 5;   break;
            case Geometry::SQUARE:       vtk_cell_type = 9;   break;
            case Geometry::TETRAHEDRON:  vtk_cell_type = 10;  break;
            case Geometry::CUBE:         vtk_cell_type = 12;  break;
         }
      }

      out << vtk_cell_type << '\n';
   }
   
   // write attributes
   out << "CELL_DATA " << NumOfElements << '\n'
       << "SCALARS material int\n"
       << "LOOKUP_TABLE default\n";
   for (int i = 0; i < NumOfElements; i++)
   {
      const Element *ele = mesh->GetBdrElement(i);
      out << ele->GetAttribute() << '\n';
   }
   out.flush();   
      
}

void printSurf4SurfVTK(Mesh *mesh, std::ostream &out){
   out <<
       "# vtk DataFile Version 3.0\n"
       "Generated by MFEM\n"
       "ASCII\n"
       "DATASET UNSTRUCTURED_GRID\n";
   
   int NumOfVertices=mesh->GetNV(); 
   int spaceDim=3;
   
    out << "POINTS " << NumOfVertices << " double\n";
    for (int i = 0; i < NumOfVertices; i++)
    {
       const double* coord=mesh->GetVertex(i);
       for(int j=0; j<spaceDim; j++){
           out << coord[j] << " ";
       }
       out << '\n';
    }
    
    int NumOfElements=mesh->GetNE();
      int size = 0;
      for (int i = 0; i < NumOfElements; i++)
      {
         const Element *ele = mesh->GetElement(i); 
         size += ele->GetNVertices() + 1;
      }
      
      out << "CELLS " << NumOfElements << ' ' << size << '\n';
      for (int i = 0; i < NumOfElements; i++)
      {
         const Element *ele = mesh->GetElement(i);
         const int *v = ele->GetVertices();
         const int nv = ele->GetNVertices();
         out << nv;
         for (int j = 0; j < nv; j++)
         {
            out << ' ' << v[j];
         }
         out << '\n';
      } 
      
   out << "CELL_TYPES " << NumOfElements << '\n';
   for (int i = 0; i < NumOfElements; i++)
   {
      const Element *ele = mesh->GetElement(i);
      int vtk_cell_type = 5;
      {
         switch (ele->GetGeometryType())
         {
            case Geometry::TRIANGLE:     vtk_cell_type = 5;   break;
            case Geometry::SQUARE:       vtk_cell_type = 9;   break;
            case Geometry::TETRAHEDRON:  vtk_cell_type = 10;  break;
            case Geometry::CUBE:         vtk_cell_type = 12;  break;
         }
      }

      out << vtk_cell_type << '\n';
   }
   
   // write attributes
   out << "CELL_DATA " << NumOfElements << '\n'
       << "SCALARS material int\n"
       << "LOOKUP_TABLE default\n";
   for (int i = 0; i < NumOfElements; i++)
   {
      const Element *ele = mesh->GetElement(i);
      out << ele->GetAttribute() << '\n';
   }
   out.flush();   
      
}

void printFiberVTK(Mesh *mesh, vector<Vector>& fiber_vecs, std::ostream &out){
   out <<
       "# vtk DataFile Version 3.0\n"
       "Generated by MFEM\n"
       "ASCII\n"
       "DATASET UNSTRUCTURED_GRID\n";
   
   int NumOfVertices=mesh->GetNV(); 
   int spaceDim=3;
   
    out << "POINTS " << NumOfVertices << " double\n";
    for (int i = 0; i < NumOfVertices; i++)
    {
       const double* coord=mesh->GetVertex(i);
       for(int j=0; j<spaceDim; j++){
           out << coord[j] << " ";
       }
       out << '\n';
    }
    
    int NumOfElements=mesh->GetNE();
      int size = 0;
      for (int i = 0; i < NumOfElements; i++)
      {
         const Element *ele = mesh->GetElement(i); 
         size += ele->GetNVertices() + 1;
      }
      
      out << "CELLS " << NumOfElements << ' ' << size << '\n';
      for (int i = 0; i < NumOfElements; i++)
      {
         const Element *ele = mesh->GetElement(i);
         const int *v = ele->GetVertices();
         const int nv = ele->GetNVertices();
         out << nv;
         for (int j = 0; j < nv; j++)
         {
            out << ' ' << v[j];
         }
         out << '\n';
      } 
      
   out << "CELL_TYPES " << NumOfElements << '\n';
   for (int i = 0; i < NumOfElements; i++)
   {
      const Element *ele = mesh->GetElement(i);
      int vtk_cell_type = 5;
      {
         switch (ele->GetGeometryType())
         {
            case Geometry::TRIANGLE:     vtk_cell_type = 5;   break;
            case Geometry::SQUARE:       vtk_cell_type = 9;   break;
            case Geometry::TETRAHEDRON:  vtk_cell_type = 10;  break;
            case Geometry::CUBE:         vtk_cell_type = 12;  break;
         }
      }

      out << vtk_cell_type << '\n';
   }
   
   // write point data
   MFEM_ASSERT(fiber_vecs.size()==(unsigned)NumOfVertices, "Size of fiber vector should equal to size of points");
   out << "POINT_DATA " << NumOfVertices<< "\n"
       << "VECTORS fiber double\n";
    for (int i = 0; i < NumOfVertices; i++) {
        fiber_vecs[i].Print(out, 10);
    }
   
   out << "\n";
               
   // write attributes
   out << "CELL_DATA " << NumOfElements << '\n'
       << "SCALARS material int\n"
       << "LOOKUP_TABLE default\n";
   for (int i = 0; i < NumOfElements; i++)
   {
      const Element *ele = mesh->GetElement(i);
      out << ele->GetAttribute() << '\n';
   }
   out.flush();   
      
}



void printAnatomy(vector<anatomy>& anatVectors, filerheader& header, std::ostream &out){
    // Print out the anatomy file header
    out << "anatomy FILEHEADER { \n" 
        << "datatype = VARRECORDASCII;\n"
        << "nfiles = 1;  \n"
        << "nrecord = " << header.nrecord << "; \n"
        << "   nfields = 1; \n"
        << "   lrec = 80; \n"
        << "   endian_key = 875770417; \n"
        << "   field_names = gid cellType sigma11 sigma12 sigma13 sigma22 sigma23 sigma33; \n"
        << "   field_types = u u f f f f f f; \n"
        << "   field_units = 1 1 mS/mm mS/mm mS/mm mS/mm mS/mm mS/mm; \n"
        << "   nx =  "<< header.nx<<"; \n"
        << "   ny = "<< header.ny<<"; \n"
        << "   nz = "<< header.nz<<"; \n"
        << "   dx = "<< header.dx<<"; \n"
        << "   dy = "<< header.dy<<"; \n"
        << "   dz = "<< header.dz<<"; \n"
        << "} \n\n";
    
    for(unsigned i=0; i<anatVectors.size(); i++){
        anatomy anat=anatVectors[i];
        out << "   "  << anat.gid << " " << anat.celltype << " ";        
        for(int j=0; j<6; j++){
            out << anat.sigma[j] << " ";    
        }
                    
        out << endl;
    }
            
}
