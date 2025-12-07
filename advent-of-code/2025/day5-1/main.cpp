// SPDX-License-Identifier: AGPL-3.0-or-later
// SPDX-FileCopyrightText: 2025 nanoseeds

#include <algorithm>
#include <tuple>
#include <vector>
#include <iostream>
#include <string>
#include <deque>
#include <format>
#include <cassert>
#ifdef ALGORITHM_TEST_MACRO
namespace day5_1 {
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

using input_type = std::tuple<vector<ranges_input>, vector<int64_t> >;
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
    vector<int64_t> ids;
    bool find_empty{false};
    for (std::string line; std::getline(std::cin, line);) {
        if (line.empty()) {
            find_empty = true;
            continue;
        }
        if (!find_empty) {
            const auto dash_pos = line.find('-');
            const auto begin = std::stol(line.substr(0, dash_pos));
            const auto end = std::stol(line.substr(dash_pos + 1));
            idrange.emplace_back(begin, end);
        } else {
            const auto id = std::stol(line);
            ids.push_back(id);
        }
    }
    return std::make_tuple(idrange, ids);
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

output_type two_pointer(const input_type &data) {
    const auto &[id_range, ids] = data;
    auto mut_ids = ids;
    int64_t result{0};
    const auto sorted_range = mergeIdRange(id_range);
    std::ranges::sort(mut_ids);
    size_t ranges_index{0}, ids_index{0};
    while (ranges_index != sorted_range.size() && ids_index != mut_ids.size()) {
        const auto &[begin, end] = sorted_range[ranges_index];
        if (const auto id = mut_ids[ids_index]; id < begin) {
            // id 不在范围内
            ids_index += 1;
        } else if (id > end) {
            // id 超出范围
            ranges_index += 1;
        } else {
            // id 在范围内
            ids_index += 1;
            result += 1;
            // std::cout << std::format("id {} in range {}-{}", id, begin, end) << next_line;
        }
    }
    return result;
}

output_type iter(const input_type &data) {
    const auto &[id_range, ids] = data;
    auto mut_ids = ids;
    std::ranges::sort(mut_ids);

    int64_t result{0};
    for (const auto id: mut_ids) {
        for (const auto &[begin, end]: id_range) {
            if (begin <= id && id <= end) {
                // std::cout << std::format("id {} in range {}-{}", id, begin, end) << next_line;
                result += 1;
                break;
            }
        }
    }
    return result;
}

output_type cal(const input_type &data) {
    return two_pointer(data);
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
