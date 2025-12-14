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
namespace day10_2 {
#endif
using std::cin;
using std::cout;
using std::tuple;
using std::vector;
using std::string;
using std::format;
static constexpr const char next_line{'\n'};

class input_case {
public:
    vector<vector<size_t> > clicks;
    vector<size_t> joltage;

    static string tostring(const auto &vec) {
        string result = "{";
        for (const auto &item: vec) {
            result += std::to_string(item) + ",";
        }
        if (!vec.empty()) {
            result.pop_back(); // Remove last comma
        }
        result += "}";
        return result;
    }
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
        if (line.empty()) {
            break;
        }
        auto ranges = std::views::split(line, ' ');
        auto iter = ranges.cbegin();
        const auto light_str = *iter;
        std::vector<vector<size_t> > order_list;
        for (++iter; iter != ranges.cend(); ++iter) {
            const vector<size_t> clicks = num_list_to_vector(*iter);
            order_list.push_back(clicks);
        }
        const auto joltage = order_list.back();
        order_list.pop_back();
        datas.emplace_back(order_list, joltage);
    }
    return datas;
}

struct vectors {
    vector<size_t> click;
    size_t multi;
};

// input_state 添加上node之后的状态
// 注意, 将输入的(3) 处理为 [0,0,0,1]这种状态
vector<size_t> add(const vector<size_t> &input_state, const vectors &nodes) {
    assert(input_state.size() == nodes.click.size());
    vector result{input_state};
    for (size_t i = 0; i < result.size(); i++) {
        result[i] += (nodes.click[i] * nodes.multi);
    }
    return result;
}

vector<size_t> sub(const vector<size_t> &input_state, const vectors &nodes) {
    assert(input_state.size() == nodes.click.size());
    vector result{input_state};
    for (size_t i = 0; i < result.size(); i++) {
        result[i] -= (nodes.click[i] * nodes.multi);
    }
    return result;
}

bool overweidht(const vector<size_t> &target, const vectors &check) {
    assert(target.size() == check.click.size());
    const auto size = target.size();
    for (size_t i = 0; i < size; i++) {
        if (target[i] < check.click[i]) {
            return false;
        }
    }
    return true;
}


// 输入一个std::span, 输出一个集合, 其中key为状态, value为到达状态的node列表
std::unordered_map<string, std::tuple<vector<size_t>, size_t> > visit(std::span<const vectors> clicks,
                                                                      const vector<size_t> &target) {
    const vector<size_t> init_state(target.size(), 0);
    std::unordered_map<string, std::tuple<vector<size_t>, size_t> > nodes_map;
    const auto init_state_string = input_case::tostring(init_state);
    nodes_map.emplace(init_state_string, std::make_tuple(init_state, 0));
    size_t order{0};
    for (const auto &click: clicks) {
        std::unordered_map<string, std::tuple<vector<size_t>, size_t> > temp_nodes_map;
        // 不添加的就是set里的原装
        for (const auto &[state_string, nodes]: nodes_map) {
            const auto &[state, nodes_size] = nodes;
            const auto new_state = add(state, click);
            if (!overweidht(target, click)) {
                // 组合已经越界, 可以忽略
                continue;
            }
            const auto new_size = nodes_size + click.multi;
            const auto new_state_string = input_case::tostring(new_state);
            if (!temp_nodes_map.contains(new_state_string)) {
                temp_nodes_map[new_state_string] = std::make_tuple(new_state, new_size);
            } else {
                const auto &[old_state, old_size] = temp_nodes_map[new_state_string];
                if (new_size < old_size) {
                    temp_nodes_map[new_state_string] = std::make_tuple(new_state, new_size);
                }
            }
        }
        for (const auto &[state, nodes]: temp_nodes_map) {
            if (!nodes_map.contains(state)) {
                nodes_map[state] = nodes;
            } else {
                const auto &[old_nodes, old_size] = nodes_map.at(state);
                const auto &[new_nodes, new_size] = nodes;
                if (new_size < old_size) {
                    nodes_map[state] = nodes;
                }
            }
        }
        order++;
    }
      return nodes_map;
}


// {3,5,4,7}最多可以由 7个 {0,0,0,1}, 可以分拆成 {1,2,4}
vector<vectors> extend(const vector<size_t> &joltage, const vector<size_t> &click) {
    size_t max_combine{0x3f3f3f3f};
    assert(joltage.size() == click.size());
    for (size_t i = 0; i < joltage.size(); i++) {
        if (click[i] != 0) {
            max_combine = std::min(max_combine, joltage[i] / click[i]);
        }
    }
    vector<size_t> multis{};
    for (size_t base = 1; max_combine > base;) {
        multis.push_back(base);
        max_combine -= base;
        base *= 2;
    }
    if (max_combine > 0) {
        multis.push_back(max_combine);
    }
    vector<vectors> result{};
    for (const auto multi: multis) {
        result.emplace_back(click, multi);
    }
    return result;
}

vector<size_t> points_to_vector(std::span<const size_t> points_span, size_t length) {
    vector<size_t> result(length, 0);
    for (const auto point: points_span) {
        result[point] += 1;
    }
    return result;
}

output_type cal(const input_type &datas) {
    // 按顺序分割成两部分, 分别求可能性, 再两两组合
    int64_t result{0};
    for (const auto &data: datas) {
        size_t min_result{0x3f3f3f3f};
        vector<vectors> clicks_extend{};
        for (const auto &click: data.clicks) {
            const vector<size_t> click_vector = points_to_vector(click, data.joltage.size());
            const auto extended = extend(data.joltage, click_vector);
            clicks_extend.insert(clicks_extend.end(), extended.cbegin(), extended.cend());
        }

        const auto nodes = clicks_extend.size();
        const auto half_nodes = nodes / 2;
        const auto left = std::span(clicks_extend.cbegin(), clicks_extend.cbegin() + half_nodes);
        const auto right = std::span(clicks_extend.cbegin() + half_nodes, clicks_extend.cend());

        const auto &target = data.joltage;

        const auto left_state = visit(left, target);
        const auto right_state = visit(right, target);
        const auto target_string = input_case::tostring(target);
        if (left_state.contains(target_string)) {
            const auto &[state, size] = left_state.at(target_string);
            min_result = size;
        }
        if (right_state.contains(target_string)) {
            const auto &[state, size] = right_state.at(target_string);
            min_result = std::min(min_result, size);
        }
        // 都没有直接相加的情况
        for (const auto &[left_key, value]: left_state) {
            const auto &[node, size] = value;
            const auto right_vec = sub(data.joltage, vectors{node, 1});
            const auto right_str = input_case::tostring(right_vec);
            if (right_state.contains(right_str)) {
                const auto &[right_node, right_size] = right_state.at(right_str);
                min_result = std::min(min_result, size + right_size);
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
