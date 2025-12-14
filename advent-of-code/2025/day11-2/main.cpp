// SPDX-License-Identifier: AGPL-3.0-or-later
// SPDX-FileCopyrightText: 2025 nanoseeds

#include <algorithm>
#include <ranges>
#include <tuple>
#include <vector>
#include <iostream>
#include <string>
#include <deque>
#include <unordered_map>
#include <format>
#include <cassert>
#include <complex>
#include <queue>
#include <set>

#ifdef ALGORITHM_TEST_MACRO
namespace day11_2 {
#endif
using std::cin;
using std::cout;
using std::tuple;
using std::vector;
using std::string;
using std::format;
static constexpr const char next_line{'\n'};

namespace tree {
struct Edge {
    int32_t to; // 边的终点
    int32_t next; // 同一个起点的下一条边的索引
};

class Graph {
public:
    // 边的结构体定义
    std::vector<int32_t> head; // head[i] 存储顶点i的第一条边的索引
    std::vector<Edge> edges; // 存储所有边的数组
    int32_t edge_count; // 当前边的总数
public:
    /**
     * @brief 构造函数
     * @param num_nodes 顶点的数量 (假设顶点编号从 1 到 num_nodes)
     * @param max_edges 预估的最大边数 (对于无向图，一条无向边算两条有向边)
     */
    Graph(int32_t num_nodes, int32_t max_edges) : edge_count(0) {
        // 初始化头节点数组，大小为顶点数+1，方便1-based 索引
        // 所有顶点的初始头节点都设为-1，表示没有出边
        head.assign(num_nodes + 1, -1);
        // 预分配存储所有边的空间
        edges.resize(max_edges);
    }

    /**
     * @brief 添加一条有向边
     * @param u 起点
     * @param v 终点
     * @param w 权重
     */
    void add_edge(const int32_t u, const int32_t v) {
        // 在边数组的末尾添加新边
        edges[edge_count].to = v;
        // 新边的 next 指向原来以 u 为起点的第一条边
        edges[edge_count].next = head[u];
        // 更新以 u 为起点的第一条边为当前新添加的边
        head[u] = edge_count;
        // 边计数器加一
        edge_count++;
    }

    /**
     * @brief 添加一条无向边
     * @param u, v 顶点
     * @param w 权重
     */
    void add_undirected_edge(const int32_t u, const int32_t v) {
        add_edge(u, v);
        add_edge(v, u);
    }
};
}

struct input_datas {
    tree::Graph graph; //图
    std::unordered_map<size_t, size_t> in_num; // 入度统计
    size_t head_index;
    size_t dac_index;
    size_t fft_index;
};

using input_type = input_datas;
using output_type = int64_t;

inline input_type read();

output_type cal(const input_type &data);

void output(const output_type &data);

int main() {
    const auto input_data = read();
    const auto output_data = cal(input_data);
    output(output_data);
    return 0;
}

inline input_type read() {
    vector<tuple<string, vector<string> > > datas;
    vector<string> inputs;
    for (std::string line; std::getline(std::cin, line);) {
        inputs.push_back(line);
    }
    size_t edges_size{0};
    for (const auto &line: inputs) {
        auto parts = std::views::split(line, ':');
        auto iter = parts.cbegin();
        const auto parent = string(cbegin(*iter), std::cend(*iter));
        ++iter;
        const auto snd_str = *iter;
        const auto real_snd_str = std::string(snd_str.cbegin() + 1, snd_str.cend());

        auto sub_parts = std::views::split(real_snd_str, ' ');
        vector<std::string> subs;
        for (auto sub_part: sub_parts) {
            subs.emplace_back(cbegin(sub_part), cend(sub_part));
        }
        edges_size += std::ranges::size(subs);
        datas.emplace_back(parent, subs);
    }
    // 然后反向建图
    std::unordered_map<string, size_t> map; // str -> index
    for (auto iter = datas.cbegin(); iter != datas.cend(); ++iter) {
        const auto &[name, subs] = *iter;
        const auto index = std::distance(datas.cbegin(), iter);
        map[name] = index;
    }
    std::unordered_map<size_t, size_t> in_num;
    map["out"] = datas.size(); // mark the final node
    tree::Graph graph(datas.size() + 1, edges_size + 3);
    for (const auto &data: datas) {
        const auto &[name, subs] = data;
        const auto name_index = map[name];
        for (const auto &sub: subs) {
            const auto sub_index = map[sub];
            graph.add_edge(sub_index, name_index);
            in_num[name_index] += 1;
        }
    }
    const auto head_index = map["svr"];
    const auto dac_index = map["dac"];
    const auto fft_index = map["fft"];
    return {graph, in_num, head_index, dac_index, fft_index};
}

output_type cal(const input_type &datas) {
    const auto &[graph, _in_num, head_index, dec_index, fft_index] = datas;
    std::unordered_map<size_t, size_t> in_num{_in_num};
    int64_t result{0};
    const size_t out_index = graph.head.size() - 2;
    std::vector<size_t> visited(graph.head.size(), 0);
    std::vector<std::array<size_t, 4>> times(graph.head.size(), {0, 0, 0, 0});
    times[out_index] = {1, 0, 0, 0};

    std::deque<size_t> queue;
    queue.push_back(out_index); // 从out结点开始

    while (!queue.empty()) {
        const auto element = queue.back();
        queue.pop_back();
        if (visited[element] == true) {
            continue;
        }
        for (int32_t ei = graph.head[element]; ei != -1; ei = graph.edges[ei].next) {
            const auto v = graph.edges[ei].to;
            if (v == dec_index) {
                times[v][0] = 0; // it's visited
                times[v][1] += times[element][0]; // becase then times[v][1] must be zero, so do the last
                times[v][2] = 0;
                times[v][3] += times[element][2];
            } else if (v == fft_index) {
                times[v][0] += 0;
                times[v][1] += 0;
                times[v][2] += (times[element][0]);
                times[v][3] += (times[element][1]);
            } else {
                times[v][0] += times[element][0];
                times[v][1] += times[element][1];
                times[v][2] += times[element][2];
                times[v][3] += times[element][3];
            }
            in_num[v] -= 1;
            if (in_num[v] == 0) {
                queue.push_back(v);
            }
        }
        visited[element] = true;
    }
    result += times[head_index][3];
    return result;
}


void output(const output_type &data) {
    std::cout << data << next_line;
}

static const auto faster_streams = [] {
    srand(time(nullptr));
    // use time to init the random seed
    std::ios::sync_with_stdio(false);
    std::istream::sync_with_stdio(false);
    std::ostream::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
    // 关闭c++风格输入输出 , 与C风格输入输出的同步,提高性能.
    return 0;
}();

#ifdef ALGORITHM_TEST_MACRO
}
#endif
