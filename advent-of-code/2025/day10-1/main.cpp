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
namespace day10_1 {
#endif
using std::cin;
using std::cout;
using std::tuple;
using std::vector;
using std::string;
using std::format;
static constexpr const char next_line{'\n'};

struct input_case {
    string lights;
    vector<vector<size_t> > clicks;
    vector<size_t> joltage;
};

using input_type = std::vector<input_case>;
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

vector<size_t> num_list_to_vector(const auto ranges) {
    vector<size_t> clicks{};
    const auto node_str = ranges;
    const auto node_str_str = string(node_str.cbegin(), node_str.cend());
    auto orders = node_str | std::views::filter([](const char ch) {
        return ch != '(' && ch != ')'
               && ch != '{' && ch != '}';
    }) | std::views::split(',');
    for (auto num_str: orders) {
        const auto num = std::stoll(std::string(num_str.cbegin(), num_str.cend()));
        clicks.push_back(num);
    }
    return clicks;
}

inline input_type read() {
    input_type datas;
    for (std::string line; std::getline(std::cin, line);) {
        auto ranges = std::views::split(line, ' ');
        auto iter = ranges.cbegin();
        const auto light_str = *iter;
        const auto lights = std::string(light_str.cbegin() + 1, light_str.cend() - 1);

        std::vector<vector<size_t> > order_list;
        for (++iter; iter != ranges.cend(); ++iter) {
            const vector<size_t> clicks = num_list_to_vector(*iter);
            order_list.push_back(clicks);
        }
        const auto joltage = order_list.back();
        order_list.pop_back();
        datas.emplace_back(lights, order_list, joltage);
    }
    return datas;
}

enum LIGHT {
    NO = '.',
    YES = '#'
};

// input_state 添加上node之后的状态
string add(const string &input_state, const vector<size_t> &nodes) {
    string result{input_state};
    for (const auto node: nodes) {
        result[node] = input_state[node] == NO ? YES : NO;
    }
    return result;
}

string sub(const string &target, const string &sourceA) {
    string result{target};
    for (size_t i = 0; i < result.size(); i++) {
        if (sourceA[i] == YES) {
            result[i] = result[i] == NO ? YES : NO;
        }
    }
    return result;
}

// hashkey, string, 代表状态
// hashvalue, vector<size_t>, 代表选择了什么序号

// 输入一个std::span, 输出一个集合, 其中key为状态, value为到达状态的node列表
std::unordered_map<string, vector<size_t> > visit(std::span<const vector<size_t>> clicks, size_t light_size) {
    const string init_state = string(light_size, LIGHT::NO);
    std::unordered_map<string, vector<size_t> > nodes_map;
    nodes_map.emplace(init_state, std::vector<size_t>{});
    size_t order{0};
    for (const auto &click: clicks) {
        std::unordered_map<string, vector<size_t> > temp_nodes_map;
        // 不添加的就是set里的原装
        for (const auto &[state, nodes]: nodes_map) {
            const auto new_state = add(state, click);
            vector<size_t> new_nodes = {nodes.cbegin(), nodes.cend()};
            new_nodes.push_back(order); // 不保存中间结果的话直接存int作为size
            if (!temp_nodes_map.contains(new_state)) {
                temp_nodes_map[new_state] = new_nodes;
            } else {
                const auto new_nodes_size = new_nodes.size();
                const auto temp_old_nodes_size = temp_nodes_map.at(new_state).size();
                if (new_nodes_size < temp_old_nodes_size) {
                    temp_nodes_map[new_state] = new_nodes;
                }
            }
            //}
        }
        for (const auto &[state, nodes]: temp_nodes_map) {
            if (!nodes_map.contains(state)) {
                nodes_map[state] = nodes;
            } else {
                const auto old_nodes_size = nodes_map.at(state).size();
                const auto new_nodes_size = nodes.size();
                if (new_nodes_size < old_nodes_size) {
                    nodes_map[state] = nodes;
                }
            }
        }
        order++;
    }
    return nodes_map;
}

output_type cal(const input_type &datas) {
    // 按顺序分割成两部分, 分别求可能性, 再两两组合
    int64_t result{0};
    for (const auto &data: datas) {
        size_t min_result{0x3f3f3f3f};
        const auto lights = data.lights.size();
        const auto nodes = data.clicks.size();
        const auto half_nodes = nodes / 2;
        const auto left = std::span(data.clicks.cbegin(), data.clicks.cbegin() + half_nodes);
        const auto right = std::span(data.clicks.cbegin() + half_nodes, data.clicks.cend());
        const auto left_state = visit(left, lights);
        const auto right_state = visit(right, lights);
        if (left_state.contains(data.lights)) {
            min_result = left_state.at(data.lights).size();
        }
        if (right_state.contains(data.lights)) {
            min_result = std::min(min_result, right_state.at(data.lights).size());
        }
        // 都没有直接相加的情况
        for (const auto &[left_key, left_nodes]: left_state) {
            const auto need_right = sub(data.lights, left_key);
            if (right_state.contains(need_right)) {
                const auto right_nodes = right_state.at(need_right);
                min_result = std::min(min_result, left_nodes.size() + right_nodes.size());
            }
        }
        if (result != 0x3f3f3f3f) {
            result += min_result;
        }
    }
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
