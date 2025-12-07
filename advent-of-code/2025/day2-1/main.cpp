// SPDX-License-Identifier: AGPL-3.0-or-later
// SPDX-FileCopyrightText: 2025 nanoseeds

#include <ranges>
#include <tuple>
#include <vector>
#include <iostream>
#include <string>
#include <cassert>

#ifdef ALGORITHM_TEST_MACRO
namespace day2_1 {
#endif
using std::cin;
using std::cout;
using std::tuple;
using std::vector;
using std::string;
static constexpr const char next_line{'\n'};
using num_t = int64_t;

struct range {
    num_t begin;
    num_t end;

    range(const int64_t begin, const int64_t end): begin(begin), end(end) {
    }
};

using input_type = vector<range>;
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
    std::string line{};
    std::getline(std::cin, line);
    vector<range> inputs;
    for (auto ranges = std::views::split(line, ',');
         const auto range: ranges) {
        auto nums = std::views::split(range, '-');
        auto iter = nums.cbegin();
        const auto left = *iter;
        ++iter;
        const auto right = *(iter);
        const auto lNum = std::stol(std::string(left.cbegin(), left.cend()));
        const auto rNum = std::stol(std::string(right.cbegin(), right.cend()));
        inputs.emplace_back(lNum, rNum);
    }
    return inputs;
}

// it just works
static constexpr std::array<std::tuple<int64_t, int64_t, int64_t, int64_t>, 8> multiplys{
    std::make_tuple(10000000'00000000, 99999999'99999999, 10000000, 100000001),
    std::make_tuple(1000000'0000000, 9999999'9999999, 1000000, 10000001),
    std::make_tuple(100000'000000, 999999'999999, 100000, 1000001),
    std::make_tuple(10000'00000, 99999'99999, 10000, 100001),
    std::make_tuple(1000'0000, 9999'9999, 1000, 10001),
    std::make_tuple(100'000, 999'999, 100, 1001),
    std::make_tuple(10'00, 9999, 10, 101),
    std::make_tuple(10, 99, 1, 11),
};

output_type cal(const input_type &data) {
    int64_t result{0};
    for (const auto &range: data) {
        for (auto element: std::ranges::reverse_view(multiplys)) {
            const auto &[low, high, base, divide] = element;
            if (range.begin > high || range.end < low) {
                continue;
            }
            // [10, 11] => 11
            // [12, 22] => 22
            // [78 ,88] => 88
            // [89, 99] => 99
            const auto min_to_left = std::max(base * divide,
                                              (range.begin / divide) * divide + (
                                                  range.begin % divide == 0 ? 0 : divide));
            const auto min_to_right = std::min(high, range.end);
            int64_t before = 0;
            for (int64_t begin = min_to_left; begin <= min_to_right; begin += divide) {
                before += begin;
            } // only for check and test
            const auto leftMin = (min_to_left - base * divide) / divide;
            const auto rightMin = ((min_to_right / divide) * divide - base * divide) / divide;
            const auto sums = (rightMin - leftMin + 1) * (leftMin + rightMin + 2 * base) * divide / 2;
            assert(before == sums);
            result += sums;
        }
    }
    return result;
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
