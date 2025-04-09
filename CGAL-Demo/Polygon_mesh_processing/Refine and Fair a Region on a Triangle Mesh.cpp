#include<CGAL/Exact_predicates_exact_constructions_kernel.h>
#include<CGAL/Polyhedron_3.h>
#include<CGAL/Polygon_mesh_processing/refine.h>
#include<CGAL/Polygon_mesh_processing/fair.h>
#include<CGAL/Polygon_mesh_processing/IO/polygon_mesh_io.h>

#include<iostream>
#include<iterator>
#include<map>
#include<vector>

typedef CGAL::Exact_predicates_inexact_constructions_kernel Kernel;
typedef CGAL::Polyhedron_3<Kernel> Polyhedron;
typedef Polyhedron::Vertex_handle  Vertex_handle;

namespace PMP=CGAL::Polygon_mesh_processing;

void extract_k_ring(Vertex_handle& v,int k,std::vector<Vertex_handle>& qv){
    std::map<Vertex_handle,int> D;
    qv.push_back(v);
    D[v]=0;

    std::size_t current_index=0;
    int dist_v;
    while(current_index<qv.size()&&(dist_v=D[qv[current_index]])<k){
        //获取当前定点
        v=qv[current_index++];
        // 定义一个半边缘循环器，用于遍历当前顶点的所有邻接边
        Polyhedron::Halfedge_around_vertex_circulator e(v->vertex_begin()),e_end(e);
        do{
            Vertex_handle new_v=e->opposite()->vertex();
            if(D.insert(std::make_pair(new_v,dist_v+1)).second){
                qv.push_back(new_v);
            }
        }while(++e!=e_end);
    }
}
