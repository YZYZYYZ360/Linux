#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include<CGAL/IO/STL.h>
#include<CGAL/Surface_mesh.h>
//#include<CGAL/draw_triangulation_2.h>
#include <iostream>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K> Delaunay;
typedef K::Point_2 Point;

// 新增STL加载函数
void load_stl_demo(){
    CGAL::Surface_mesh<K::Point_3> mesh;

    // 打开STL文件
    std::ifstream input("/home/yyz/Model_STL/xyzCalibration_cube.stl",std::ios::binary);
    if(!input){
        std::cerr<<("无法打开STL文件！")<<std::endl;
        return;
    }

    std::cout<<"STL顶点数: "<<mesh.num_vertices()<<std::endl;
    std::cout<<"STL面片数: "<<mesh.num_faces()<<std::endl;
}

int main(){
    load_stl_demo();
    // 生成随机点（模拟3D打印的切片轮廓点）
    std::vector<Point> points={
        Point(0,0),Point(1,0),Point(0,1),Point(1,1),Point(0.5,0.5)
    };

    Delaunay triangulation;
    triangulation.insert(points.begin(),points.end());

    // 输出三角剖分结果（可用于路径规划）
    std::cout<<"生成的三角形数量："<<triangulation.number_of_faces()<<std::endl;
    for(auto face=triangulation.faces_begin();face!=triangulation.faces_end();++face) {
        if(!triangulation.is_infinite(face)){
            auto p1=face->vertex(0)->point();
            auto p2=face->vertex(1)->point();
            auto p3=face->vertex(2)->point();
            std::cout << "三角形顶点: " 
                      << "(" << p1.x() << "," << p1.y() << "), "
                      << "(" << p2.x() << "," << p2.y() << "), "
                      << "(" << p3.x() << "," << p3.y() << ")\n";
        }
    }

    return 0;
}

