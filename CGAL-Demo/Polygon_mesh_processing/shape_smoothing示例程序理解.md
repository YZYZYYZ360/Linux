
这段代码是使用 CGAL 库对三维网格进行 **形状平滑（Shape Smoothing）** 的示例，核心是通过**平均曲率流（Mean Curvature Flow）**算法平滑网格表面，去除尖锐棱角，使整体形状更光滑（如从“多边形猪”变为“光滑猪”）。以下是逐行逐模块的详细解释：


### 一、基础头文件与类型定义
```cpp
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Surface_mesh.h>  
#include <CGAL/Polygon_mesh_processing/smooth_shape.h>
#include <CGAL/Polygon_mesh_processing/IO/polygon_mesh_io.h>
#include <iostream>
#include <set>
#include <string>

typedef CGAL::Exact_predicates_inexact_constructions_kernel   K;
typedef CGAL::Surface_mesh<K::Point_3>                        Mesh;
namespace PMP = CGAL::Polygon_mesh_processing;
```
- **几何内核 `K`**：  
  `Exact_predicates_inexact_constructions_kernel` 确保几何谓词（如点是否在边上、法向量计算）的精确性，同时允许坐标以浮点数（`double`）存储，平衡精度与效率。  
- **网格类型 `Mesh`**：  
  表示三角形网格，每个顶点存储为三维点 `K::Point_3`。  
- **命名空间缩写 `PMP`**：  
  简化对 CGAL 多边形网格处理模块的调用。


### 二、主函数流程：输入处理与网格读取
```cpp
int main(int argc, char* argv[]) {
  // 1. 确定输入文件（默认使用 CGAL 自带的 "pig.off" 模型）
  const std::string filename = (argc > 1) ? argv[1] : CGAL::data_file_path("meshes/pig.off");
  Mesh mesh;
  if (!PMP::IO::read_polygon_mesh(filename, mesh)) {
    std::cerr << "Invalid input." << std::endl;
    return 1;
  }

  // 2. 设置平滑参数：迭代次数（默认10次）和时间步长（默认0.0001）
  const unsigned int nb_iterations = (argc > 2) ? std::atoi(argv[2]) : 10;
  const double time = (argc > 3) ? std::atof(argv[3]) : 0.0001;
```
- **输入模型**：  
  支持 OFF 格式的三角形网格，默认使用 `pig.off`（一个多面体猪模型，带有尖锐棱角）。  
- **关键参数**：  
  - `nb_iterations`：平滑迭代次数，次数越多平滑效果越明显（但可能过度平滑）。  
  - `time`：时间步长（曲率流中的演化速率），较小的值更稳定（避免顶点移动过快导致网格翻转）。


### 三、约束边界顶点（保持模型轮廓不变）
```cpp
  // 3. 收集所有边界顶点，防止平滑时边界变形
  std::set<Mesh::Vertex_index> constrained_vertices;
  for (Mesh::Vertex_index v : vertices(mesh)) {
    if (is_border(v, mesh))  // 判断顶点是否在网格边界（边界顶点的边仅被一个面共享）
      constrained_vertices.insert(v);
  }
  std::cout << "Constraining: " << constrained_vertices.size() << " border vertices" << std::endl;

  // 4. 创建顶点约束映射：标记哪些顶点不能移动（边界顶点）
  CGAL::Boolean_property_map<std::set<Mesh::Vertex_index> > vcmap(constrained_vertices);
```
- **边界顶点的意义**：  
  模型的边界（如猪的耳朵边缘、口鼻轮廓）通常是关键特征，平滑时需保持不动，避免轮廓扭曲。  
- `is_border(v, mesh)`：  
  CGAL 提供的函数，判断顶点是否为边界顶点（其邻接边中存在仅被一个面共享的边）。  
- `vcmap`：  
  一个布尔属性映射，将约束顶点集合转换为 CGAL 平滑算法所需的输入格式（`true` 表示顶点被约束，不能移动）。


### 四、核心步骤：形状平滑算法 `smooth_shape()`
```cpp
  // 5. 执行形状平滑：基于平均曲率流，顶点沿法向移动
  std::cout << "Smoothing shape... (" << nb_iterations << " iterations)" << std::endl;
  PMP::smooth_shape(
    mesh, 
    time,  // 时间步长（控制每次顶点移动的幅度）
    CGAL::parameters::number_of_iterations(nb_iterations)  // 迭代次数
                .vertex_is_constrained_map(vcmap)  // 传入顶点约束映射
  );
```
- **算法原理（平均曲率流）**：  
  - 每个顶点沿曲面 **法向** 移动，速度等于其 **平均曲率**：  
    - 凸角（正曲率）顶点向曲面内部移动（“削尖”）；  
    - 凹角（负曲率）顶点向曲面外部移动（“填坑”）；  
    - 平面区域（零曲率）顶点不动。  
  - 自动检测并减缓圆柱状区域的演化（避免“颈缩”奇异点，如猪的脖子变细断裂）。  
- **参数作用**：  
  - `time`：较小的值（如 0.0001）确保稳定性，避免数值发散；  
  - `vertex_is_constrained_map`：强制边界顶点固定，内部顶点自由平滑。


### 五、输出平滑后的网格
```cpp
  // 6. 保存结果：写入 OFF 文件，保留高精度坐标（stream_precision(17)）
  CGAL::IO::write_polygon_mesh("mesh_shape_smoothed.off", mesh, CGAL::parameters::stream_precision(17));
  std::cout << "Done!" << std::endl;
  return EXIT_SUCCESS;
}
```
- **输出格式**：  
  与输入文件相同的 OFF 格式，包含顶点坐标、三角形连接关系，平滑后的网格顶点位置被修改，但拓扑结构（面数、连接关系）不变。  
- **精度控制**：  
  `stream_precision(17)` 确保顶点坐标以双精度浮点数（17位有效数字）存储，避免精度损失。


### 六、代码核心目标总结
1. **输入**：任意三角形网格（如多面体猪模型，带有尖锐棱角）。  
2. **处理**：  
   - 检测所有边界顶点，标记为“约束顶点”（保持不动，保护模型轮廓）；  
   - 使用 **平均曲率流** 算法，对内部顶点进行迭代平滑：  
     - 凸角顶点向曲面内部移动，凹角顶点向外移动，最终趋近于曲率均匀的光滑曲面（如球体）；  
     - 圆柱状区域（如猪的四肢）自动减缓移动，避免生成奇异点。  
3. **输出**：平滑后的网格，边界保持不变，内部棱角被消除，整体形状更光滑（如“多边形猪”变为“圆润猪”）。


### 七、关键参数调优与适用场景
- **`time`（时间步长）**：  
  - 过小：迭代次数需增加（计算时间长），但稳定性高；  
  - 过大：可能导致顶点移动过度，网格翻转（需通过实验调整，通常取极小值如 1e-4）。  
- **`nb_iterations`（迭代次数）**：  
  - 次数少：平滑不彻底（保留部分棱角）；  
  - 次数多：过度平滑（如猪的鼻子、耳朵变得过于圆润，失去特征）。  
- **适用场景**：  
  - 有机模型去噪（如扫描得到的噪声网格）；  
  - 几何简化（将多面体模型转换为光滑曲面，便于渲染或仿真）；  
  - 去除尖锐特征（如将机械零件的棱角平滑为圆角）。


### 八、与网格平滑的区别
- **形状平滑（`smooth_shape`）**：  
  关注**整体曲面形态**，基于曲率流，顶点沿法向移动，适合生成类球体光滑曲面（不优化三角形质量）。  
- **网格平滑（如 `angle_and_area_smoothing`）**：  
  关注**局部三角形质量**（角度、面积均匀化），适合提升网格质量（如有限元分析前的预处理）。  

通过这个示例，CGAL 实现了对三维网格的**内在几何平滑**，无需依赖网格离散化方式（如三角形大小不影响平滑效果），是几何建模和计算机图形学中重要的形状优化工具。