#ifndef LOAD_AND_REPAIR_STL
#define LOAD_AND_REPAIR_STL

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Surface_mesh.h>
#include <CGAL/IO/STL.h>
#include <CGAL/Polygon_mesh_processing/repair.h>
#include <CGAL/Polygon_mesh_processing/stitch_borders.h> 
#include <CGAL/Polygon_mesh_processing/IO/polygon_mesh_io.h>
#include <CGAL/boost/graph/iterator.h> 
#include <iostream>
#include <vector>

// 定义核心的几何内核（相当于数学计算的基础引擎）
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
// 定义三维网格数据结构
typedef CGAL::Surface_mesh<K::Point_3> Surface_mesh;
typedef boost::graph_traits<Surface_mesh>::edge_descriptor edge_descriptor;
typedef boost::graph_traits<Surface_mesh>::halfedge_descriptor halfedge_descriptor;

namespace PMP = CGAL::Polygon_mesh_processing;

class LAR_STL {
public:
    // 负责加载和修复 STL 文件
    LAR_STL(const std::string& filename);
    ~LAR_STL();

    // 获取修复后的网格
    const Surface_mesh& get_repaired_mesh() const;
    // 保存修复后的网格到文件
    bool save_repaired_mesh(const std::string& outfilename) const;
    // 流形验证
    bool is_manifold();

private:
    Surface_mesh mesh;
    bool is_loaded_and_repaired;

    // 移除孤立顶点
    void remove_isolated_vertices();
    // 加载并修复 STL 文件
    bool load_and_repair(const std::string& filename);
    // 流形修复
    void manifold_repair();
};
#endif    