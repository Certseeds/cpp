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
#include <queue>
#include <set>

#ifdef ALGORITHM_TEST_MACRO
namespace day8_1 {
#endif
using std::cin;
using std::cout;
using std::tuple;
using std::vector;
using std::string;
using std::format;
static constexpr const char next_line{'\n'};

struct point {
    int64_t x;
    int64_t y;
    int64_t z;

public:
    static int64_t multi(point x1, point x2) {
        return (x1.x - x2.x) * (x1.x - x2.x)
               + (x1.y - x2.y) * (x1.y - x2.y)
               + (x1.z - x2.z) * (x1.z - x2.z);
    }
};

using input_type = std::tuple<std::vector<point>, int32_t>;
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
    vector<point> datas;
    std::string links;
    std::getline(std::cin, links);
    const auto links_num = std::stoi(links);
    for (std::string line; std::getline(std::cin, line);) {
        auto ranges = std::views::split(line, ',');
        auto iter = ranges.cbegin();
        const auto x_str = *iter;
        ++iter;
        const auto y_str = *iter;
        ++iter;
        const auto z_str = *iter;
        const auto x = std::stoll(std::string(x_str.cbegin(), x_str.cend()));
        const auto y = std::stoll(std::string(y_str.cbegin(), y_str.cend()));
        const auto z = std::stoll(std::string(z_str.cbegin(), z_str.cend()));
        datas.emplace_back(x, y, z);
    }
    return std::make_tuple(datas, links_num);
}

struct distance {
    size_t p1;
    size_t p2;
    int64_t length{0};
};

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

vector<distance> N2(const input_type &datas) {
    const auto &[points, links_num] = datas;
    const auto sizes = points.size();
    static constexpr auto cmp{
        [](const auto v1, const auto v2) {
            return v1.length > v2.length;
        }
    };
    std::priority_queue<distance, std::vector<distance>, decltype(cmp)> distances(cmp);
    for (size_t i = 0; i < sizes; ++i) {
        for (size_t j = 0; j < sizes; ++j) {
            if (i < j) {
                distances.push(distance{i, j, point::multi(points[i], points[j])});
            }
        }
    }
    vector<distance> dists;
    for (size_t i = 0; i < links_num; ++i) {
        const auto head = distances.top();
        distances.pop();
        dists.push_back(head);
        std::cout << std::format("add edge: {} - {}", head.p1, head.p2) << next_line;
    }
    return dists;
}

// 还可以分治来求最短距离, 但是1000数量级还没必要做

// 求这个图的连通分量
int64_t graph_to_connected(const tree::Graph &graph) {
    vector<int8_t> visited(graph.head.size(), 0);
    std::priority_queue<int64_t> nodes_nums;
    for (size_t i = 0; i < graph.head.size(); ++i) {
        if (visited[i]) {
            continue;
        }
        //std::cout << std::format("begin visit {}", i) << next_line;
        int64_t result{0};
        for (std::deque<size_t> orders{i}; !orders.empty();) {
            const auto head = orders.front();
            orders.pop_front();
            if (visited[head]) {
                continue;
            }
            //std::cout << std::format("visit head: {}", head) << next_line;
            visited[head] = 1;
            result += 1;
            for (int32_t ei = graph.head[head]; ei != -1; ei = graph.edges[ei].next) {
                const auto v = graph.edges[ei].to;
                //std::cout << std::format("visit edge: {} - {}", head, v) << next_line;
                if (!visited[v]) {
                    orders.push_front(v);
                }
            }
        }
        //std::cout << std::format("linked size {}", result) << next_line;
        nodes_nums.push(result);
    }
    const auto fst = nodes_nums.top();
    nodes_nums.pop();
    const auto snd = nodes_nums.top();
    nodes_nums.pop();
    const auto trd = nodes_nums.top();
    // std::cout << std::format("{} {} {}", fst, snd, trd) << std::endl;
    return fst * snd * trd;
}

output_type cal(const input_type &datas) {
    const auto dists = N2(datas);
    const auto &[points, links_num] = datas;
    const auto sizes = points.size();
    tree::Graph graph(sizes, 2 * links_num + 3);
    for (const auto head: dists) {
        graph.add_undirected_edge(head.p1, head.p2);
        // std::cout << std::format("add edge: {} - {}", head.p1, head.p2) << next_line;
    }
    const auto result = graph_to_connected(graph);
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
