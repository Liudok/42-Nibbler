#define CATCH_CONFIG_MAIN

#include "catch.hpp"

#include "../LogicLib/LogicLib.hpp"

TEST_CASE( "TwoPlusTwoIsFour", "[sampleSum]" )
{
	REQUIRE(sampleSum(2, 2) == 4);
}
