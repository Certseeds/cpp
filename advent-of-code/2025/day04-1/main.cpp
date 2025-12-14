// SPDX-License-Identifier: AGPL-3.0-or-later
// SPDX-FileCopyrightText: 2025 nanoseeds

#include <tuple>
#include <vector>
#include <iostream>
#include <string>
#include <deque>
#include <format>
#include <cassert>

#ifdef ALGORITHM_TEST_MACRO
namespace day4_1 {
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

enum INPUT_CHAR{
    TRUE = '@',
    FALSE = '.',
};

inline input_type read() {
    vector<string> inputs;
    for (std::string line; std::getline(std::cin, line);) {
        inputs.push_back(line);
    }
    assert(!inputs.empty());
    assert(!inputs.front().empty());
    const auto rows = inputs.size();
    const auto columns = inputs.front().size();
    vector<vector<int8_t> > matrix(rows + 2, vector<int8_t>(columns + 2, 0));
    for (size_t i = 1; i <= rows; ++i) {
        const auto &line = inputs[i - 1];
        for (size_t j = 1; j <= columns; ++j) {
            matrix[i][j] = (line[j - 1] == INPUT_CHAR::TRUE) ? 1 : 0;
        }
    }
    return matrix;
}

input_type conv(const input_type &data) {
    const auto rows = data.size() - 2;
    const auto columns = data.front().size() - 2;
    vector matrix(rows + 2, vector<int8_t>(columns + 2, 0));
    for (size_t i = 1; i <= rows; ++i) {
        for (size_t j = 1; j <= columns; ++j) {
            matrix[i][j] += data[i - 1][j - 1];
            matrix[i][j] += data[i - 1][j];
            matrix[i][j] += data[i - 1][j + 1];
            matrix[i][j] += data[i][j - 1];
            // matrix[i][j] += data[i][j];
            matrix[i][j] += data[i][j + 1];
            matrix[i][j] += data[i + 1][j - 1];
            matrix[i][j] += data[i + 1][j];
            matrix[i][j] += data[i + 1][j + 1];
        }
    }
    return matrix;
}

output_type cal(const input_type &data) {
    const auto conv_result = conv(data);
    int64_t result{0};
    for (size_t i = 0; i < data.size(); ++i) {
        for (size_t j = 0; j < data.front().size(); ++j) {
            if (data[i][j] == 1 && conv_result[i][j] < 4) {
                result += 1;
                // std::cout << std::format("{},{}", i, j) <<  next_line;
            }
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
