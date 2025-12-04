// SPDX-License-Identifier: AGPL-3.0-or-later
// SPDX-FileCopyrightText: 2025 nanoseeds

#include <ranges>
#include <tuple>
#include <vector>
#include <iostream>
#include <ranges>
#include <string>
#include <deque>
#include <unordered_map>
#include <format>

#ifdef ALGORITHM_TEST_MACRO
namespace day3_1 {
#endif
using std::tie;
using std::cin;
using std::cout;
using std::tuple;
using std::vector;
using std::string;
using std::format;
static constexpr const char end{'\n'};
using num_t = int64_t;

using input_type = vector<vector<int8_t> >;
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
    vector<vector<int8_t> > inputs;
    for (std::string line; std::getline(std::cin, line);) {
        vector<int8_t> input;
        input.reserve(line.size());
        for (size_t i = 0; i < line.size(); ++i) {
            input.emplace_back(line[i] - '0');
        }
        inputs.push_back(input);
    }
    return inputs;
}

output_type one(const vector<int8_t> &range) {
    // 换个思路, 把这个列表视作多个子列表, 其中分成9个
    // key: 9, value: 最左侧的9的index, .... 最右侧的9的index
    // key: 8, the same
    // ...
    // key: 1
    // get max时只需要按照
    // 1. key from 9 to 1
    // each value from min to max 的顺序
    // get two nums 即可
    std::unordered_map<int8_t, std::deque<int64_t> > orders{};
    for (int i = 1; i <= 9; i++) {
        orders[i] = std::deque<int64_t>{};
    }
    // init done
    for (size_t i = 0; i < range.size(); ++i) {
        const auto num = range[i];
        orders[num].push_back(i);
    }
    // now get the num
    const auto [leftIndex, leftNum] = [&orders, range]() {
        for (int i = 9; i >= 1; --i) {
            if (orders.at(i).empty()) {
                continue;
            }
            const auto index = orders.at(i).front();
            if (index + 1 == range.size()) {
                // 显然不能取最右侧的
                continue;
            }
            orders.at(i).pop_front();
            return std::make_tuple(index, range.at(index));
        }
        return std::make_tuple<int64_t, int8_t>(0, 0);
    }();
    for (int i = 9; i >= 1; --i) {
        if (orders[i].empty()) {
            continue;
        }
        for (const auto index: orders[i]) {
            if (index > leftIndex) {
                const auto result = leftNum * 10 + i;
                return result;
            }
        }
    }
    // should not reach
    return -0x3f3f3f3f;
}

output_type two(const vector<int8_t> &range) {
    const auto range_but_not_rightest = --range.cend();
    int8_t leftNum = 0;
    size_t leftIndex = 0;
    for (auto iter = range.cbegin(); iter != range_but_not_rightest; ++iter) {
        if (*iter > leftNum) {
            leftNum = *iter;
            leftIndex = std::distance(range.cbegin(), iter);
        }
    }
    int8_t rightNum = 0;
    for (auto iter = range.cbegin() + leftIndex + 1; iter != range.cend(); ++iter) {
        if (*iter > rightNum) {
            rightNum = *iter;
        }
    }
    return leftNum * 10 + rightNum;
}

output_type cal(const input_type &data) {
    int64_t results{0};
    for (const auto &range: data) {
        const auto result = one(range);
        results += result;
    }
    return results;
}

void output(const output_type &data) {
    std::cout << data << end;
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
