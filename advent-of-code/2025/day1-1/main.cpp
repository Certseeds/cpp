// SPDX-License-Identifier: AGPL-3.0-or-later
// SPDX-FileCopyrightText: 2025 nanoseeds

#include <tuple>
#include <vector>
#include <iostream>
#include <string>

#ifdef ALGORITHM_TEST_MACRO
namespace lab_00_A {
#endif
using std::tie;
using std::cin;
using std::cout;
using std::tuple;
using std::vector;
using std::string;
static constexpr const char end{'\n'};

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
        location = location + time;
        location = location % maximum;
        if (location == 0) {
            result += 1;
        }
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
