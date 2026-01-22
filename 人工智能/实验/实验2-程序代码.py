#!/usr/bin/env python
# coding: utf-8

# In[3]:


import heapq  # 导入堆队列模块，用于实现优先队列
import math   # 导入数学模块，用于计算欧几里得距离
import time   # 导入时间模块，用于计算算法运行时间

class Node:
    """
    节点类：表示迷宫中的每个位置点
    
    属性：
    - x, y: 节点的坐标位置
    - g: 从起点到当前节点的实际代价
    - h: 从当前节点到目标节点的启发式估计代价
    - f: 总代价，f = g + h
    - parent: 父节点指针，用于路径重构
    - neighbors: 相邻节点列表，存储与该节点相连的其他节点
    """
    
    def __init__(self, x, y):
        """初始化节点"""
        self.x = x          # 节点的x坐标
        self.y = y          # 节点的y坐标
        self.g = 0          # 从起点到当前节点的实际代价，初始为0
        self.h = 0          # 启发函数值，初始为0
        self.f = 0          # 总代价f = g + h，初始为0
        self.parent = None  # 父节点指针，用于回溯路径
        self.neighbors = [] # 存储相连的节点列表
    
    def __lt__(self, other):
        """
        小于比较运算符重载
        用于堆队列中的节点比较，按照f值排序
        """
        return self.f < other.f
    
    def __eq__(self, other):
        """
        相等比较运算符重载
        判断两个节点是否相同（坐标相同）
        """
        return self.x == other.x and self.y == other.y


class MazeSolver:
    """
    迷宫求解器类：使用A*算法求解迷宫最短路径
    
    属性：
    - nodes: 迷宫中的所有节点
    - edges: 迷宫中的所有边（连接关系）
    - start: 起点节点
    - end: 终点节点
    - nodes_expanded: 算法扩展的节点数量（用于性能分析）
    """
    
    def __init__(self, nodes, edges, start, end):
        """初始化迷宫求解器"""
        self.nodes = nodes  # 存储所有节点
        self.edges = edges  # 存储所有边（连接关系）
        self.start = start  # 起点节点
        self.end = end      # 终点节点
        self.nodes_expanded = 0  # 记录扩展的节点数量
        
        # 构建邻接关系：根据边信息建立节点之间的连接
        self.build_adjacency()
    
    def build_adjacency(self):
        """根据边构建节点的邻接关系"""
        # 遍历所有边
        for edge in self.edges:
            node1, node2 = edge  # 解构边，获取两个节点
            
            # 如果node2不在node1的邻居列表中，则添加
            if node2 not in node1.neighbors:
                node1.neighbors.append(node2)
            
            # 如果node1不在node2的邻居列表中，则添加
            if node1 not in node2.neighbors:
                node2.neighbors.append(node1)
    
    def heuristic_manhattan(self, node):
        """
        曼哈顿距离启发函数
        计算当前节点到终点的曼哈顿距离（水平和垂直方向的距离之和）
        公式：|x1 - x2| + |y1 - y2|
        """
        return abs(node.x - self.end.x) + abs(node.y - self.end.y)
    
    def heuristic_euclidean(self, node):
        """
        欧几里得距离启发函数
        计算当前节点到终点的直线距离
        公式：√((x1 - x2)² + (y1 - y2)²)
        """
        return math.sqrt((node.x - self.end.x)**2 + (node.y - self.end.y)**2)
    
    def heuristic_zero(self, node):
        """
        零启发函数
        当启发函数始终返回0时，A*算法退化为Dijkstra算法
        会搜索所有可能的方向，保证找到最短路径但效率较低
        """
        return 0
    
    def a_star(self, heuristic_func=None, heuristic_name="曼哈顿距离"):
        """
        A*算法实现
        
        参数：
        - heuristic_func: 启发函数，默认为曼哈顿距离
        - heuristic_name: 启发函数名称，用于显示
        
        返回值：
        - path: 找到的路径（节点坐标列表）
        - run_time: 算法运行时间
        - nodes_expanded: 扩展的节点数量
        """
        
        # 如果没有指定启发函数，使用曼哈顿距离作为默认
        if heuristic_func is None:
            heuristic_func = self.heuristic_manhattan
        
        # 开放列表：存储待探索的节点（使用最小堆实现优先队列）
        open_list = []
        # 关闭集合：存储已探索的节点
        closed_set = set()
        # 重置扩展节点计数器
        self.nodes_expanded = 0
        
        # 初始化起点节点
        self.start.g = 0  # 起点到起点的代价为0
        self.start.h = heuristic_func(self.start)  # 计算启发函数值
        self.start.f = self.start.g + self.start.h  # 计算总代价
        # 将起点加入开放列表
        heapq.heappush(open_list, self.start)
        
        # 记录算法开始时间
        start_time = time.time()
        
        # 主循环：当开放列表不为空时继续搜索
        while open_list:
            # 从开放列表中取出f值最小的节点（堆顶元素）
            current = heapq.heappop(open_list)
            # 增加扩展节点计数
            self.nodes_expanded += 1
            
            # 检查是否到达终点
            if current.x == self.end.x and current.y == self.end.y:
                # 记录结束时间
                end_time = time.time()
                # 重构路径
                path = self.reconstruct_path(current)
                # 返回路径、运行时间和扩展节点数
                return path, end_time - start_time, self.nodes_expanded
            
            # 将当前节点加入关闭集合（标记为已探索）
            closed_set.add((current.x, current.y))
            
            # 遍历当前节点的所有邻居节点
            for neighbor in current.neighbors:
                # 如果邻居节点已在关闭集合中，跳过
                if (neighbor.x, neighbor.y) in closed_set:
                    continue
                
                # 计算从起点经过当前节点到邻居节点的代价
                # 假设每条边的代价为1
                tentative_g = current.g + 1
                
                # 检查邻居节点是否在开放列表中
                in_open = False
                for node in open_list:
                    if node.x == neighbor.x and node.y == neighbor.y:
                        in_open = True
                        # 如果找到更优路径，更新邻居节点的信息
                        if tentative_g < node.g:
                            node.g = tentative_g  # 更新实际代价
                            node.h = heuristic_func(node)  # 更新启发值
                            node.f = node.g + node.h  # 更新总代价
                            node.parent = current  # 更新父节点
                        break
                
                # 如果邻居节点不在开放列表中，将其加入
                if not in_open:
                    neighbor.g = tentative_g  # 设置实际代价
                    neighbor.h = heuristic_func(neighbor)  # 计算启发值
                    neighbor.f = neighbor.g + neighbor.h  # 计算总代价
                    neighbor.parent = current  # 设置父节点
                    # 将邻居节点加入开放列表（优先队列）
                    heapq.heappush(open_list, neighbor)
        
        # 如果开放列表为空且未找到路径，返回None
        end_time = time.time()
        return None, end_time - start_time, self.nodes_expanded
    
    def reconstruct_path(self, node):
        """
        重构路径：从终点节点回溯到起点节点
        
        参数：
        - node: 终点节点
        
        返回值：
        - path: 从起点到终点的路径（节点坐标列表）
        """
        path = []  # 存储路径
        current = node  # 从终点开始
        
        # 沿着父节点指针回溯到起点
        while current:
            path.append((current.x, current.y))  # 添加当前节点坐标
            current = current.parent  # 移动到父节点
        
        # 反转路径，使其从起点到终点
        return path[::-1]
    
    def print_maze_graph(self, path=None):
        """
        打印迷宫图：显示节点和连接关系
        
        参数：
        - path: 找到的路径（可选），如果提供会标记路径上的节点
        """
        print("\n迷宫图（节点和连接关系）:")
        
        # 创建坐标到节点的映射（虽然这里没用到，但保留以备扩展）
        coord_to_node = {(node.x, node.y): node for node in self.nodes}
        
        # 打印所有节点及其标记
        print("节点坐标:")
        for node in self.nodes:
            marker = ""  # 节点标记
            # 如果是起点节点
            if (node.x, node.y) == (self.start.x, self.start.y):
                marker = " [起点]"
            # 如果是终点节点
            elif (node.x, node.y) == (self.end.x, self.end.y):
                marker = " [终点]"
            # 如果在路径上
            elif path and (node.x, node.y) in path:
                marker = " [路径]"
            # 打印节点坐标和标记
            print(f"  ({node.x},{node.y}){marker}")
        
        # 打印所有连接关系
        print("\n连接关系:")
        printed_edges = set()  # 用于避免重复打印边
        # 遍历所有边
        for edge in self.edges:
            node1, node2 = edge
            # 创建边的唯一标识（排序后的坐标元组）
            edge_key = tuple(sorted([(node1.x, node1.y), (node2.x, node2.y)]))
            # 如果这条边还没打印过
            if edge_key not in printed_edges:
                # 打印连接关系
                print(f"  ({node1.x},{node1.y}) -- ({node2.x},{node2.y})")
                # 标记这条边已打印
                printed_edges.add(edge_key)


def input_maze_graph():
    """
    从用户输入获取迷宫图信息
    
    返回值：
    - nodes: 节点列表
    - edges: 边列表
    """
    print("请输入迷宫图的节点和连接关系")
    print("说明：")
    print("  - 首先输入所有节点的坐标")
    print("  - 然后输入节点之间的连接关系")
    print("  - 坐标格式: x y (例如: 1 1)")
    
    nodes = []  # 存储所有节点
    edges = []  # 存储所有边
    
    # 步骤1: 输入所有节点坐标
    print("\n步骤1: 输入所有节点坐标")
    print("输入'done'结束节点输入")
    
    while True:
        try:
            # 获取用户输入的节点坐标
            coord_input = input("输入节点坐标 (x y): ").strip()
            # 如果输入'done'，结束节点输入
            if coord_input.lower() == 'done':
                # 检查节点数量是否足够
                if len(nodes) < 2:
                    print("至少需要2个节点！")
                    continue
                break
            
            # 分割输入字符串
            parts = coord_input.split()
            # 检查输入格式
            if len(parts) != 2:
                print("错误：请输入两个数字！")
                continue
            
            # 解析坐标
            x, y = int(parts[0]), int(parts[1])
            
            # 检查节点是否已存在
            if any(node.x == x and node.y == y for node in nodes):
                print("错误：该坐标的节点已存在！")
                continue
            
            # 创建新节点并添加到列表
            nodes.append(Node(x, y))
            print(f"已添加节点 ({x},{y})")
            
        except ValueError:
            print("错误：请输入有效的数字！")
    
    # 步骤2: 输入节点之间的连接关系
    print("\n步骤2: 输入节点之间的连接关系")
    print("输入'done'结束边输入")
    
    # 显示所有节点及其索引，方便用户输入
    print("可用节点:")
    for i, node in enumerate(nodes):
        print(f"  {i}: ({node.x},{node.y})")
    
    while True:
        try:
            # 获取用户输入的连接关系
            edge_input = input("输入连接关系 (节点索引1 节点索引2): ").strip()
            # 如果输入'done'，结束边输入
            if edge_input.lower() == 'done':
                # 检查边数量是否足够
                if len(edges) == 0:
                    print("至少需要一条边！")
                    continue
                break
            
            # 分割输入字符串
            parts = edge_input.split()
            # 检查输入格式
            if len(parts) != 2:
                print("错误：请输入两个节点索引！")
                continue
            
            # 解析节点索引
            idx1, idx2 = int(parts[0]), int(parts[1])
            
            # 检查索引是否有效
            if idx1 < 0 or idx1 >= len(nodes) or idx2 < 0 or idx2 >= len(nodes):
                print("错误：节点索引超出范围！")
                continue
            
            # 检查是否连接同一个节点
            if idx1 == idx2:
                print("错误：不能连接同一个节点！")
                continue
            
            # 获取对应的节点对象
            node1, node2 = nodes[idx1], nodes[idx2]
            
            # 检查边是否已存在
            edge_exists = False
            for edge in edges:
                if (edge[0] == node1 and edge[1] == node2) or (edge[0] == node2 and edge[1] == node1):
                    edge_exists = True
                    break
            
            if edge_exists:
                print("错误：该边已存在！")
                continue
            
            # 添加边到列表
            edges.append((node1, node2))
            print(f"已添加连接: ({node1.x},{node1.y}) -- ({node2.x},{node2.y})")
            
        except ValueError:
            print("错误：请输入有效的数字！")
    
    return nodes, edges


def main():
    """
    主函数：程序入口点
    """
    print("=" * 60)
    print("           迷宫问题 - A*算法求解（图结构）")
    print("=" * 60)
    
    # 输入迷宫图
    nodes, edges = input_maze_graph()
    
    # 选择起点和终点
    print("\n步骤3: 选择起点和终点")
    print("可用节点:")
    for i, node in enumerate(nodes):
        print(f"  {i}: ({node.x},{node.y})")
    
    while True:
        try:
            # 获取起点和终点索引
            start_idx = int(input("选择起点节点索引: "))
            end_idx = int(input("选择终点节点索引: "))
            
            # 检查索引是否有效
            if start_idx < 0 or start_idx >= len(nodes) or end_idx < 0 or end_idx >= len(nodes):
                print("错误：节点索引超出范围！")
                continue
            
            # 检查起点和终点是否相同
            if start_idx == end_idx:
                print("错误：起点和终点不能相同！")
                continue
            
            break
        except ValueError:
            print("错误：请输入有效的数字！")
    
    # 获取起点和终点节点
    start_node = nodes[start_idx]
    end_node = nodes[end_idx]
    
    # 显示起点和终点信息
    print(f"\n起点: ({start_node.x},{start_node.y})")
    print(f"终点: ({end_node.x},{end_node.y})")
    
    print("\n" + "=" * 60)
    print("开始求解...")
    print("=" * 60)
    
    # 创建迷宫求解器实例
    solver = MazeSolver(nodes, edges, start_node, end_node)
    
    # 显示迷宫图
    solver.print_maze_graph()
    
    # 定义要测试的启发函数列表
    heuristic_functions = [
        (solver.heuristic_manhattan, "曼哈顿距离"),
        (solver.heuristic_euclidean, "欧几里得距离"),
        (solver.heuristic_zero, "零启发函数(Dijkstra)")
    ]
    
    results = []  # 存储各启发函数的求解结果
    
    # 对每个启发函数进行测试
    for heuristic_func, name in heuristic_functions:
        print(f"\n使用{name}启发函数:")
        print("-" * 40)
        
        # 运行A*算法
        path, run_time, nodes_expanded = solver.a_star(heuristic_func, name)
        
        # 处理求解结果
        if path:
            print(f"✓ 找到最短路径!")
            print(f"路径: {path}")
            print(f"路径长度: {len(path)}步")
            print(f"扩展节点数: {nodes_expanded}")
            print(f"运行时间: {run_time:.6f}秒")
            # 保存结果
            results.append((name, path, len(path), nodes_expanded, run_time))
            
            # 显示带路径的迷宫图（只在第一次成功时显示）
            if name == "曼哈顿距离":
                solver.print_maze_graph(path)
        else:
            print("✗ 未找到路径!")
            # 保存结果（即使未找到路径）
            results.append((name, None, 0, nodes_expanded, run_time))
    
    # 结果比较：显示各启发函数的性能对比
    print("\n" + "=" * 60)
    print("            算法性能比较")
    print("=" * 60)
    
    # 打印表头
    print(f"{'启发函数':<20} {'找到路径':<8} {'路径长度':<8} {'扩展节点数':<10} {'运行时间(秒)':<12}")
    print("-" * 65)
    
    # 打印每行结果
    for name, path, length, nodes, time_taken in results:
        found = "是" if path else "否"  # 是否找到路径
        length_str = str(length) if path else "N/A"  # 路径长度
        # 格式化输出
        print(f"{name:<20} {found:<8} {length_str:<8} {nodes:<10} {time_taken:.6f}")


# 程序入口点
if __name__ == "__main__":
    main()


# In[ ]:




