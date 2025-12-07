// SPDX-License-Identifier: AGPL-3.0-or-later
// SPDX-FileCopyrightText: 2025 nanoseeds

#include <ranges>
#include <tuple>
#include <vector>
#include <iostream>
#include <string>
#include <deque>
#include <format>

#ifdef ALGORITHM_TEST_MACRO
namespace day3_1 {
#endif
using std::cin;
using std::cout;
using std::tuple;
using std::vector;
using std::string;
using std::format;
static constexpr const char next_line{'\n'};
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

// 递归的考虑在还剩下 remains个的情况下, 应该在前面前几个选哪一个数字
// 重点是前面的选择和后面的没有关系
std::vector<int8_t> two(std::span<const int8_t> range, size_t remains) {
    if (remains == 0) {
        return {};
    }
    const auto range_but_not_rightest = range.cend() - remains + 1;
    int8_t leftNum = 0;
    size_t leftIndex = 0;
    for (auto iter = range.cbegin(); iter != range_but_not_rightest; ++iter) {
        if (*iter > leftNum) {
            leftNum = *iter;
            leftIndex = std::distance(range.cbegin(), iter);
        }
    }
    auto rightValues = two(std::span(range.cbegin() + leftIndex + 1, range.cend()), remains - 1);
    rightValues.push_back(leftNum);
    return rightValues;
}

output_type cal(const input_type &data) {
    int64_t results{0};
    for (const auto &range: data) {
        const auto spa = std::span{range.cbegin(), range.cend()};
        auto result = two(spa, 12);
        int64_t sums = 0;
        int64_t multi = 1;
        for (int i = 0; i < 12; i++) {
            sums += result[i] * multi;
            multi *= 10;
            // result[i] += '0';
        }

        // std::cout << format("result for range {} is {}\n", std::string(result.crbegin(), result.crend()), sums);
        results += sums;
    }
    return results;
}

void output(const output_type &data) {
    std::cout << data <<  next_line;
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
