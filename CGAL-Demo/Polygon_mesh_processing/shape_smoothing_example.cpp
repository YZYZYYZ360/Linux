// 引入精确谓词和不精确构造的内核，确保几何计算的准确性和效率
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
// 引入表面网格类，用于处理三维网格数据
#include <CGAL/Surface_mesh.h>  
// 引入形状平滑功能的头文件，提供形状平滑算法
#include <CGAL/Polygon_mesh_processing/smooth_shape.h>
// 引入多边形网格输入输出功能的头文件，用于读取和写入网格文件
#include <CGAL/Polygon_mesh_processing/IO/polygon_mesh_io.h>
// 引入输入输出流库，用于在控制台输出信息
#include <iostream>
// 引入集合库，用于存储约束顶点的索引
#include <set>
// 引入字符串库，用于处理文件路径和命令行参数
#include <string>

// 定义使用的内核类型，精确谓词和不精确构造
typedef CGAL::Exact_predicates_inexact_constructions_kernel   K;
// 定义表面网格类型，使用上述内核的三维点
typedef CGAL::Surface_mesh<K::Point_3>                        Mesh;
// 定义命名空间别名，方便后续使用多边形网格处理模块
namespace PMP = CGAL::Polygon_mesh_processing;

int main(int argc, char* argv[])
{
    // 确定输入文件的路径
    // 如果命令行提供了参数，则使用第一个参数作为文件名
    // 否则，使用 CGAL 提供的默认数据文件 "meshes/pig.off"
    const std::string filename = (argc > 1) ? argv[1] : CGAL::data_file_path("meshes/pig.off");
    // 定义一个表面网格对象，用于存储读取的网格数据
    Mesh mesh;
    // 尝试从文件中读取多边形网格数据
    // 如果读取失败，输出错误信息并返回错误码 1
    if (!PMP::IO::read_polygon_mesh(filename, mesh))
    {
        std::cerr << "Invalid input." << std::endl;
        return 1;
    }

    // 确定形状平滑的迭代次数
    // 如果命令行提供了第二个参数，则将其转换为整数作为迭代次数
    // 否则，使用默认值 10
    const unsigned int nb_iterations = (argc > 2) ? std::atoi(argv[2]) : 10;
    // 确定形状平滑的时间步长
    // 如果命令行提供了第三个参数，则将其转换为双精度浮点数作为时间步长
    // 否则，使用默认值 0.0001
    const double time = (argc > 3) ? std::atof(argv[3]) : 0.0001;

    // 定义一个集合，用于存储约束顶点的索引
    std::set<Mesh::Vertex_index> constrained_vertices;
    // 遍历网格中的所有顶点
    for (Mesh::Vertex_index v : vertices(mesh))
    {
        // 判断当前顶点是否为边界顶点
        if (is_border(v, mesh))
            // 如果是边界顶点，则将其索引添加到约束顶点集合中
            constrained_vertices.insert(v);
    }
    // 输出约束的边界顶点数量
    std::cout << "Constraining: " << constrained_vertices.size() << " border vertices" << std::endl;

    // 创建一个布尔属性映射，用于标记顶点是否为约束顶点
    // 该映射使用约束顶点集合初始化
    CGAL::Boolean_property_map<std::set<Mesh::Vertex_index> > vcmap(constrained_vertices);

    // 输出开始形状平滑的信息，包含迭代次数
    std::cout << "Smoothing shape... (" << nb_iterations << " iterations)" << std::endl;
    // 调用形状平滑函数，对网格进行形状平滑处理
    // time 是时间步长，控制每次迭代的变化程度
    // number_of_iterations 是迭代次数，指定平滑过程的迭代次数
    // vertex_is_constrained_map 是顶点约束映射，指定哪些顶点是约束顶点，不会被移动
    PMP::smooth_shape(mesh, time, CGAL::parameters::number_of_iterations(nb_iterations)
                                                 .vertex_is_constrained_map(vcmap));

    // 将平滑后的网格写入文件 "mesh_shape_smoothed.off"
    // stream_precision(17) 指定输出文件的精度为 17 位
    CGAL::IO::write_polygon_mesh("mesh_shape_smoothed.off", mesh, CGAL::parameters::stream_precision(17));
    // 输出处理完成的信息
    std::cout << "Done!" << std::endl;
    // 返回成功退出码
    return EXIT_SUCCESS;
}    