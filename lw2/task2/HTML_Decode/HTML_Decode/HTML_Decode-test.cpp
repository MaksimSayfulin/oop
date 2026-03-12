#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "HTML_Decode.h"
#include <stdexcept>
#include <chrono>


TEST_CASE("HTML decoding")
{
	SECTION("Basic tests")
    {
		std::string str = "";
		REQUIRE(HtmlDecode(str) == str);
		str = "Cat &lt;says&gt; &quot;Meow&quot;. M&amp;M&apos;s";
		REQUIRE(HtmlDecode(str) == "Cat <says> \"Meow\". M&M's");
	}

    SECTION("Edge cases and invalid input") 
    {
        REQUIRE(HtmlDecode("Fish & Chips") == "Fish & Chips");

        REQUIRE(HtmlDecode("&") == "&");

        REQUIRE(HtmlDecode("&&") == "&&");

        REQUIRE(HtmlDecode("Unknown &foo;") == "Unknown &foo;"); //

        REQUIRE(HtmlDecode("Unknown &amp") == "Unknown &amp"); // 

        REQUIRE(HtmlDecode("Hello World") == "Hello World");
    }

    SECTION("Complex sequences") 
    {
        REQUIRE(HtmlDecode("&lt;&gt;&quot;") == "<>\"");

        REQUIRE(HtmlDecode("&amp;amp;") == "&amp;");
    }

}

