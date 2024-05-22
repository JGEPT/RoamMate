#include <iostream>
#include <string>
#include <cmath>
#include <nlohmann/json.hpp>
#define CPPHTTPLIB_OPENSSL_SUPPORT // Enable OpenSSL support in httplib
#include "httplib.h"

int main()
{
    int distance = 0;
    int fare = 0;
    float TaxiFare = 0;
    std::string distanceStr;

    // Your Google Maps API key
    const std::string api_key = "AIzaSyBB68RllPp39IFsqWfInRCef_z0GbPqNVY"; // Ensure this API key has Directions API enabled

    // Origin and destination
    std::string origin;
    std::string destination;
    const std::string mode = "transit"; // To specify commuting

    std::cout << "Input your origin: ";
    getline(std::cin, origin);
    std::cout << "Input your destination: ";
    getline(std::cin, destination);

    // Construct the URL
    std::string url = "/maps/api/directions/json?origin=" + origin + "&destination=" + destination + "&mode=" + mode + "&key=" + api_key;

    // Create an HTTP client with SSL/TLS support
    httplib::SSLClient client("maps.googleapis.com");

    // Send a GET request
    auto res = client.Get(url.c_str());

    // Check if the request was successful
    if (res && res->status == 200)
    {
        // Parse the JSON response
        auto json_response = nlohmann::json::parse(res->body);

        // Check for API errors
        if (json_response.contains("error_message"))
        {
            std::cerr << "API Error: " << json_response["error_message"] << std::endl;
            return 1;
        }

        // Print the JSON response
        std::cout << json_response.dump(4) << std::endl;

        // Example: Accessing specific data
        auto routes = json_response["routes"];
        for (const auto &route : routes)
        {
            std::cout << "Route Summary: " << route["summary"] << std::endl;

            auto legs = route["legs"];
            for (const auto &leg : legs)
            {
                std::cout << "Start Address: " << leg["start_address"] << std::endl;
                std::cout << "End Address: " << leg["end_address"] << std::endl;
                std::cout << "Duration: " << leg["duration"]["text"] << std::endl;
                std::cout << "Duration: " << std::stof((std::string)leg["duration"]["text"]) << std::endl;
                std::cout << "Distance: " << leg["distance"]["text"] << std::endl;
                std::cout << "Distance: " << std::stof((std::string)leg["distance"]["text"]) << std::endl;

                auto steps = leg["steps"];
                for (const auto &step : steps)
                {
                    std::cout << "Step: " << step["html_instructions"] << std::endl;
                    std::cout << "  Travel Mode: " << step["travel_mode"] << std::endl;
                    std::cout << "  Duration: " << step["duration"]["text"] << std::endl;
                    std::cout << "  Distance: " << step["distance"]["text"] << std::endl;
                    if (step["travel_mode"] == "TRANSIT")
                    {
                        distanceStr = step["distance"]["text"];
                        distance = round(std::stof(distanceStr));
                        if (distance > 5)
                        {
                            fare += round((float)(15 + (distance - 5) * 2.65));
                        }
                        else
                        {
                            fare += 15;
                        }
                    }

                    if (step.contains("transit_details"))
                    {
                        auto transit = step["transit_details"];
                        std::cout << "  Transit Details: " << std::endl;
                        std::cout << "    Line: " << transit["line"]["name"] << std::endl;
                        if (transit["line"].contains("short_name"))
                        {
                            std::cout << "    Short Name: " << transit["line"]["short_name"] << std::endl;
                        }
                        std::cout << "    Vehicle Type: " << transit["line"]["vehicle"]["type"] << std::endl;
                        std::cout << "    Departure Stop: " << transit["departure_stop"]["name"] << std::endl;
                        std::cout << "    Arrival Stop: " << transit["arrival_stop"]["name"] << std::endl;
                        std::cout << "    Departure Time: " << transit["departure_time"]["text"] << std::endl;
                        std::cout << "    Arrival Time: " << transit["arrival_time"]["text"] << std::endl;
                    }
                }
            }
        }
    }
    else
    {
        std::cerr << "Error: " << res.error() << std::endl;
    }

    std::cout << "Estimated jeepney fare: " << fare << "PHP" << std::endl;

    return 0;
}