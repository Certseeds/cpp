// SPDX-License-Identifier: AGPL-3.0-or-later
// SPDX-FileCopyrightText: 2025 nanoseeds

#include <algorithm>
#include <ranges>
#include <tuple>
#include <vector>
#include <iostream>
#include <string>
#include <deque>
#include <unordered_map>
#include <format>
#include <cassert>
#ifdef ALGORITHM_TEST_MACRO
namespace day7_1 {
#endif
using std::cin;
using std::cout;
using std::tuple;
using std::vector;
using std::string;
using std::format;
static constexpr const char next_line{'\n'};

enum class node: int8_t {
    BEGIN = 0,
    EMPTY = 1,
    SPLITTER = 2,
    QUICK = 3,
};

const std::unordered_map<int8_t, node> umap{
    {'S', node::BEGIN},
    {'.', node::EMPTY},
    {'^', node::SPLITTER},
};

using input_type = std::vector<vector<node> >;
using output_type = int64_t;

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
    vector<vector<node> > datas;
    for (std::string line; std::getline(std::cin, line);) {
        vector<node> data;
        data.reserve(line.size());
        for (const auto ch: line) {
            assert(ch == 'S' || ch == '.' || ch == '^');
            data.push_back(umap.at(ch));
        }
        datas.push_back(data);
    }
    return datas;
}

int64_t move(const vector<node> &fst, vector<node> &snd) {
    const auto length = fst.size();
    int64_t result{0};
    for (size_t i = 0; i < length; ++i) {
        if (fst[i] == node::BEGIN) {
            snd[i] = node::QUICK;
            // only once
        } else if (fst[i] == node::EMPTY) {
            // do nothing
        } else if (fst[i] == node::SPLITTER) {
            // do nothing
        } else if (fst[i] == node::QUICK) {
            if (snd[i] == node::EMPTY) {
                snd[i] = node::QUICK;
            } else if (snd[i] == node::SPLITTER) {
                if (i >= 1 && snd[i - 1] == node::EMPTY) {
                    snd[i - 1] = node::QUICK;
                }
                if (i < length - 1 && snd[i + 1] == node::EMPTY) {
                    snd[i + 1] = node::QUICK;
                }
                result += 1;
            }
        }
    }
    return result;
}
string printvector(const vector<node> &vec) {
    string result;
    for (const auto &n: vec) {
        switch (n) {
            case node::BEGIN: {
                result += 'S';
                break;
            }
            case node::EMPTY: {
                result += '.';
                break;
            }
            case node::SPLITTER: {
                result += '^';
                break;
            }
            case node::QUICK: {
                result += '|';
                break;
            }
            default: {
                throw std::invalid_argument("Invalid node");
            }
        }
    }
    return result;
}

output_type cal(const input_type &datas) {
    vector<vector<node> > mut_datas{datas};
    int64_t result{0};
    for (size_t i = 0; i + 1 < datas.size(); ++i) {
        const auto fst_line = mut_datas[i];
        auto& snd_line = mut_datas[i + 1];
        result += move(fst_line, snd_line);
    }
    // for (const auto &n: mut_datas) {
    //     std::cout << printvector(n) << next_line;
    // }
    return result;
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
