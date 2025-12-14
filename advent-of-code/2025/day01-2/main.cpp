// SPDX-License-Identifier: AGPL-3.0-or-later
// SPDX-FileCopyrightText: 2025 nanoseeds

#include <tuple>
#include <vector>
#include <iostream>
#include <string>
#include <cassert>
#ifdef ALGORITHM_TEST_MACRO
namespace day1_2 {
#endif
using std::cin;
using std::cout;
using std::tuple;
using std::vector;
using std::string;
static constexpr const char next_line{'\n'};

using num_t = int32_t;
using input_type = vector<string>;
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
    vector<string> inputs;
    for (std::string line; std::getline(std::cin, line);) {
        inputs.push_back(line);
    }
    return inputs;
}

constexpr const static auto init = 50;
constexpr const static auto maximum = 100;

enum RORATE: char {
    LEFT = 'L',
    RIGHT = 'R'
};

output_type cal(const input_type &data) {
    vector<int32_t> times{};
    for (const auto &line: data) {
        const auto fst = line[0];
        const auto remain = line.substr(1);
        const auto remain_num = std::stoi(remain);
        switch (fst) {
            case LEFT: {
                times.push_back(remain_num * -1);
                break;
            }
            case RIGHT: {
                times.push_back(remain_num);
                break;
            }
            default:
                throw std::invalid_argument("invalid fst");
        }
    }
    num_t location = init;
    num_t result = 0;
    for (const auto time: times) {
        const auto beforeLocation = location;
        assert(beforeLocation >= 0);
        assert(beforeLocation < maximum);
        location = location + time;
        if (time > 0) {
            const int quick_result = (location / maximum) - (beforeLocation / maximum);
            result += quick_result;
        }
        if (time < 0) {
            // (0, -1) to (0, -99) is all zero
            // (0, -100) to (0, -199) is all one
            // ----
            // (50, -1) to (50, -49) is all zero
            // (50, -50) to (50, -149) is all one
            // (50, -150) to (50, -249) is all two
            // ----
            // (99, -1) to (99, -98) is all zero
            // (99, -99) to (99, -198) is all one
            // (99, -199) to (99, -298) is all two
            // 1. if location still > 0, just return 0
            // if begin from 0, length/100 is result
            // if begin from 50, (length+50) / 100 is result
            // if begin from 99, (length+1) / 100 is result
            // 3. (length + (maximum - beforeLocation)%maximum) / 100)
            const auto quick_result =
                    location > 0 ? 0 : (time * -1 + (maximum - beforeLocation) % maximum) / maximum;
            result += quick_result;
        }
        location = (location % maximum + maximum) % maximum;
        // 判定末端位置是否为0
        //if (location == 0) {
        //    result += 1;
        //}
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
