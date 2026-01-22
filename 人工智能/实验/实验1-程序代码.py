#!/usr/bin/env python
# coding: utf-8

# In[1]:


import heapq  # 导入堆队列模块，用于实现优先队列
import copy  # 导入复制模块，用于深度复制状态
import time  # 导入时间模块，用于计算算法运行时间


class EightPuzzle:
    def __init__(self, initial_state, goal_state):
        # 初始化八数码问题求解器
        self.initial_state = initial_state  # 存储初始状态
        self.goal_state = goal_state  # 存储目标状态
        self.n = 3  # 网格大小为3x3

    def find_blank(self, state):
        """找到空白格(0)的位置"""
        # 遍历整个3x3网格
        for i in range(self.n):
            for j in range(self.n):
                # 如果找到值为0的位置（空白格）
                if state[i][j] == 0:
                    return i, j  # 返回空白格的行列坐标
        return None  # 如果没有找到空白格，返回None

    def get_neighbors(self, state):
        """获取所有可能的下一步状态"""
        # 找到当前状态中空白格的位置
        i, j = self.find_blank(state)
        neighbors = []  # 存储所有邻居状态
        moves = []  # 存储移动方向（未使用）

        # 定义四个可能的移动方向：上、下、左、右
        # 每个方向用(dx, dy, 方向名称)表示
        directions = [(-1, 0, '上'), (1, 0, '下'), (0, -1, '左'), (0, 1, '右')]

        # 遍历所有可能的方向
        for dx, dy, direction in directions:
            # 计算移动后的新位置
            new_i, new_j = i + dx, j + dy
            # 检查新位置是否在网格范围内
            if 0 <= new_i < self.n and 0 <= new_j < self.n:
                # 创建当前状态的深度副本，避免修改原状态
                new_state = copy.deepcopy(state)
                # 交换空白格和目标位置的数字
                new_state[i][j], new_state[new_i][new_j] = new_state[new_i][
                    new_j], new_state[i][j]
                # 将新状态和移动方向添加到邻居列表中
                neighbors.append((new_state, direction))

        return neighbors  # 返回所有可能的邻居状态

    def misplaced_tiles(self, state):
        """启发函数1: 不在位的将牌个数(W(n))"""
        count = 0  # 计数器，记录不在位的将牌数量
        # 遍历整个网格
        for i in range(self.n):
            for j in range(self.n):
                # 如果不是空白格且当前位置的数字与目标位置不同
                if state[i][j] != 0 and state[i][j] != self.goal_state[i][j]:
                    count += 1  # 增加计数器
        return count  # 返回不在位的将牌总数

    def manhattan_distance(self, state):
        """启发函数2: 曼哈顿距离(P(n))"""
        distance = 0  # 总曼哈顿距离
        positions = {}  # 字典，存储目标状态中每个数字的位置

        # 构建目标状态的位置映射
        for i in range(self.n):
            for j in range(self.n):
                # 如果不是空白格，记录该数字在目标状态中的位置
                if self.goal_state[i][j] != 0:
                    positions[self.goal_state[i][j]] = (i, j)

        # 计算当前状态中每个将牌的曼哈顿距离
        for i in range(self.n):
            for j in range(self.n):
                tile = state[i][j]  # 当前格子的数字
                # 如果不是空白格
                if tile != 0:
                    # 获取该数字在目标状态中的位置
                    goal_i, goal_j = positions[tile]
                    # 计算曼哈顿距离并累加到总距离
                    distance += abs(i - goal_i) + abs(j - goal_j)

        return distance  # 返回总曼哈顿距离

    def is_goal(self, state):
        """检查是否达到目标状态"""
        # 直接比较当前状态和目标状态是否完全相同
        return state == self.goal_state

    def solve(self, heuristic='misplaced'):
        """使用A*算法求解八数码问题"""
        open_list = []  # 开放列表，存储待扩展的节点（优先队列）
        closed_set = set()  # 关闭集合，存储已扩展的节点（用于避免重复扩展）

        # 计算初始节点的启发函数值
        if heuristic == 'misplaced':
            h_value = self.misplaced_tiles(self.initial_state)  # 使用不在位将牌数
        else:
            h_value = self.manhattan_distance(self.initial_state)  # 使用曼哈顿距离

        # 创建初始节点
        initial_node = {
            'state': self.initial_state,  # 节点状态
            'g': 0,  # 从起始节点到当前节点的实际代价（步数）
            'h': h_value,  # 启发函数估计值
            'parent': None,  # 父节点，用于重建路径
            'action': None  # 从父节点到当前节点的移动方向
        }
        initial_node['f'] = initial_node['g'] + initial_node[
            'h']  # 总估计代价 f(n) = g(n) + h(n)

        # 将初始节点加入开放列表
        # 使用堆队列实现优先队列，按f值排序
        # id(initial_node['state'])用于确保唯一性，避免比较状态本身
        heapq.heappush(
            open_list,
            (initial_node['f'], id(initial_node['state']), initial_node))

        nodes_expanded = 0  # 记录扩展的节点总数
        max_open_list_size = 1  # 记录开放列表的最大大小

        # 主循环：当开放列表不为空时继续搜索
        while open_list:
            # 从开放列表中取出f值最小的节点（A*算法的核心）
            current_f, _, current_node = heapq.heappop(open_list)
            current_state = current_node['state']  # 当前节点的状态

            # 将状态转换为可哈希的元组形式，用于集合比较
            state_tuple = tuple(tuple(row) for row in current_state)

            # 如果当前节点已经在关闭集合中，跳过处理
            if state_tuple in closed_set:
                continue

            # 检查当前节点是否为目标状态
            if self.is_goal(current_state):
                # 如果是目标状态，重建解路径
                path = []  # 存储解路径
                node = current_node  # 从当前节点开始回溯
                # 沿着父节点指针回溯，直到起始节点
                while node['parent'] is not None:
                    # 将移动方向和状态添加到路径中
                    path.append((node['action'], node['state']))
                    node = node['parent']  # 移动到父节点
                path.reverse()  # 反转路径，使其从起始到目标
                # 返回解信息
                return {
                    'solution': path,  # 解路径
                    'nodes_expanded': nodes_expanded,  # 扩展的节点总数
                    'max_open_list': max_open_list_size,  # 开放列表的最大大小
                    'solution_length': len(path)  # 解路径长度（步数）
                }

            # 将当前节点添加到关闭集合，标记为已扩展
            closed_set.add(state_tuple)
            nodes_expanded += 1  # 增加扩展节点计数

            # 生成当前节点的所有邻居节点（可能的下一个状态）
            for neighbor_state, action in self.get_neighbors(current_state):
                # 将邻居状态转换为可哈希的元组形式
                neighbor_tuple = tuple(tuple(row) for row in neighbor_state)

                # 如果邻居节点已经在关闭集合中，跳过处理
                if neighbor_tuple in closed_set:
                    continue

                # 计算邻居节点的g、h、f值
                g = current_node['g'] + 1  # 实际代价比父节点增加1
                if heuristic == 'misplaced':
                    h = self.misplaced_tiles(neighbor_state)  # 使用不在位将牌数
                else:
                    h = self.manhattan_distance(neighbor_state)  # 使用曼哈顿距离
                f = g + h  # 总估计代价

                # 创建邻居节点
                neighbor_node = {
                    'state': neighbor_state,
                    'g': g,
                    'h': h,
                    'f': f,
                    'parent': current_node,  # 设置父节点为当前节点
                    'action': action  # 记录从当前节点到邻居节点的移动方向
                }

                # 将邻居节点加入开放列表
                heapq.heappush(open_list,
                               (f, id(neighbor_state), neighbor_node))

            # 更新开放列表的最大大小
            max_open_list_size = max(max_open_list_size, len(open_list))

        return None  # 如果开放列表为空且未找到解，返回无解


def print_state(state):
    """打印状态"""
    # 遍历每一行
    for row in state:
        # 将每个数字转换为字符串，0显示为空格，用空格分隔
        print(' '.join(str(x) if x != 0 else ' ' for x in row))
    print()  # 打印空行


def input_state(prompt):
    """从用户输入获取状态"""
    print(prompt)
    state = []  # 存储状态
    # 循环输入3行
    for i in range(3):
        while True:
            # 获取用户输入，去除首尾空格并按空格分割
            row_input = input(
                f"请输入第{i+1}行(3个数字，用空格分隔，0代表空格): ").strip().split()
            # 检查输入是否为3个数字
            if len(row_input) == 3:
                try:
                    # 尝试将输入转换为整数列表
                    row = [int(x) for x in row_input]
                    state.append(row)  # 添加到状态中
                    break  # 输入有效，退出循环
                except ValueError:
                    print("输入错误，请确保输入的是数字!")
            else:
                print("输入错误，请确保输入3个数字!")
    return state  # 返回构建的状态


def main():
    print("=" * 50)
    print("八数码问题求解器")
    print("=" * 50)

    # 获取用户输入的初始状态
    print("\n请输入初始状态:")
    initial_state = input_state("请按行输入3x3的数字矩阵:")

    # 获取用户输入的目标状态
    print("\n请输入目标状态:")
    goal_state = input_state("请按行输入3x3的数字矩阵:")

    # 打印初始状态和目标状态
    print("\n初始状态:")
    print_state(initial_state)
    print("目标状态:")
    print_state(goal_state)

    # 创建八数码问题求解器实例
    puzzle = EightPuzzle(initial_state, goal_state)

    # 让用户选择启发函数
    print("请选择启发函数:")
    print("1. 不在位将牌数(W(n))")
    print("2. 曼哈顿距离(P(n))")
    choice = input("请输入选择(1或2): ").strip()

    # 根据用户选择设置启发函数
    if choice == '1':
        heuristic = 'misplaced'
        heuristic_name = "不在位将牌数(W(n))"
    else:
        heuristic = 'manhattan'
        heuristic_name = "曼哈顿距离(P(n))"

    print(f"\n使用{heuristic_name}作为启发函数进行求解...")

    # 记录开始时间，执行求解
    start_time = time.time()
    result = puzzle.solve(heuristic=heuristic)
    end_time = time.time()  # 记录结束时间

    # 处理求解结果
    if result:
        # 如果找到解，打印解信息
        print(f"找到解! 总步数: {result['solution_length']}")
        print(f"扩展节点数: {result['nodes_expanded']}")
        print(f"最大开放列表大小: {result['max_open_list']}")
        print(f"求解耗时: {end_time - start_time:.4f}秒")

        # 询问用户是否显示详细解路径
        show_path = input("\n是否显示详细解路径? (y/n): ").strip().lower()
        if show_path == 'y':
            print("\n解路径:")
            print("初始状态:")
            print_state(initial_state)
            # 遍历解路径中的每一步
            for i, (action, state) in enumerate(result['solution'], 1):
                print(f"步骤{i}: 移动方向: {action}")
                print_state(state)
    else:
        print("无解!")  # 如果无解，打印提示信息


# 程序入口点
if __name__ == "__main__":
    main()


# In[ ]:




