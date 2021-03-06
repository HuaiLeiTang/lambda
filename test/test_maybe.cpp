
#include "gmock/gmock.h"
#include "lambda.hpp"

using namespace lambda;
using namespace lambda::monad;

auto id_m(int n) { return n; }
auto id_s(int n) { return std::to_string(n); }

TEST(lambda, maybe) {
    let num = some(1024);
    ASSERT_TRUE(bool(num));

    let n = num >> id_m >> id_s >> ([](auto s) { return s + "!"; });

    display << n << 1 << "\n";

    ASSERT_EQ("1024!", *n);
}
