#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Surface_mesh.h>
#include <CGAL/Polygon_mesh_processing/detect_features.h>
#include <CGAL/Polygon_mesh_processing/surface_Delaunay_remeshing.h>
#include <CGAL/Polygon_mesh_processing/IO/polygon_mesh_io.h>
#include <CGAL/Mesh_constant_domain_field_3.h>
#include <fstream>

// 定义使用的内核，精确的谓词和不精确的构造
typedef CGAL::Exact_predicates_inexact_constructions_kernel   K;
// 定义表面网格类型，使用上述内核的三维点
typedef CGAL::Surface_mesh<K::Point_3>                        Mesh;
// 定义尺寸场类型，用于指定网格边长
typedef CGAL::Mesh_constant_domain_field_3<K, int> Sizing_field;
// 定义命名空间别名，方便后续使用多边形网格处理模块
namespace PMP = CGAL::Polygon_mesh_processing;

int main(int argc, char* argv[])
{
    // 确定输入文件名，如果没有提供命令行参数，则使用默认文件
    std::string filename = (argc > 1) ? std::string(argv[1])
        : CGAL::data_file_path("meshes/anchor_dense.off");
    // 定义一个表面网格对象
    Mesh mesh;
    // 读取输入文件到网格对象中，并检查是否为有效的三角网格
    if (!PMP::IO::read_polygon_mesh(filename, mesh) || !CGAL::is_triangle_mesh(mesh))
    {
        // 若输入无效，输出错误信息并返回错误码
        std::cerr << "Invalid input." << std::endl;
        return 1;
    }
    // 确定目标边长，如果没有提供命令行参数，则使用默认值
    double target_edge_length = (argc > 2) ? std::stod(std::string(argv[2])) : 0.02;
    // 创建尺寸场对象，设置目标边长
    Sizing_field size(target_edge_length);
    // 确定表面逼近距离，如果没有提供命令行参数，则使用默认值
    double fdist = (argc > 3) ? std::stod(std::string(argv[3])) : 0.01;

    // 输出提示信息，开始检测特征边
    std::cout << "Detect features..." << std::endl;
    // 定义边是否为特征边的属性映射类型
    using EIFMap = boost::property_map<Mesh, CGAL::edge_is_feature_t>::type;
    // 获取边是否为特征边的属性映射
    EIFMap eif = get(CGAL::edge_is_feature, mesh);
    // 检测网格中的尖锐边，角度阈值为45度
    PMP::detect_sharp_edges(mesh, 45, eif);

    // 输出提示信息，开始重新网格化
    std::cout << "Start remeshing of " << filename
        << " (" << num_faces(mesh) << " faces)..." << std::endl;
    // 进行表面Delaunay重新网格化，设置相关参数
    Mesh outmesh = PMP::surface_Delaunay_remeshing(mesh,
        CGAL::parameters::protect_constraints(true)
        .mesh_edge_size(size)
        .mesh_facet_distance(fdist)
        .edge_is_constrained_map(eif));
    // 输出提示信息，重新网格化完成
    std::cout << "Remeshing done." << std::endl;

    // 打开输出文件
    std::ofstream ofs("anchor_remeshed.off");
    // 将重新网格化后的网格写入输出文件
    ofs << outmesh;
    // 关闭输出文件
    ofs.close();

    return 0;
}    