#define CATCH_CONFIG_MAIN
#include "Catch.hpp"
#include "ParseURL.h"
#include <string>
#include <stdexcept>

std::string url;
std::string host;
std::string doc;
int port = 0;
Protocol prot;

TEST_CASE("Valid URLs with default ports")
{
    SECTION("HTTP - default port 80")
    {
        url = "http://example.com";
        REQUIRE_NOTHROW(ParseURL(url, prot, port, host, doc));
        REQUIRE(prot == Protocol::HTTP);
        REQUIRE(host == "example.com");
        REQUIRE(port == 80);
        REQUIRE(doc == "");
    }

    SECTION("HTTPS - default port 443")
    {
        url = "https://example.com";
        REQUIRE_NOTHROW(ParseURL(url, prot, port, host, doc));
        REQUIRE(prot == Protocol::HTTPS);
        REQUIRE(host == "example.com");
        REQUIRE(port == 443);
        REQUIRE(doc == "");
    }

    SECTION("FTP - default port 21")
    {
        url = "ftp://example.com";
        REQUIRE_NOTHROW(ParseURL(url, prot, port, host, doc));
        REQUIRE(prot == Protocol::FTP);
        REQUIRE(host == "example.com");
        REQUIRE(port == 21);
        REQUIRE(doc == "");
    }
}

TEST_CASE("Valid URLs with various components")
{
    SECTION("Case insensitive protocol")
    {
        url = "htTp://example.com";
        REQUIRE_NOTHROW(ParseURL(url, prot, port, host, doc));
        REQUIRE(prot == Protocol::HTTP);
        REQUIRE(host == "example.com");
    }

    SECTION("With subdomain")
    {
        url = "http://subdomain.example.com";
        REQUIRE_NOTHROW(ParseURL(url, prot, port, host, doc));
        REQUIRE(host == "subdomain.example.com");
    }

    SECTION("With custom port")
    {
        url = "http://example.com:8080";
        REQUIRE_NOTHROW(ParseURL(url, prot, port, host, doc));
        REQUIRE(port == 8080);
        REQUIRE(doc == "");
    }

    SECTION("With document path")
    {
        url = "http://example.com/index.html";
        REQUIRE_NOTHROW(ParseURL(url, prot, port, host, doc));
        REQUIRE(doc == "index.html");
    }

    SECTION("With port and document path")
    {
        url = "http://example.com:8080/path/to/page";
        REQUIRE_NOTHROW(ParseURL(url, prot, port, host, doc));
        REQUIRE(port == 8080);
        REQUIRE(doc == "path/to/page");
    }

    SECTION("With complex document (query and fragment)")
    {
        url = "http://example.com/docs/page?param=value#title";
        REQUIRE_NOTHROW(ParseURL(url, prot, port, host, doc));
        REQUIRE(doc == "docs/page?param=value#title");
    }

    SECTION("With trailing slash")
    {
        url = "http://example.com/";
        REQUIRE_NOTHROW(ParseURL(url, prot, port, host, doc));
        REQUIRE(doc == "");
    }

    SECTION("With double slash in path")
    {
        url = "http://example.com//";
        REQUIRE_NOTHROW(ParseURL(url, prot, port, host, doc));
        REQUIRE(doc == "/");
    }
}

TEST_CASE("Edge cases for host and port")
{
    SECTION("Minimal host")
    {
        url = "http://a.b";
        REQUIRE_NOTHROW(ParseURL(url, prot, port, host, doc));
        REQUIRE(host == "a.b");
    }

    SECTION("IPv4 address")
    {
        url = "http://127.0.0.1";
        REQUIRE_NOTHROW(ParseURL(url, prot, port, host, doc));
        REQUIRE(host == "127.0.0.1");
    }

    SECTION("localhost")
    {
        url = "http://localhost";
        REQUIRE_NOTHROW(ParseURL(url, prot, port, host, doc));
        REQUIRE(host == "localhost");
    }

    SECTION("Port minimum value (1)")
    {
        url = "http://example.com:1";
        REQUIRE_NOTHROW(ParseURL(url, prot, port, host, doc));
        REQUIRE(port == 1);
    }

    SECTION("Port maximum value (65535)")
    {
        url = "http://example.com:65535";
        REQUIRE_NOTHROW(ParseURL(url, prot, port, host, doc));
        REQUIRE(port == 65535);
    }
}

TEST_CASE("Invalid URLs - protocol errors")
{
    SECTION("Missing protocol")
    {
        url = "example.com";
        REQUIRE_THROWS_AS(ParseURL(url, prot, port, host, doc), std::invalid_argument);
    }

    SECTION("Wrong protocol")
    {
        url = "gopher://example.com";
        REQUIRE_THROWS_AS(ParseURL(url, prot, port, host, doc), std::invalid_argument);
    }

    SECTION("Missing ://")
    {
        url = "http:/example.com";
        REQUIRE_THROWS_AS(ParseURL(url, prot, port, host, doc), std::invalid_argument);
    }

    SECTION("Missing colon")
    {
        url = "http//example.com";
        REQUIRE_THROWS_AS(ParseURL(url, prot, port, host, doc), std::invalid_argument);
    }
}

TEST_CASE("Invalid URLs - host errors")
{
    SECTION("Empty host")
    {
        url = "http://";
        REQUIRE_THROWS_AS(ParseURL(url, prot, port, host, doc), std::invalid_argument);
    }

    SECTION("Host with space")
    {
        url = "http://exa mple.com";
        REQUIRE_THROWS_AS(ParseURL(url, prot, port, host, doc), std::invalid_argument);
    }

    SECTION("Host starting with dot")
    {
        url = "http://.example.com";
        REQUIRE_THROWS_AS(ParseURL(url, prot, port, host, doc), std::invalid_argument);
    }

    SECTION("Host ending with dot")
    {
        url = "http://example.com.";
        REQUIRE_THROWS_AS(ParseURL(url, prot, port, host, doc), std::invalid_argument);
    }

    SECTION("Double dot in host")
    {
        url = "http://example..com";
        REQUIRE_THROWS_AS(ParseURL(url, prot, port, host, doc), std::invalid_argument);
    }
}

TEST_CASE("Invalid URLs - port errors")
{
    SECTION("Port 0")
    {
        url = "http://example.com:0";
        REQUIRE_THROWS_AS(ParseURL(url, prot, port, host, doc), std::invalid_argument);
    }

    SECTION("Port > 65535")
    {
        url = "http://example.com:70000";
        REQUIRE_THROWS_AS(ParseURL(url, prot, port, host, doc), std::invalid_argument);
    }

    SECTION("Port with letters")
    {
        url = "http://example.com:abc";
        REQUIRE_THROWS_AS(ParseURL(url, prot, port, host, doc), std::invalid_argument);
    }

    SECTION("Port with space")
    {
        url = "http://example.com: 8080";
        REQUIRE_THROWS_AS(ParseURL(url, prot, port, host, doc), std::invalid_argument);
    }

    SECTION("Empty port")
    {
        url = "http://example.com:";
        REQUIRE_THROWS_AS(ParseURL(url, prot, port, host, doc), std::invalid_argument);
    }
}

TEST_CASE("Invalid URLs - empty or malformed")
{
    SECTION("Empty string")
    {
        url = "";
        REQUIRE_THROWS_AS(ParseURL(url, prot, port, host, doc), std::invalid_argument);
    }

    SECTION("Just protocol")
    {
        url = "http://";
        REQUIRE_THROWS_AS(ParseURL(url, prot, port, host, doc), std::invalid_argument);
    }

    SECTION("Missing host with port")
    {
        url = "http://:8080";
        REQUIRE_THROWS_AS(ParseURL(url, prot, port, host, doc), std::invalid_argument);
    }
}