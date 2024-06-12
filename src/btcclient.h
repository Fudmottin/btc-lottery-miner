#ifndef BTCCLIENT_H
#define BTCCLIENT_H

#include <nlohmann/json.hpp>

class BTCClient {
public:
    BTCClient() = delete;
    BTCClient(const std::string& url, const std::string& port, const std::string& auth);
    nlohmann::json make_request(const std::string& method, const nlohmann::json& params = nullptr);
    nlohmann::json make_request(const std::vector<std::pair<std::string, nlohmann::json>>& requests);

private:
    std::string url_;
    std::string port_;
    std::string auth_;
};

#endif //BTCLIENT_H
