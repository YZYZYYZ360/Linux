#include<CGAL/Exact_predicates_exact_constructions_kernel.h>
#include<CGAL/Surface_mesh.h>

#include<CGAL/Polygon_mesh_processing/triangulate_faces.h>
#include<CGAL/Polygon_mesh_processing/IO/polygon_mesh_io.h>

#include<CGAL/boost/graph/helpers.h>

#include<string>
#include<iostream>

typedef CGAL::Exact_predicates_inexact_constructions_kernel Kernel;
typedef Kernel::Point_3                                     Point;
typedef CGAL::Surface_mesh<Point>                           Surface_mesh;

namespace PMP=CGAL::Polygon_mesh_processing;

int main(int argc,char** argv){
    const std::string filename=(argc>1)?argv[1]:CGAL::data_file_path("meshs/P.off");
    const char* outfilename=(argc>2)?argv[2]:"P_tri.off";

    Surface_mesh mesh;
    if(!PMP::IO::read_polygon_mesh(filename,mesh)){
        std::cerr<<"Error: Invalid input."<<std::endl;
        return EXIT_FAILURE;
    }

    if(is_empty(mesh)){
        std::cerr<<"Warning: empty file?"<<std::endl;
        return EXIT_FAILURE;
    }

    if(!CGAL::is_triangle_mesh(mesh))
    std::cout << "Input mesh is not triangulated." << std::endl;
    else
    std::cout << "Input mesh is triangulated." << std::endl;

    PMP::triangulate_faces(mesh);

    for(boost::graph_traits<Surface_mesh>::face_descriptor f:faces(mesh)){
        if(!CGAL::is_triangle(halfedge(f,mesh),mesh))
            std::cerr<< "Error: non-triangular face left in mesh." << std::endl;
    }

    CGAL::IO::write_polygon_mesh(outfilename,mesh,CGAL::parameters::stream_precision(17));
    
    return EXIT_SUCCESS;
}
