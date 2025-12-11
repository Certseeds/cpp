# SPDX-License-Identifier: AGPL-3.0-or-later
# SPDX-FileCopyrightText: 2025 nanoseeds

import sys
from dataclasses import dataclass
from typing import List

from ortools.linear_solver import pywraplp


@dataclass
class InputCase:
    clicks: List[List[int]]  # 每个元素是一个点击的索引列表，如 (3) -> [3], (1,3) -> [1,3]
    joltage: List[int]  # 目标状态，如 {3,5,4,7} -> [3,5,4,7]

    @staticmethod
    def tostring(vec: List[int]) -> str:
        return "{" + ",".join(str(x) for x in vec) + "}"


def parse_num_list(s: str) -> List[int]:
    """
    解析形如 (3) 或 (1,3) 或 {3,5,4,7} 的字符串为整数列表
    """
    # 去除括号
    s = s.strip()
    s = s.replace("(", "").replace(")", "")
    s = s.replace("{", "").replace("}", "")
    if not s:
        return []
    return [int(x) for x in s.split(",")]


def read() -> List[InputCase]:
    """
    从标准输入读取数据，解析为 InputCase 列表
    输入格式示例:
    [.##.] (3) (1,3) (2) (2,3) (0,2) (0,1) {3,5,4,7}
    """
    datas: List[InputCase] = []
    with open("./resource/large.data.in") as input:
        lines = input.readlines()
    for line in lines:
        line = line.strip()
        if not line:
            break
        parts = line.split()
        # 第一个是 light_str (如 [.##.])，跳过
        # 最后一个是 joltage (如 {3,5,4,7})
        # 中间的是 clicks (如 (3) (1,3) ...)

        order_list: List[List[int]] = []
        for part in parts[1:]:
            clicks = parse_num_list(part)
            order_list.append(clicks)

        joltage = order_list.pop()  # 最后一个是目标
        datas.append(InputCase(clicks=order_list, joltage=joltage))

    return datas


def process_case(case: InputCase) -> int:
    """
    使用 OR-Tools 求解整数线性规划问题

    问题建模:
    - 变量: x_i = 第 i 个按钮按的次数 (非负整数)
    - 约束: 对于每个计数器 j, sum(x_i * A[i][j]) = joltage[j]
      其中 A[i][j] = 1 如果按钮 i 影响计数器 j, 否则为 0
    - 目标: 最小化 sum(x_i) (总按键次数)
    """
    num_counters = len(case.joltage)
    num_buttons = len(case.clicks)

    # 构建系数矩阵 A: A[i][j] = 1 表示按钮 i 会增加计数器 j
    # clicks[i] 是按钮 i 影响的计数器索引列表
    A = [[0] * num_counters for _ in range(num_buttons)]
    for i, click_indices in enumerate(case.clicks):
        for j in click_indices:
            if j < num_counters:
                A[i][j] = 1

    # 创建 solver
    solver = pywraplp.Solver.CreateSolver("SCIP")
    if not solver:
        # 尝试使用 CBC 作为备选
        solver = pywraplp.Solver.CreateSolver("CBC")
    if not solver:
        print("Solver not available", file=sys.stderr)
        return 0x3F3F3F3F

    # 定义变量: x[i] = 按钮 i 的按压次数，非负整数
    # 上界设为所有目标值之和（最坏情况）
    upper_bound = sum(case.joltage) + 1
    x = [solver.IntVar(0, upper_bound, f"x_{i}") for i in range(num_buttons)]

    # 添加约束: 对于每个计数器 j，所有按钮对它的贡献之和等于目标值
    # sum(x[i] * A[i][j] for i in range(num_buttons)) == joltage[j]
    for j in range(num_counters):
        constraint_expr = sum(x[i] * A[i][j] for i in range(num_buttons))
        solver.Add(constraint_expr == case.joltage[j])

    # 目标函数: 最小化总按键次数
    solver.Minimize(sum(x))

    # 求解
    status = solver.Solve()

    if status == pywraplp.Solver.OPTIMAL:
        result = int(solver.Objective().Value())
        return result
    elif status == pywraplp.Solver.INFEASIBLE:
        print(f"No solution for {InputCase.tostring(case.joltage)}", file=sys.stderr)
        return 0x3F3F3F3F
    else:
        print(f"Solver status: {status}", file=sys.stderr)
        return 0x3F3F3F3F


def cal(datas: List[InputCase]) -> int:
    """
    主计算函数，遍历所有 case 并累加结果
    """
    result = 0
    for data in datas:
        min_result = process_case(data)
        if min_result != 0x3F3F3F3F:
            result += min_result
    return result


def output(data: int) -> None:
    print(data)


def main():
    input_data = read()
    output_data = cal(input_data)
    output(output_data)


if __name__ == "__main__":
    main()
