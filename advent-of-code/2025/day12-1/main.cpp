// SPDX-License-Identifier: AGPL-3.0-or-later
// SPDX-FileCopyrightText: 2025 nanoseeds

#include <tuple>
#include <vector>
#include <iostream>
#include <string>
#include <sstream>
#include <set>
#include <algorithm>

#ifdef ALGORITHM_TEST_MACRO
namespace day12_1 {
#endif
using std::cin;
using std::cout;
using std::tie;
using std::tuple;
using std::vector;
using std::string;
using std::pair;
using std::set;
using std::istringstream;
static constexpr const char next_line{'\n'};

// 表示一个形状的所有 '#' 位置（相对于某个锚点）
using Shape = vector<pair<int, int>>;

// 区域定义
struct Region {
    int width;
    int height;
    vector<int> counts;  // counts[i] = 需要放置多少个形状 i
};

// 输入数据结构
struct InputData {
    vector<Shape> baseShapes;           // 原始形状
    vector<vector<Shape>> allVariants;  // allVariants[i] = shape i 的所有变体
    vector<Region> regions;             // 所有区域
};

using input_type = InputData;
using output_type = int32_t;

static bool is_digits(const string& s) {
    if (s.empty()) return false;
    for (const char ch : s) {
        if (ch < '0' || ch > '9') return false;
    }
    return true;
}

// 将形状标准化（平移到原点附近，排序去重）
Shape normalize(Shape s) {
    if (s.empty()) return s;
    int minR = s[0].first, minC = s[0].second;
    for (auto& p : s) {
        minR = std::min(minR, p.first);
        minC = std::min(minC, p.second);
    }
    for (auto& p : s) {
        p.first -= minR;
        p.second -= minC;
    }
    std::sort(s.begin(), s.end());
    return s;
}

// 生成形状的所有旋转和翻转变体
vector<Shape> generateVariants(const Shape& original) {
    set<Shape> variants;
    Shape current = original;

    for (int flip = 0; flip < 2; flip++) {
        for (int rot = 0; rot < 4; rot++) {
            variants.insert(normalize(current));
            // 顺时针旋转90度: (r, c) -> (c, -r)
            Shape rotated;
            for (auto& p : current) {
                rotated.push_back({p.second, -p.first});
            }
            current = rotated;
        }
        // 水平翻转: (r, c) -> (r, -c)
        for (auto& p : current) {
            p.second = -p.second;
        }
    }

    return vector<Shape>(variants.begin(), variants.end());
}

// 解析形状定义
Shape parseShape(const vector<string>& lines) {
    Shape s;
    for (int r = 0; r < static_cast<int>(lines.size()); r++) {
        for (int c = 0; c < static_cast<int>(lines[r].size()); c++) {
            if (lines[r][c] == '#') {
                s.push_back({r, c});
            }
        }
    }
    return normalize(s);
}

// 检查形状是否可以放在 (row, col) 位置
bool canPlace(const vector<vector<int>>& grid, const Shape& shape, int row, int col) {
    int H = static_cast<int>(grid.size());
    int W = static_cast<int>(grid[0].size());
    for (const auto& p : shape) {
        int r = row + p.first;
        int c = col + p.second;
        if (r < 0 || r >= H || c < 0 || c >= W || grid[r][c] != 0) {
            return false;
        }
    }
    return true;
}

// 放置形状，标记为 markId
void placeShape(vector<vector<int>>& grid, const Shape& shape, int row, int col, int markId) {
    for (const auto& p : shape) {
        grid[row + p.first][col + p.second] = markId;
    }
}

// 移除形状
void removeShape(vector<vector<int>>& grid, const Shape& shape, int row, int col) {
    for (const auto& p : shape) {
        grid[row + p.first][col + p.second] = 0;
    }
}

// 找到第一个空位（从左到右，从上到下）
pair<int, int> findFirstEmpty(const vector<vector<int>>& grid) {
    for (int r = 0; r < static_cast<int>(grid.size()); r++) {
        for (int c = 0; c < static_cast<int>(grid[0].size()); c++) {
            if (grid[r][c] == 0) return {r, c};
        }
    }
    return {-1, -1};
}

static int total_piece_cells(const vector<int>& remaining, const vector<Shape>& baseShapes) {
    int total = 0;
    const auto n = std::min(remaining.size(), baseShapes.size());
    for (size_t i = 0; i < n; ++i) {
        total += remaining[i] * static_cast<int>(baseShapes[i].size());
    }
    return total;
}

static pair<int, int> variant_hw(const Shape& variant) {
    int max_r = 0;
    int max_c = 0;
    for (const auto& p : variant) {
        max_r = std::max(max_r, p.first);
        max_c = std::max(max_c, p.second);
    }
    return {max_r + 1, max_c + 1};
}

static bool any_variant_fits(const vector<Shape>& variants, int H, int W) {
    for (const auto& v : variants) {
        const auto [hh, ww] = variant_hw(v);
        if (hh <= H && ww <= W) return true;
    }
    return false;
}

// 回溯求解：按“件”放置，区域允许留空
static bool solve_by_shape(vector<vector<int>>& grid,
                           vector<int>& remaining,
                           const vector<int>& order,
                           const vector<vector<Shape>>& allVariants,
                           int H, int W) {

    int shapeIdx = -1;
    for (const int idx : order) {
        if (idx >= 0 && idx < static_cast<int>(remaining.size()) && remaining[idx] > 0) {
            shapeIdx = idx;
            break;
        }
    }
    if (shapeIdx == -1) return true;

    for (const auto& variant : allVariants[shapeIdx]) {
        const auto [hh, ww] = variant_hw(variant);
        if (hh > H || ww > W) continue;

        for (int r = 0; r + hh <= H; ++r) {
            for (int c = 0; c + ww <= W; ++c) {
                if (!canPlace(grid, variant, r, c)) continue;

                placeShape(grid, variant, r, c, shapeIdx + 1);
                --remaining[shapeIdx];

                if (solve_by_shape(grid, remaining, order, allVariants, H, W)) {
                    return true;
                }

                ++remaining[shapeIdx];
                removeShape(grid, variant, r, c);
            }
        }
    }

    return false;
}

// 处理单个区域
bool processRegion(const Region& region,
                   const vector<Shape>& baseShapes,
                   const vector<vector<Shape>>& allVariants) {
    vector<int> remaining = region.counts;

    // 确保 remaining 大小与形状数量一致
    while (remaining.size() < baseShapes.size()) {
        remaining.push_back(0);
    }

    // 剪枝：总面积不够放
    const int need = total_piece_cells(remaining, baseShapes);
    const int cap = region.width * region.height;
    if (need > cap) return false;

    // 剪枝：某个需要的形状无论如何都放不下（仅按尺寸判断）
    for (size_t i = 0; i < baseShapes.size() && i < remaining.size(); ++i) {
        if (remaining[i] > 0 && !any_variant_fits(allVariants[i], region.height, region.width)) {
            return false;
        }
    }

    // 优先放置面积更大的形状
    vector<int> order;
    order.reserve(baseShapes.size());
    for (int i = 0; i < static_cast<int>(baseShapes.size()); ++i) {
        order.push_back(i);
    }
    std::sort(order.begin(), order.end(), [&](int a, int b) {
        return baseShapes[a].size() > baseShapes[b].size();
    });

    // 创建网格并求解
    vector<vector<int>> grid(region.height, vector<int>(region.width, 0));

    return solve_by_shape(grid, remaining, order, allVariants, region.height, region.width);
}

inline input_type read();
output_type cal(const input_type& data);
void output(const output_type& data);

int main() {
    const auto input_data = read();
    const auto output_data = cal(input_data);
    output(output_data);
    return 0;
}

inline input_type read() {
    InputData data;

    auto trim_cr = [](string s) {
        if (!s.empty() && s.back() == '\r') s.pop_back();
        return s;
    };
    auto is_region_line = [](const string& s) {
        return s.find('x') != string::npos && s.find(':') != string::npos;
    };
    auto is_shape_header = [](const string& s) {
        const auto colon = s.find(':');
        if (colon == string::npos) return false;
        // 形状头通常是 "N:"，区域头是 "WxH:"（包含x）
        if (s.find('x') != string::npos) return false;
        return is_digits(s.substr(0, colon));
    };
    auto parseRegionLine = [](const string& regionLine) -> Region {
        Region region;
        const auto xPos = regionLine.find('x');
        const auto colonPos = regionLine.find(':');
        region.width = std::stoi(regionLine.substr(0, xPos));
        region.height = std::stoi(regionLine.substr(xPos + 1, colonPos - xPos - 1));
        istringstream iss(regionLine.substr(colonPos + 1));
        int cnt;
        while (iss >> cnt) {
            region.counts.push_back(cnt);
        }
        return region;
    };

    vector<string> lines;
    for (string line; std::getline(cin, line);) {
        lines.push_back(trim_cr(line));
    }

    size_t i = 0;
    // 解析形状块
    for (; i < lines.size(); ) {
        string line = lines[i];
        if (line.empty()) {
            ++i;
            continue;
        }
        if (is_region_line(line)) break;
        if (!is_shape_header(line)) {
            ++i;
            continue;
        }

        ++i; // 跳过 "N:"
        vector<string> shapeLines;
        for (; i < lines.size(); ++i) {
            const auto& s = lines[i];
            if (s.empty()) {
                ++i;
                break;
            }
            if (is_shape_header(s) || is_region_line(s)) {
                break;
            }
            shapeLines.push_back(s);
        }

        if (!shapeLines.empty()) {
            const auto shape = parseShape(shapeLines);
            data.baseShapes.push_back(shape);
            data.allVariants.push_back(generateVariants(shape));
        }
    }

    // 解析区域行
    for (; i < lines.size(); ++i) {
        const auto& line = lines[i];
        if (line.empty()) continue;
        if (is_region_line(line)) {
            data.regions.push_back(parseRegionLine(line));
        }
    }

    return data;
}

output_type cal(const input_type& data) {
    int32_t count = 0;

    for (const auto& region : data.regions) {
        if (processRegion(region, data.baseShapes, data.allVariants)) {
            count++;
        }
    }

    return count;
}

void output(const output_type& data) {
    cout << data << next_line;
}

static const auto faster_streams = [] {
    std::ios::sync_with_stdio(false);
    std::istream::sync_with_stdio(false);
    std::ostream::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);
    return 0;
}();

#ifdef ALGORITHM_TEST_MACRO
}
#endif
