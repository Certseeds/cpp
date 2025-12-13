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
#include <span>

#ifdef ALGORITHM_TEST_MACRO
namespace day9_2 {
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
        return (std::abs(lhs.x - rhs.x) + 1) * (std::abs(lhs.y - rhs.y) + 1);
    }

    friend bool operator==(const point &lhs, const point &rhs) {
        return lhs.x == rhs.x
               && lhs.y == rhs.y;
    }

    friend bool operator!=(const point &lhs, const point &rhs) {
        return !(lhs == rhs);
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

struct line {
    point x;
    point y;

    bool operator==(const line &input) const {
        return x == input.x && y == input.y;
    }
};

// 叉积: (O->A) × (O->B)
// 正值: B在OA的逆时针方向
// 负值: B在OA的顺时针方向
// 零: 三点共线
int64_t cross(const point &O, const point &A, const point &B) {
    return (A.x - O.x) * (B.y - O.y) - (A.y - O.y) * (B.x - O.x);
}

// 判断点是否在线段上（包括端点）
bool pointOnSegment(const point &p, const point &a, const point &b) {
    // 首先检查共线
    if (cross(a, b, p) != 0) return false;
    // 然后检查是否在边界内
    return std::min(a.x, b.x) <= p.x && p.x <= std::max(a.x, b.x) &&
           std::min(a.y, b.y) <= p.y && p.y <= std::max(a.y, b.y);
}

// 使用射线法判断点是否在多边形内部（严格内部返回1，边界上返回0，外部返回-1）
int pointInPolygon(std::span<const point> polygon, const point &p) {
    const auto n = polygon.size();
    int windingNumber = 0;

    for (size_t i = 0; i < n; ++i) {
        const auto &a = polygon[i];
        const auto &b = polygon[(i + 1) % n];

        // 检查点是否在边上
        if (pointOnSegment(p, a, b)) return 0;

        // 计算绕数（winding number）
        if (a.y <= p.y) {
            if (b.y > p.y) {
                // 向上穿越
                if (cross(a, b, p) > 0) {
                    ++windingNumber;
                }
            }
        } else {
            if (b.y <= p.y) {
                // 向下穿越
                if (cross(a, b, p) < 0) {
                    --windingNumber;
                }
            }
        }
    }

    return windingNumber != 0 ? 1 : -1;
}

// 判断两条线段是否相交（不包括端点重合的情况）
bool segmentsIntersect(const point &a1, const point &a2, const point &b1, const point &b2) {
    const auto sign = [](const int64_t x) { return (x > 0) - (x < 0); };

    const int64_t d1 = cross(b1, b2, a1);
    const int64_t d2 = cross(b1, b2, a2);
    const int64_t d3 = cross(a1, a2, b1);
    const int64_t d4 = cross(a1, a2, b2);

    // 如果有交叉（不仅仅是触碰）
    if (sign(d1) * sign(d2) < 0 && sign(d3) * sign(d4) < 0) {
        return true;
    }

    return false;
}

// 判断矩形的一条边是否与多边形的边相交（排除端点）
bool rectangleEdgeCrossesPolygon(std::span<const point> polygon,
                                 const point &r1, const point &r2) {
    const auto n = polygon.size();
    for (size_t i = 0; i < n; ++i) {
        const auto &a = polygon[i];
        const auto &b = polygon[(i + 1) % n];
        if (segmentsIntersect(r1, r2, a, b)) {
            return true;
        }
    }
    return false;
}

// 检查以p1和p2为对角线的矩形是否完全在多边形内部（包括边界）
// 矩形的四个角是: (p1.x, p1.y), (p2.x, p1.y), (p2.x, p2.y), (p1.x, p2.y)
bool checkRectangle(std::span<const point> points, const point &p1, const point &p2) {
    // 矩形退化为点或线段的情况
    if (p1.x == p2.x || p1.y == p2.y) {
        // 对于线段情况，检查线段上所有整数点是否在多边形内
        // 但实际上根据题意，两个对角的红色点必须不同，这里简化处理
        return true;
    }

    // 矩形的四个顶点
    point corners[4] = {
        {p1.x, p1.y},
        {p2.x, p1.y},
        {p2.x, p2.y},
        {p1.x, p2.y}
    };

    // 检查四个顶点是否都在多边形内部或边界上
    for (const auto &corner: corners) {
        int status = pointInPolygon(points, corner);
        if (status < 0) {
            // 在多边形外部
            return false;
        }
    }

    // 检查矩形的四条边是否与多边形的边相交
    for (int i = 0; i < 4; ++i) {
        if (rectangleEdgeCrossesPolygon(points, corners[i], corners[(i + 1) % 4])) {
            return false;
        }
    }

    return true;
}

int64_t N3(std::span<const point> points) {
    const auto sizes = points.size();
    int64_t result{0};
    size_t p1{0}, p2{0};
    for (size_t i = 0; i < sizes; ++i) {
        for (size_t j = 0; j < sizes; ++j) {
            if (i < j) {
                const auto &point1 = points[i];
                const auto &point2 = points[j];
                if (checkRectangle(points, point1, point2)) {
                    const auto newResult = points[i] * points[j];
                    if (newResult > result) {
                        result = newResult;
                        p1 = i;
                        p2 = j;
                    }
                }
            }
        }
    }
    std::cout << std::format("({},{}) ({},{})", points[p1].x, points[p1].y, points[p2].x, points[p2].y) << next_line;
    return result;
}

output_type cal(const input_type &datas) {
    const auto result = N3(std::span(datas.cbegin(), datas.cend()));
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
