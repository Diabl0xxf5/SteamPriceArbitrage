#pragma once

#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <iostream>
#include <deque>
#include <mutex>

class ProxyList {

	struct ProxyItem {
		std::string type;
		std::string ip;
		std::string port;

		std::string to_string() {
			std::stringstream ss;
			ss << type << ip << ":" << port;
			return ss.str();
		}
	};

public:

	ProxyList() = default;

	void AddProxyHTTP(std::string filename) {
		
		std::ifstream work_file(filename);

		if (work_file.is_open()) {
			std::ifstream f(filename);
			std::stringstream ss;
			ss << f.rdbuf();

			std::string line;
			getline(ss, line);

			while (getline(ss, line)) {
				items_.push_back(ParseLine(line, "http://"));
			}

		}

		work_file.close();

	}

	void AddProxyListSOCK4(std::string filename) {

		std::ifstream work_file(filename);

		if (work_file.is_open()) {
			std::ifstream f(filename);
			std::stringstream ss;
			ss << f.rdbuf();

			std::string line;

			while (getline(ss, line)) {
				items_.push_back(ParseLine(line, "socks4://"));
			}

		}

		work_file.close();

	}

	void AddProxyListSOCK5(std::string filename) {

		std::ifstream work_file(filename);

		if (work_file.is_open()) {
			std::ifstream f(filename);
			std::stringstream ss;
			ss << f.rdbuf();

			std::string line;

			while (getline(ss, line)) {
				items_.push_back(ParseLine(line, "socks5://"));
			}

		}

		work_file.close();

	}

	std::string GetNext() noexcept {
		
		m.lock();

		ProxyItem proxy = items_.front();
		items_.pop_front();
		items_.push_back(proxy);

		m.unlock();

		return proxy.to_string();
	}

private:
	std::deque<ProxyItem> items_;
	std::mutex m;

	ProxyItem ParseLine(std::string line, std::string type) {

		static const char delimiter = ':';
		ProxyItem result;
		result.type = type;
		std::stringstream stream(line);
	
		getline(stream, result.ip, delimiter);
		getline(stream, result.port, delimiter);

		return result;

	}
};
