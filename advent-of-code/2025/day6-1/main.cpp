// SPDX-License-Identifier: AGPL-3.0-or-later
// SPDX-FileCopyrightText: 2025 nanoseeds

#include <algorithm>
#include <ranges>
#include <tuple>
#include <vector>
#include <iostream>
#include <ranges>
#include <string>
#include <deque>
#include <unordered_map>
#include <format>
#include <cassert>
#ifdef ALGORITHM_TEST_MACRO
namespace day6_1 {
#endif
using std::tie;
using std::cin;
using std::cout;
using std::tuple;
using std::vector;
using std::string;
using std::format;
static constexpr const char next_line{'\n'};

enum class operation: int8_t {
    ADD = 0,
    MULTIPLY = 1
};

using input_type = std::vector<std::tuple<std::vector<int64_t>, operation> >;
using output_type = string;

inline input_type read();

output_type cal(const input_type &data);

void output(const output_type &data);

int main() {
    const auto input_data = read();
    const auto output_data = cal(input_data);
    output(output_data);
    return 0;
}

std::string trim(const std::string &s) {
    const auto start = std::find_if_not(s.begin(), s.end(), [](unsigned char ch) { return std::isspace(ch); });
    const auto end = std::find_if_not(s.rbegin(), s.rend(), [](unsigned char ch) { return std::isspace(ch); }).base();
    if (start >= end) return "";
    return std::string(start, end);
}

inline input_type read() {
    vector<vector<int64_t> > num_ranges;
    for (std::string line; std::getline(std::cin, line);) {
        line = trim(line);
        if (line.contains('+') || line.contains('*')) {
            auto ops = std::views::split(line, ' ');
            size_t order{0};
            std::vector<std::tuple<std::vector<int64_t>, operation> > result{};
            for (auto &&opstr: ops) {
                const auto sv = std::string_view(opstr.cbegin(), opstr.cend());
                if (sv.empty()) {
                    continue;
                }
                vector<int64_t> nums{};
                for (const auto &range: num_ranges) {
                    nums.push_back(range[order]);
                }
                if (sv == "+") {
                    result.emplace_back(nums, operation::ADD);
                } else if (sv == "*") {
                    result.emplace_back(nums, operation::MULTIPLY);
                } else {
                    throw std::invalid_argument("Invalid operation");
                }
                order++;
            }
            return result;
        }
        auto result = std::views::split(line, ' ');
        vector<int64_t> nums;
        for (auto &&numstr: result) {
            std::string_view sv(&*numstr.begin(), std::ranges::distance(numstr));
            if (sv.empty()) {
                continue;
            }
            nums.push_back(std::stoll(sv.data(), nullptr, 10));
        }
        num_ranges.push_back(std::move(nums));
    }
    return {};
}


output_type cal(const input_type &data) {
    __int128_t result{0};
    for (const auto &[nums, op]: data) {
        switch (op) {
            case operation::ADD: {
                __int128_t sum{0};
                for (const auto num: nums) {
                    sum += num;
                }
                result += sum;
                break;
            }
            case operation::MULTIPLY: {
                __int128_t mul{1};
                for (const auto num: nums) {
                    mul *= num;
                }
                result += mul;
                break;
            }
            default: {
                throw std::invalid_argument("Invalid operation");
            }
        }
    }
    return std::format("{}", result);
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
