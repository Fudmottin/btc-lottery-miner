#include "btcclient.h"
#include <boost/beast.hpp>
#include <boost/asio.hpp>
#include <iostream>
#include <string>

namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
using tcp = net::ip::tcp;
using nlohmann::json;

BTCClient::BTCClient(const std::string& url, const std::string& port, const std::string& auth)
    : url_(url), port_(port), auth_(auth) {}

json BTCClient::make_request(const std::string& method, const json& params) {
    net::io_context io_context;

    tcp::resolver resolver(io_context);
    auto endpoints = resolver.resolve(url_, port_);

    beast::tcp_stream stream(io_context);
    stream.connect(endpoints);

    http::request<http::string_body> req{http::verb::post, "/", 11};
    req.set(http::field::host, url_ + ":" + port_);
    req.set(http::field::content_type, "application/json");
    req.set(http::field::accept, "application/json");
    req.set(http::field::authorization, "Basic " + auth_);

    json request_data = {
        {"jsonrpc", "1.0"},
        {"id", method},
        {"method", method},
        {"params", params}
    };

    req.body() = request_data.dump();
    req.prepare_payload();

    http::write(stream, req);

    beast::flat_buffer buffer;
    http::response<http::dynamic_body> res;
    http::read(stream, buffer, res);

    if (res.result() == http::status::ok) {
        return json::parse(beast::buffers_to_string(res.body().data()));
    } else {
        std::cerr << "Request failed with status: " << res.result_int() << std::endl;
        return nullptr;
    }
}

nlohmann::json BTCClient::make_request(const std::vector<std::pair<std::string, nlohmann::json>>& requests) {
    net::io_context io_context;

    tcp::resolver resolver(io_context);
    auto endpoints = resolver.resolve(url_, port_);

    beast::tcp_stream stream(io_context);
    stream.connect(endpoints);

    http::request<http::string_body> req{http::verb::post, "/", 11};
    req.set(http::field::host, url_ + ":" + port_);
    req.set(http::field::content_type, "application/json");
    req.set(http::field::accept, "application/json");
    req.set(http::field::authorization, "Basic " + auth_);

    nlohmann::json batch_request = nlohmann::json::array();
    int id = 1;
    for (const auto& request : requests) {
        nlohmann::json req_obj = {
            {"jsonrpc", "2.0"},
            {"id", id++},
            {"method", request.first},
            {"params", request.second}
        };
        batch_request.push_back(req_obj);
    }

    req.body() = batch_request.dump();
    req.prepare_payload();

    http::write(stream, req);

    beast::flat_buffer buffer;
    http::response<http::dynamic_body> res;
    http::read(stream, buffer, res);

    if (res.result() == http::status::ok) {
        return nlohmann::json::parse(beast::buffers_to_string(res.body().data()));
    } else {
        std::cerr << "Request failed with status: " << res.result_int() << std::endl;
        return nullptr;
    }
}
