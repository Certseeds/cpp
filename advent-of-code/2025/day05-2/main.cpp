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
#ifdef ALGORITHM_TEST_MACRO
namespace day5_2 {
#endif
using std::cin;
using std::cout;
using std::tuple;
using std::vector;
using std::string;
using std::format;
static constexpr const char next_line{'\n'};
using num_t = int64_t;

struct ranges_input {
    num_t begin;
    num_t end;
};

using input_type = vector<ranges_input>;
using output_type = num_t;

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
    vector<ranges_input> idrange;
    for (std::string line; std::getline(std::cin, line);) {
        if (line.empty()) {
            break;  // 遇到空行停止读取区间，但继续消耗剩余输入
        }
        const auto dash_pos = line.find('-');
        if (dash_pos == std::string::npos) {
            break;  // 如果没有 '-'，说明进入了第二部分，停止
        }
        const auto begin = std::stol(line.substr(0, dash_pos));
        const auto end = std::stol(line.substr(dash_pos + 1));
        idrange.emplace_back(begin, end);
    }
    return idrange;
}

vector<ranges_input> mergeIdRange(vector<ranges_input> input) {
    if (input.size() < 2) {
        return input;
    }
    assert(input.size() >=2);
    std::ranges::sort(input, [](auto left, auto right) {
        if (left.begin != right.begin) {
            return left.begin < right.begin;
        }
        return left.end < right.end;
    });
    vector<ranges_input> result;
    ranges_input element = input[0];
    for (auto iter = input.cbegin() + 1; iter != input.cend(); ++iter) {
        if (const auto head = *iter; element.end + 1 >= head.begin) {
            element.end = std::max(head.end, element.end);
        } else {
            result.push_back(element);
            element = head;
        }
    }
    result.push_back(element);
    return result;
}


output_type cal(const input_type &data) {
    int64_t result{0};
    const auto sorted_range = mergeIdRange(data);
    for (const auto [begin, end]: sorted_range) {
        result += (end - begin + 1);
        // std::cout << std::format("[{},{}] diff is {}", begin, end, end - begin + 1) << next_line;
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
