#include "LAR_STL.h"

LAR_STL::LAR_STL(const std::string& filename) : is_loaded_and_repaired(false) {
    is_loaded_and_repaired = load_and_repair(filename);
}

LAR_STL::~LAR_STL() {}

const Surface_mesh& LAR_STL::get_repaired_mesh() const {
    return mesh;
}

// 移除孤立顶点
void LAR_STL::remove_isolated_vertices() {
    std::vector<Surface_mesh::Vertex_index> to_removed; // 修正拼写错误
    for (auto v : mesh.vertices()) {
        if (mesh.is_isolated(v)) {
            to_removed.push_back(v);
        }
    }
    for (auto v : to_removed) {
        mesh.remove_vertex(v);
    }
}

// 流形验证
bool LAR_STL::is_manifold() {
    // 检查边是否为流形边（最多关联2个面）
    for (auto e : mesh.edges()) {
        std::size_t num_faces = 0;
        halfedge_descriptor h1 = halfedge(e, mesh);
        halfedge_descriptor h2 = opposite(h1, mesh);
        if (is_border(h1, mesh) == false) {
            ++num_faces;
        }
        if (is_border(h2, mesh) == false) {
            ++num_faces;
        }
        if (num_faces > 2) {
            std::cerr << "非流形边: 边关联了 " << num_faces << " 个面" << std::endl;
            return false;
        }
    }

    // 检查顶点是否为流形顶点（边构成闭合环路）
    for (auto v : mesh.vertices()) {
        if (PMP::is_non_manifold_vertex(v, mesh)) {
            std::cout << "顶点 " << v << " 是非流形顶点" << std::endl;
            return false;
        }
    }

    return true;
}

// 流形修复
void LAR_STL::manifold_repair() {
    // 复制非流形顶点
    std::vector<std::vector<Surface_mesh::Vertex_index>> duplicated_vertices;
    std::size_t new_vertices_nb = PMP::duplicate_non_manifold_vertices(mesh,
                                                                       PMP::parameters::output_iterator(
                                                                           std::back_inserter(duplicated_vertices)));
    std::cout << new_vertices_nb << " 个顶点已被添加以修复网格流形性" << std::endl;

    // 修复边界
    PMP::stitch_borders(mesh);

    // 处理自相交（此处可根据实际情况添加更合适的替代方案）
    // 目前暂时注释掉该功能
    // PMP::remove_self_intersections(mesh);
}

// //高级修复
// void LAR_STL::advanced_repair(Surface_mesh&mesh){
//     // 缝合微小缝隙（阈值0.1mm）
//     PMP::stitch_borders(mesh,0.1,CGAL::parameters::apply_per_connected_component(true));
// }

// 加载并修复 STL 文件
bool LAR_STL::load_and_repair(const std::string& filename) {
    std::ifstream input(filename, std::ios::binary);
    if (!input) {
        std::cerr << "错误：无法打开文件 " << filename << std::endl;
        return false;
    }

    if (!CGAL::IO::read_STL(input, mesh)) {
        std::cerr << "错误：STL 文件解析失败" << std::endl;
        return false;
    }

    std::cout << "=== 修复前状态 ===" << std::endl;
    std::cout << "顶点数: " << mesh.num_vertices() << std::endl;
    std::cout << "面片数: " << mesh.num_faces() << std::endl;

    remove_isolated_vertices();
    manifold_repair();

    std::cout << "\n=== 修复后状态 ===" << std::endl;
    std::cout << "有效顶点: " << mesh.num_vertices() << std::endl;
    std::cout << "有效面片: " << mesh.num_faces() << std::endl;

    return true;
}    

// 保存修复后的网格到文件
bool LAR_STL::save_repaired_mesh(const std::string& outfilename) const {
    if (CGAL::IO::write_STL(outfilename, mesh)) {
        std::cout << "\n修复结果已保存至：" << outfilename << std::endl;
        return true;
    } else {
        std::cerr << "保存文件 " << outfilename << " 失败。" << std::endl;
        return false;
     }
}