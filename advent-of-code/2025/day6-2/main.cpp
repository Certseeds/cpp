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
#include <utility>
#ifdef ALGORITHM_TEST_MACRO
namespace day6_2 {
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

constexpr int8_t NO_CHAR{' '};

// 读取多行数字
// 返回从每一行location位置开始, 直到下一个 multiline位置都是 NO_CHAR的位置, 这一段组成的数字列表
std::tuple<vector<int64_t>, size_t> convert_mannum_to_cipher(const vector<string> &lines, const size_t location) {
    vector<int64_t> nums;
    size_t visit_location = location;
    while (true) {
        vector<int8_t> chars{};
        for (const auto &line: lines) {
            chars.push_back(line[visit_location]);
        }
        if (std::ranges::all_of(std::as_const(chars), [](const auto ch) { return ch == NO_CHAR; })) {
            return std::make_tuple(nums, visit_location - 1);
        }
        auto digits = chars | std::views::filter([](const auto ch) { return ch != NO_CHAR; });
        int64_t num{0};
        for (auto digit: digits) {
            num *= 10;
            num += (digit - '0');
        }
        nums.push_back(num);
        if (visit_location == 0) {
            // the leftest line result in here
            return std::make_tuple(nums, 0);
        }
        visit_location--;
    }
}

inline input_type read() {
    vector<string> num_ranges;
    size_t order{0};
    for (std::string line; std::getline(std::cin, line);) {
        if (line.contains('+') || line.contains('*')) {
            line = trim(line);
            const auto rline = string(line.crbegin(), line.crend());
            auto ops = std::views::split(rline, ' ');
            std::vector<std::tuple<std::vector<int64_t>, operation> > result{};
            for (auto &&opstr: ops) {
                const auto sv = std::string_view(opstr.cbegin(), opstr.cend());
                if (sv.empty()) {
                    continue;
                }
                const auto [cipher, next_order] = convert_mannum_to_cipher(num_ranges, order);
                order = next_order;
                if (sv == "+") {
                    result.emplace_back(cipher, operation::ADD);
                } else if (sv == "*") {
                    result.emplace_back(cipher, operation::MULTIPLY);
                } else {
                    throw std::invalid_argument("Invalid operation");
                }
            }
            return result;
        }
        order = line.size() - 1;
        num_ranges.push_back(line);
    }
    return {};
}


output_type cal(const input_type &data) {
    __int128_t result{0};
    for (const auto &[nums, op]: data) {
        switch (op) {
            case operation::ADD: {
                __int128_t sum{0};
                //std::cout << "ADD: ";
                for (const auto num: nums) {
                    sum += num;
                    //std::cout << std::format("{} + ", num);
                }
                //std::cout << std::format("0 = {}", sum) << next_line;
                result += sum;
                break;
            }
            case operation::MULTIPLY: {
                __int128_t mul{1};
                //std::cout << "MUL: ";
                for (const auto num: nums) {
                    mul *= num;
                    //std::cout << std::format("{} * ", num);
                }
                //std::cout << std::format("1 = {}", mul) << next_line;
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
