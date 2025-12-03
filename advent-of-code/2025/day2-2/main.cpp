// SPDX-License-Identifier: AGPL-3.0-or-later
// SPDX-FileCopyrightText: 2025 nanoseeds

#include <tuple>
#include <vector>
#include <iostream>
#include <ranges>
#include <string>
#include <set>

#ifdef ALGORITHM_TEST_MACRO
namespace day2_2 {
#endif
using std::tie;
using std::cin;
using std::cout;
using std::tuple;
using std::vector;
using std::string;
static constexpr const char end{'\n'};
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
static const std::vector<std::tuple<int64_t, int64_t, int64_t, int64_t> > multiplys{
    std::make_tuple(10'000'000'000, 999'999999999, 1, 11111111111), // 1 num 11

    std::make_tuple(100'0000000, 999'9999999, 1, 111'111'111'1),
    std::make_tuple(100'0000000, 999'9999999, 10, 101010101),
    std::make_tuple(100'0000000, 999'9999999, 10000, 100001),

    std::make_tuple(10000'0000, 99999'9999, 1, 111'111'111),
    std::make_tuple(10000'0000, 99999'9999, 100, 1001001),

    std::make_tuple(1000'0000, 9999'9999, 1, 11111111),
    std::make_tuple(1000'0000, 9999'9999, 10, 1010101),
    std::make_tuple(1000'0000, 9999'9999, 1000, 10001),

    std::make_tuple(1000'000, 9999'999, 1, 1111111),

    std::make_tuple(100'000, 999'999, 100, 1001),
    std::make_tuple(100'000, 999'999, 10, 10101),
    std::make_tuple(100'000, 999'999, 1, 111111),

    std::make_tuple(10000, 99999, 1, 11111),

    std::make_tuple(10'00, 9999, 10, 101),
    std::make_tuple(10'00, 9999, 1, 1111),

    std::make_tuple(100, 999, 1, 111),
    std::make_tuple(10, 99, 1, 11),
};

output_type cal(const input_type &data) {
    int64_t result{0};
    for (const auto &range: data) {
        std::set<int64_t> added{};
        for (auto element: std::ranges::reverse_view(multiplys)) {
            const auto &[low, high, base, divide] = element;
            if (range.begin > high || range.end < low) {
                continue;
            }
            // [10, 11] => 11
            // [12, 22] => 22
            // [78 ,88] => 88
            // [89, 99] => 99
            const auto min_to_left = std::max(base * divide, // 这个区间内的最低值
                                              (range.begin / divide) * divide + (
                                                  range.begin % divide == 0 ? 0 : divide) // range.begin右边界上的最低值
            );
            // 这边有去重逻辑, 就不能简单的等差数列了
            const auto min_to_right = std::min(high, range.end);
            for (int64_t begin = min_to_left; begin <= min_to_right; begin += divide) {
                added.insert(begin); // 注意比如 222222, 会同时被 111111 * 2 以及 101 * 222, 10101 * 22 所表示
            }
        }
        result = std::accumulate(added.cbegin(), added.cend(), result);
    }
    return result;
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
