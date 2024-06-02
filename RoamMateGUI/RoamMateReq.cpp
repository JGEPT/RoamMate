#include <iostream>
#include <string>
#include <cmath>
#include <unordered_set>
#include <regex>
#define CPPHTTPLIB_OPENSSL_SUPPORT
#include "nlohmann/json.hpp"
#include "httplib.h"

/*
================================================================================================================================================================
FILE 		: RoamMateRoutes.cpp
AUTHORS		: James Gabriel Ty, Christian James Bayadog, Charisse Valmores
DESCRIPTION : This program is allows RoamMate to communicate with the google maps API. This program asks the user for an origin and destination, then from that
it will request a route from the google maps API to get from the origin to the destination. The routes are then parsed then the distance, time, steps, and estimated
fares will be displayed.
COPYRIGHT	: May 20, 2024
REVISION HISTORY:
Date:		   By:		  			 Description:
================================================================================================================================================================
*/

// API key initialization
const std::string ApiKey = "AIzaSyBB68RllPp39IFsqWfInRCef_z0GbPqNVY";

class RouteRequest
{
private:
    // Create a class containing all the details of the jeep route
    class JeepRoute
    {
        // Set class elements to public and declare needed variables
    public:
        int fare, DiscFare;
        int duration;
        int distance;
        std::string steps, FareSum, DiscSum, Route;

        JeepRoute() : fare(0), DiscFare(0), duration(0), distance(0), steps(""), FareSum(""), DiscSum(""), Route("") {} // Contructor to initalize default values
    };

    // Create a class containing all the detials of the taxi route
    class TaxiRoute
    {
        // Set class elements to public and declare needed variables
    public:
        int duration;
        float distance;
        int fare;
        std::string Route;

        TaxiRoute() : duration(0), distance(0.0f), fare(0.0f), Route("") {} // Contructor to initalize default values

        /*
        ================================================================================================================================================================
        FUNCTION	: CalcPrintFare
        DESCRIPTION : This function calculates the estimated taxi fare, and then prints it out.
        ARGUEMENTS  : This function has no arguments but it access other elements of the class.
        RETURNS     : This function returns nothing.
        ================================================================================================================================================================
        */

        void CalcPrintFare() // Member function to calculate and print the fare
        {
            fare = 45 + (duration * 3.10) + (distance * 8.30);                   // Calculate the fare with the forumla
            Route += "Estimated taxi fare: " + std::to_string((int)round(fare)) + " PHP"+"\n";                  // Store the fare to the route string
        }
    };

    // Variable declerations
    std::string origin;
    std::string destination;
    std::string route;

public:
/*
================================================================================================================================================================
FUNCTION	: sendHTTPRequest
DESCRIPTION : This function sends and HTTP request to the google maps API.
ARGUEMENTS  : This function's argument is string url.
RETURNS     : This function returns a json that the google maps API sends.
================================================================================================================================================================
*/

    nlohmann::json sendHTTPRequest(const std::string &url)
    {
        httplib::SSLClient client("maps.googleapis.com"); // Create a HTTPS client
        auto res = client.Get(url.c_str());               // Make a get request to the google maps API
        if (res && res->status == 200)
        {
            return nlohmann::json::parse(res->body); // Return the json file
        }
        else
        {
            std::cerr << "Error: " << res.error() << std::endl; // If any error occur inform the user
            exit(1);                                            // exit the function
        }
    }

    std::string makeRequest(const std::string origin, const std::string destination)
    {
        std::string Route = "";
        // Make API request with transit mode for jeepney routes
        std::string transit_url = URL(origin, destination, "transit", ApiKey, "true");
        auto JeepResp = sendHTTPRequest(transit_url);
        Route += parseJeepResp(JeepResp);

        // Make API request with driving mode for taxi routes
        std::string driving_url = URL(origin, destination, "driving", ApiKey, "false");
        auto TaxiResp = sendHTTPRequest(driving_url);
        Route += parseTaxiResp(TaxiResp);
        return Route;
    }

    /*
================================================================================================================================================================
FUNCTION	: URL
DESCRIPTION : This function creates the url for the API call.
ARGUEMENTS  : This function's arguments are string origin, string destination, string mode, string ApiKey, and string Alternate.
RETURNS     : This function returns a string which is the URL needed for the API call.
================================================================================================================================================================
*/

    std::string URL(const std::string &origin, const std::string &destination, const std::string &mode, const std::string &ApiKey, const std::string Alternate)
    {
        return "/maps/api/directions/json?origin=" + origin + "&destination=" + destination + "&mode=" + mode + "&key=" + ApiKey + "&alternatives=" + Alternate;
    }

    /*
================================================================================================================================================================
FUNCTION	: parseJeepResp
DESCRIPTION : This function parses the json for a transit route request from the google maps API.
ARGUEMENTS  : This function's argument is json &response.
RETURNS     : This function returns nothing
================================================================================================================================================================
*/

    std::string parseJeepResp(const nlohmann::json &response)
    {
        std::unordered_set<std::string> uniqueRoutes; // Declare the unordered set
        JeepRoute details;                            // Declare an object of the class JeepRoute

        auto routes = response["routes"]; // Get the different routes from the response json
        for (const auto &route : routes)  // Loop through all the different routes
        {
            std::string routeIdentifier; // Declare a string called routeIdentifier to identify unique routes
            auto legs = route["legs"];   // Get the legs of the routes
            for (const auto &leg : legs) // Loop through all the legs
            {
                details.duration += leg["duration"]["value"].get<int>(); // Parse the json to get the duration
                details.distance += leg["distance"]["value"].get<int>(); // Parse the json to get the distance

                auto steps = leg["steps"];     // Get all the steps
                for (const auto &step : steps) // Loop through all the steps
                {
                    if (step.contains("transit_details")) // Checks if there is a short name of
                    {                                     // the jeep, then prints it
                        auto transit = step["transit_details"];
                        if (transit["line"].contains("short_name"))
                        {
                            routeIdentifier += transit["line"]["short_name"].get<std::string>();
                        }
                    }
                }
            }
            if (uniqueRoutes.find(routeIdentifier) != uniqueRoutes.end()) // Sees if the routeIdentifier already
            {                                                             // exists in the unique rounds set
                continue;                                                 // If it already exists skip it
            }
            uniqueRoutes.insert(routeIdentifier); // Inserts the route identifier into the set

            details.fare = 0, details.DiscFare = 0;
            details.steps = "", details.FareSum = "", details.DiscSum = "";
            for (const auto &leg : legs) // Loops through the legs
            {
                // Saves the duration and distance into a string
                details.Route += "\nDuration: " + (std::string)leg["duration"]["text"] +
                                 "\nDistance: " + (std::string)leg["distance"]["text"];

                auto steps = leg["steps"];
                int StepCount = 0;
                for (const auto &step : steps) // Loops through the steps
                {
                    if (step["travel_mode"] == "TRANSIT") // Checks if the travel mode is transit
                    {
                        int distance = round(step["distance"]["value"].get<int>() / 1000.0); // Stores the distance in an int
                        if (step.contains("transit_details"))                                // Checks if there are transit details
                        {
                            auto transit = step["transit_details"];
                            // Adds the short name to the steps string, and calculates and displays the fare for the step
                            if (transit["line"].contains("short_name"))
                            {
                                if (StepCount > 0)
                                {
                                    details.steps += (" -> " + transit["line"]["short_name"].get<std::string>());
                                }
                                else
                                {
                                    details.steps += transit["line"]["short_name"].get<std::string>();
                                }
                                if (distance > 5)
                                {
                                    details.fare += round(15 + (distance - 5) * 2.65);
                                    details.DiscFare += round((15 + (distance - 5) * 2.65) * 0.80);
                                    details.FareSum += (std::string)transit["line"]["short_name"] + " - " + std::to_string((int)round(15 + (distance - 5) * 2.65)) + " PHP ";
                                    details.DiscSum += (std::string)transit["line"]["short_name"] + " - " + std::to_string((int)(round((15 + (distance - 5) * 2.65) * .80))) + " PHP ";
                                }
                                else
                                {
                                    details.fare += 15;
                                    details.DiscFare += 12;
                                    details.FareSum += (std::string)transit["line"]["short_name"] + " - 15 PHP ";
                                    details.DiscSum += (std::string)transit["line"]["short_name"] + " - 12 PHP ";
                                }
                            }
                        }
                    }
                    // Adds the directions to the steps string
                    else if (StepCount > 0)
                    {
                        details.steps += (" -> " + step["html_instructions"].get<std::string>());
                    }
                    else
                    {
                        details.steps += step["html_instructions"].get<std::string>();
                    }
                    StepCount++;
                }
            }
            // Save the entire route and the estimated fare into a string.
            details.Route += "\nRoute: " + details.steps + "\nFare summary: " + details.FareSum + " = " + std::to_string(details.fare) + " PHP\n" + "Discounted fare summary: " + details.DiscSum + " = " + std::to_string(details.DiscFare) + " PHP\n\n";
        }
        return details.Route;
    }

    /*
================================================================================================================================================================
FUNCTION	: parseTaxiResp
DESCRIPTION : This function parses the json for a driving route request from the google maps API.
ARGUEMENTS  : This function's argument is json &response.
RETURNS     : This function returns nothing
================================================================================================================================================================
*/

    std::string parseTaxiResp(const nlohmann::json &response)
    {
        TaxiRoute details; // Create an object of the class TaxiRoute

        auto routes = response["routes"]; // Set routes to be the routes we get from the response
        for (const auto &route : routes)  // Loop through the routes
        {
            auto legs = route["legs"];   // Set the legs to be the legs of the route
            for (const auto &leg : legs) // Loop through the legs
            {
                // Print the duration and distance for the taxi route
                details.Route += "\nTaxi Duration: " + (std::string) leg["duration"]["text"] + "\nTaxi Distance: " + (std::string)leg["distance"]["text"] + "\n";
                // Parse the duration using regex to separete the hours and mins
                std::string duration_str = leg["duration"]["text"];
                std::regex hours_regex(R"((\d+)\s*hour[s]?)");  // Define the regex for the hours
                std::regex minutes_regex(R"((\d+)\s*min[s]?)"); // Define the regex for the mins
                std::smatch match;                              // Make a match with regex
                // Set the distance to the converted value when converting the distance string to a float.
                details.distance = std::stof((std::string)leg["distance"]["text"]);
                if (std::regex_search(duration_str, match, hours_regex)) // Check if it matches the hours regex
                {
                    details.duration += std::stoi(match[1]) * 60; // Set the duration distance to the int conversion
                } // of the match and then multiply it by 60
                if (std::regex_search(duration_str, match, minutes_regex)) // Check if it matches the mins regex
                {
                    details.duration += std::stoi(match[1]); // Set the duration distance to the int conversion
                } // of the matched string
            }
        }
        details.CalcPrintFare(); // Call the CalcPrintFare function
        return details.Route;
    }
};
