#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Surface_mesh.h>

#include <CGAL/Polygon_mesh_processing/triangulate_faces.h>
#include<CGAL/boost/graph/copy_face_graph.h>

#include<iostream>
#include<fstream>
#include<map>
#include<string>
#include<unordered_map>
#include<utility>

typedef CGAL::Exact_predicates_inexact_constructions_kernel Kernel;
typedef Kernel::Point_3                    Point;
typedef CGAL::Surface_mesh<Point>          Surface_mesh;
typedef boost::graph_traits<Surface_mesh>::face_descriptor face_descriptor;

class Insert_iterator{                      
    typedef std::unordered_map<face_descriptor,face_descriptor> Container;
    Container& container;        //相当于给这个Container自定义类容器添加了一个插入迭代器Insert_iterator

public:

    Insert_iterator(Container& c):container(c){};

    Insert_iterator& operator=(const std::pair<face_descriptor,face_descriptor>& p){
        container[p.second]=p.first;
        return *this;
    }

    Insert_iterator& operator*(){return *this;}

    Insert_iterator operator++(int){return *this;}  //加int是为了区分前后置递增，符合stl规范，后置递增返回副本，前置递增返回引用
};

struct Visitor:public CGAL::Polygon_mesh_processing::Triangulate_faces::Default_visitor<Surface_mesh>{
    typedef std::unordered_map<face_descriptor,face_descriptor> Container;

    Container& container;
    face_descriptor qfd;

    Visitor(Container& c):container(c){};

    void before_subface_creations(face_descriptor fd){
        std::cout << "split : " << fd << " into:" << std::endl;
    }

    void after_subface_creations(face_descriptor fd){
        std::cout<<" "<<fd;
        container[fd]=qfd;
    }

    void after_subface_creations()
    {
      std::cout << std::endl;
    }
};

int main(int argc,char**argv){
    const std::string filename=(argc>1)?argv[1]:CGAL::data_file_path("meshes/P.off");
    std::ifstream input(filename);

    Surface_mesh mesh;
    if (!input || !(input >> mesh) || mesh.is_empty())
    {
      std::cerr << "Not a valid off file." << std::endl;
      return 1;
    }

    std::unordered_map<face_descriptor,face_descriptor> t2q;

    Surface_mesh copy;

    CGAL::copy_face_graph(mesh,copy,CGAL::parameters::face_to_face_output_iterator(Insert_iterator(t2q)));

    Visitor v(t2q);
    CGAL::Polygon_mesh_processing::triangulate_faces(copy,CGAL::parameters::visitor(v));
    for(std::unordered_map<face_descriptor,face_descriptor>::iterator it = t2q.begin(); it != t2q.end(); ++it){
        std::cout << it->first << "  "  << it->second << std::endl;
    }
    return 0;
}