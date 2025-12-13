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
#include <complex>
#include <queue>
#include <set>

#ifdef ALGORITHM_TEST_MACRO
namespace day9_1 {
#endif
using std::cin;
using std::cout;
using std::tuple;
using std::vector;
using std::string;
using std::format;
static constexpr const char next_line{'\n'};

struct point {
    int64_t x;
    int64_t y;

    // 点积重载
    friend int64_t operator*(const point &lhs, const point &rhs) {
        return (std::abs(lhs.x - rhs.x) + 1) * (std::abs(lhs.y - rhs.y ) + 1);
    }
};

using input_type = std::vector<point>;
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
    vector<point> datas;
    for (std::string line; std::getline(std::cin, line);) {
        auto ranges = std::views::split(line, ',');
        auto iter = ranges.cbegin();
        const auto x_str = *iter;
        ++iter;
        const auto y_str = *iter;
        const auto x = std::stoll(std::string(x_str.cbegin(), x_str.cend()));
        const auto y = std::stoll(std::string(y_str.cbegin(), y_str.cend()));
        datas.emplace_back(x, y);
    }
    return datas;
}

struct distance {
    size_t p1;
    size_t p2;
    int64_t length{0};
};

// 叉积: (O->A) × (O->B)
// 正值: B在OA的逆时针方向
// 负值: B在OA的顺时针方向
// 零: 三点共线
int64_t cross(const point& O, const point& A, const point& B) {
    return (A.x - O.x) * (B.y - O.y) - (A.y - O.y) * (B.x - O.x);
}

// Andrew's monotone chain algorithm 构建凸包
// 返回凸包顶点 (逆时针顺序)
// 时间复杂度: O(n log n)
vector<point> convex_hull(vector<point> points) {
    const auto n = points.size();
    if (n < 3) return points;

    // 按 (x, y) 字典序排序
    std::ranges::sort(points, [](const auto& a, const auto& b) {
        return a.x != b.x ? a.x < b.x : a.y < b.y;
    });

    vector<point> hull;
    hull.reserve(2 * n);

    // 构建下凸包
    for (size_t i = 0; i < n; ++i) {
        while (hull.size() >= 2 && cross(hull[hull.size()-2], hull[hull.size()-1], points[i]) <= 0) {
            hull.pop_back();
        }
        hull.push_back(points[i]);
    }

    // 构建上凸包
    const auto lower_size = hull.size();
    for (size_t i = n - 1; i > 0; --i) {
        while (hull.size() > lower_size && cross(hull[hull.size()-2], hull[hull.size()-1], points[i-1]) <= 0) {
            hull.pop_back();
        }
        hull.push_back(points[i-1]);
    }

    hull.pop_back(); // 移除重复的起点
    return hull;
}

// 在凸包点上枚举，找最大矩形面积
// 时间复杂度: O(h^2), h 是凸包点数
// 最优解一定在凸包上的两点之间
int64_t max_rectangle_on_hull(const vector<point>& hull) {
    const auto h = hull.size();
    if (h < 2) return 0;

    int64_t max_area = 0;
    for (size_t i = 0; i < h; ++i) {
        for (size_t j = i + 1; j < h; ++j) {
            max_area = std::max(max_area, hull[i] * hull[j]);
        }
    }
    return max_area;
}

int64_t N2(const std::span<point> points) {
    const auto sizes = points.size();
    static constexpr auto cmp{
        [](const auto v1, const auto v2) {
            return v1.length < v2.length;
        }
    };
    std::priority_queue<distance, std::vector<distance>, decltype(cmp)> distances(cmp);
    for (size_t i = 0; i < sizes; ++i) {
        for (size_t j = 0; j < sizes; ++j) {
            if (i < j) {
                distances.push(distance{i, j, points[i] * points[j]});
            }
        }
    }
    const auto head = distances.top();
    return head.length;
}

output_type cal(const input_type &datas) {
    // 方法0: O(n) 极值点法 (最优, 利用数据特征)

    const auto hull = convex_hull(vector<point>(datas));
    const auto result = max_rectangle_on_hull(hull);
    return result;

    // 方法2: 暴力 O(n^2) (保留作为对比)
    // vector<point> mut_datas{datas};
    // const auto result = N2(mut_datas);
    // return result;
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
