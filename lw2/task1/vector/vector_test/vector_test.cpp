#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "../vector/vector.h"


TEST_CASE("Vector multiplication on min element") {
    // Multiply by 1 each element
    std::vector<double> vect = { 1., 2., 3. };
    REQUIRE(ProcessNumbers(vect) == std::vector<double>{1., 2., 3.});
    // One of the number equals zero
    vect = { 1., 2., 0. };
    REQUIRE(ProcessNumbers(vect) == std::vector<double>{0., 0., 0.});
    // One of the element is negative
    vect = { -1., 2., 3. };
    REQUIRE(ProcessNumbers(vect) == std::vector<double>{1., -2., -3.});
    // Empty vector
    vect = {};
    REQUIRE(ProcessNumbers(vect) == std::vector<double>{});
    // One element
    vect = { 6. };
    REQUIRE(ProcessNumbers(vect) == std::vector<double>{36.});
    // More quantity of elements
    vect = { 2., 2., 2., 2., 2. };
    REQUIRE(ProcessNumbers(vect) == std::vector<double>{4., 4., 4., 4., 4.});
    //Invalid arguments
    vect = { 2., INFINITY, 3. };
    REQUIRE_THROWS_AS(ProcessNumbers(vect), std::invalid_argument);
    vect = { 2., NAN, 3. };
    REQUIRE_THROWS_AS(ProcessNumbers(vect), std::invalid_argument);
    // 2 * MAX_DOUBLE 
    vect = { 2., std::numeric_limits<double>::max(), 3. };
    REQUIRE_THROWS_AS(ProcessNumbers(vect), std::out_of_range);
    // 2 * MIN_DOUBLE
    vect = { 2., std::numeric_limits<double>::lowest(), 3. };
    REQUIRE_THROWS_AS(ProcessNumbers(vect), std::out_of_range);
    
}
