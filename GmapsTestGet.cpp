#include <iostream>
#define CPPHTTPLIB_OPENSSL_SUPPORT
#include "httplib.h"
#include <nlohmann/json.hpp>

int main()
{
    // Your Google Maps API key
    const std::string api_key = "AIzaSyBB68RllPp39IFsqWfInRCef_z0GbPqNVY";

    // Origin and destination
    const std::string origin = "New+York,NY";
    const std::string destination = "Los+Angeles,CA";

    // Construct the URL
    std::string url = "/maps/api/directions/json?origin=" + origin + "&destination=" + destination + "&key=" + api_key;

    // Create an HTTP client
    httplib::Client client("https://maps.googleapis.com");

    // Send a GET request
    auto res = client.Get(url.c_str());

    // Check if the request was successful
    if (res && res->status == 200)
    {
        // Print the response body
        std::cout << res->body << std::endl;
    }
    else
    {
        std::cerr << "Error: " << res.error() << std::endl;
    }

    return 0;
}