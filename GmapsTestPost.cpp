#include <httplib.h>
#include <iostream>

void httpPostRequest(const std::string& host, const std::string& path, const httplib::Params& params) {
    httplib::Client cli(host.c_str());
    auto res = cli.Post(path.c_str(), params);

    if (res && res->status == 200) {
        std::cout << res->body << std::endl;
    } else {
        if (res) {
            std::cerr << "Request failed: HTTP status " << res->status << std::endl;
        } else {
            std::cerr << "Request failed: " << res.error() << std::endl;
        }
    }
}

int main() {
    std::string host = "http://postman-echo.com";
    std::string path = "/post";
    httplib::Params params {
        {"field1", "value1"},
        {"field2", "value2"}
    };
    httpPostRequest(host, path, params);
    return 0;
}