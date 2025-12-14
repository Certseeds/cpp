// SPDX-License-Identifier: AGPL-3.0-or-later
// SPDX-FileCopyrightText: 2025 nanoseeds
#ifdef ALGORITHM_TEST_MACRO

#include <catch_main.hpp>
#include <iostream>
#include <tuple>
#include <vector>
#include <random>

#include "main.cpp"

std::string getFilePath() noexcept { return "./../../../../advent-of-code/2025/day03-1/resource/"; }

const std::string CS203_redirect::file_paths = getFilePath();

namespace day3_1 {
using std::cin;
using std::cout;
using std::tie;
using std::tuple;
using std::vector;

TEST_CASE("test case 1", "[2025 day3-1]") {
    SECTION("do") {
        const CS203_redirect cr{"01.data.in", "01.test.out"};
        const auto input_data = read();
        const auto output_data = cal(input_data);
        output(output_data);
    }
    SECTION("compare files") {
        CHECK(compareFiles("01.test.out", "01.data.out"));
    }
}

TEST_CASE("test case large", "[2025 day3-1][.]") {
    SECTION("do") {
        const CS203_redirect cr{"large.data.in", "large.test.out"};
        const auto input_data = read();
        const auto output_data = cal(input_data);
        output(output_data);
    }
    SECTION("compare files") {
        CHECK(compareFiles("large.test.out", "large.data.out"));

    }
}
}
#endif // ALGORITHM_TEST_MACRO
