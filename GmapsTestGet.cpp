#include "httplib.h"
#include <iostream>

void httpGetRequest(const std::string &host, const std::string &path)
{
    httplib::Client cli(host.c_str());
    auto res = cli.Get(path.c_str());

    if (res && res->status == 200)
    {
        std::cout << res->body << std::endl;
    }
    else
    {
        if (res)
        {
            std::cerr << "Request failed: HTTP status " << res->status << std::endl;
        }
        else
        {
            std::cerr << "Request failed: " << res.error() << std::endl;
        }
    }
}

int main()
{
    std::string host = "http://postman-echo.com";
    std::string path = "/get";
    httpGetRequest(host, path);
    return 0;
}