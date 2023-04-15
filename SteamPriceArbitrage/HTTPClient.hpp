#pragma once

#include <string>
#include <boost/beast.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>

namespace basio = boost::asio;
namespace http = boost::beast::http;

class HTTPClient {

public:

	HTTPClient() = delete;
	HTTPClient(const HTTPClient& other) = delete;

	explicit HTTPClient(std::string main_api, std::string api_argument)
		: main_api_(main_api), api_argument_(api_argument) {
	}
	
	~HTTPClient() {

	}

	std::string getResponse(const std::string& headers) {

		basio::io_context io;
		basio::ip::tcp::resolver resolver(io);
		basio::ip::tcp::socket socket(io);

		basio::connect(socket, resolver.resolve(main_api_, "http"));
		std::string argument = api_argument_ + headers;
		http::request<http::string_body> request;
		request.method(http::verb::get);
		request.version(11);
		request.set(http::field::host, main_api_);
		request.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
		request.target(api_argument_);
		request.set(boost::beast::http::field::content_type, "application/x-www-form-urlencoded");
		request.set(boost::beast::http::field::accept, "application/json");
		request.set(boost::beast::http::field::cache_control, "no-cache");


		http::write(socket, request);

		std::string response;
		{
			boost::beast::flat_buffer buffer;
			http::response<http::dynamic_body> res;
			http::read(socket, buffer, res);
			response = boost::beast::buffers_to_string(res.body().data());
		}
		socket.shutdown(basio::ip::tcp::socket::shutdown_both);
		return response;
	}

private:
	std::string main_api_;
	std::string api_argument_;
};

/* example

	{
		LOG_DURATION("API TEST");
		HTTPClient client("ip-api.com", "/json/");
		std::string json = client.getResponse("google.com");

		if (!json.empty()) {
			JSONParser parser;
			std::string field = "lat";
			auto field_value = parser.getFieldFromJSON<double>(json, field);
			std::cout << field_value << std::endl;
		}
	}

*/
