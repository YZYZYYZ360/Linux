// 引入精确谓词和不精确构造的内核，确保几何计算的准确性和效率
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
// 引入表面网格类，用于处理三维网格数据
#include <CGAL/Surface_mesh.h>
// 引入角度和面积平滑功能的头文件，提供网格平滑算法
#include <CGAL/Polygon_mesh_processing/angle_and_area_smoothing.h>
// 引入特征检测功能的头文件，用于检测网格中的尖锐边
#include <CGAL/Polygon_mesh_processing/detect_features.h>
// 引入多边形网格输入输出功能的头文件，用于读取和写入网格文件
#include <CGAL/Polygon_mesh_processing/IO/polygon_mesh_io.h>
// 引入输入输出流库，用于在控制台输出信息
#include <iostream>
// 引入字符串库，用于处理文件路径和命令行参数
#include <string>

// 定义使用的内核类型，精确谓词和不精确构造
typedef CGAL::Exact_predicates_inexact_constructions_kernel     K;
// 定义表面网格类型，使用上述内核的三维点
typedef CGAL::Surface_mesh<K::Point_3>                          Mesh;
// 定义边描述符类型，用于表示网格中的边
typedef boost::graph_traits<Mesh>::edge_descriptor              edge_descriptor;
// 定义命名空间别名，方便后续使用多边形网格处理模块
namespace PMP = CGAL::Polygon_mesh_processing;

int main(int argc, char** argv)
{
    // 确定输入文件的路径
    // 如果命令行提供了参数，则使用第一个参数作为文件名
    // 否则，使用 CGAL 提供的默认数据文件 "meshes/anchor_dense.off"
    const std::string filename = argc > 1 ? argv[1] : CGAL::data_file_path("meshes/anchor_dense.off");
    // 定义一个表面网格对象，用于存储读取的网格数据
    Mesh mesh;
    // 尝试从文件中读取多边形网格数据
    // 如果读取失败，输出错误信息并返回错误码 1
    if (!PMP::IO::read_polygon_mesh(filename, mesh))
    {
        std::cerr << "Invalid input." << std::endl;
        return 1;
    }

    // 定义一个属性映射类型，用于标记边是否为特征边（尖锐边）
    typedef boost::property_map<Mesh, CGAL::edge_is_feature_t>::type EIFMap;
    // 获取该属性映射
    EIFMap eif = get(CGAL::edge_is_feature, mesh);
    // 检测网格中两面夹角大于 60° 的边，并将这些边标记为特征边
    PMP::detect_sharp_edges(mesh, 60, eif);

    // 统计尖锐边的数量
    int sharp_counter = 0;
    // 遍历网格中的所有边
    for (edge_descriptor e : edges(mesh))
    {
        // 如果当前边被标记为特征边
        if (get(eif, e))
            // 尖锐边数量加 1
            ++sharp_counter;
    }
    // 输出尖锐边的数量
    std::cout << sharp_counter << " sharp edges" << std::endl;

    // 确定网格平滑的迭代次数
    // 如果命令行提供了第二个参数，则将其转换为整数作为迭代次数
    // 否则，使用默认值 10
    const unsigned int nb_iterations = (argc > 2) ? std::atoi(argv[2]) : 10;
    // 输出开始网格平滑的信息，包含迭代次数
    std::cout << "Smoothing mesh... (" << nb_iterations << " iterations)" << std::endl;

    // 调用角度和面积平滑函数，对网格进行平滑处理
    PMP::angle_and_area_smoothing(mesh,
        // 设置平滑的迭代次数
        CGAL::parameters::number_of_iterations(nb_iterations)
        // 不使用安全约束，允许所有可能的顶点移动操作
        .use_safety_constraints(false)
        // 传入之前标记的特征边映射，确保这些尖锐边在平滑过程中不被改变
        .edge_is_constrained_map(eif));

    // 将平滑后的网格写入文件 "mesh_smoothed.off"
    // stream_precision(17) 指定输出文件的精度为 17 位
    CGAL::IO::write_polygon_mesh("mesh_smoothed.off", mesh, CGAL::parameters::stream_precision(17));
    // 输出处理完成的信息
    std::cout << "Done!" << std::endl;
    // 返回成功退出码
    return EXIT_SUCCESS;
}    