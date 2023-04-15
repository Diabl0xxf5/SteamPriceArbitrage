#include <windows.h>
#include <winhttp.h>
#include <iostream>
#include <string>
#include <vector>

#include "log_duration.h"

#pragma comment(lib, "winhttp.lib")

class WinHTTPClient {

public:

	WinHTTPClient(std::wstring serverName, std::wstring requestPath) : serverName_(serverName), requestPath_(requestPath) { }

	std::pair<std::string, bool> getResponse(std::wstring queryString, std::wstring proxyAddres = L"") {

		HINTERNET hSession = nullptr;
		HINTERNET hConnect = nullptr;
		HINTERNET hRequest = nullptr;

		auto closeAll = [&]() {

			if (hRequest)
				WinHttpCloseHandle(hRequest);
			if (hConnect)
				WinHttpCloseHandle(hConnect);
			if (hSession)
				WinHttpCloseHandle(hSession);
		};


		hSession = WinHttpOpen(L"WinHTTP Example/1.0", WINHTTP_ACCESS_TYPE_DEFAULT_PROXY, WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);
		
		
		if (!proxyAddres.empty()) {
			WINHTTP_PROXY_INFO proxyInfo = { 0 };
			proxyInfo.dwAccessType = WINHTTP_ACCESS_TYPE_NAMED_PROXY;
			proxyInfo.lpszProxy = (LPWSTR)proxyAddres.c_str();
			proxyInfo.lpszProxyBypass = WINHTTP_NO_PROXY_BYPASS;

			auto result = WinHttpSetOption(hSession, WINHTTP_OPTION_PROXY, &proxyInfo, sizeof(proxyInfo));
			if (!result) {
				closeAll();
				return { "������ ��������� ������.", false };
			}
		}

		
		if (!hSession) {
			return { "������ ������������� WinHTTP: " + GetLastError(), false };
		}
		

		hConnect = WinHttpConnect(hSession, (LPCWSTR)serverName_.c_str(), INTERNET_DEFAULT_HTTPS_PORT, 0);

		if (!hConnect) {
			closeAll();
			return { "������ ����������: " + GetLastError(), false };
		}

		std::wstring requestPath(requestPath_);
		requestPath.append(queryString);
		
		LPCWSTR par = (LPCWSTR)requestPath.c_str();
		hRequest = WinHttpOpenRequest(hConnect, L"GET", par, NULL, WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, WINHTTP_FLAG_SECURE);
		
		DWORD dwConnectTimeout = 1500; // ������� ���������� � �������������
		DWORD dwReceiveTimeout = 1500; // ������� �������� ������ � �������������

		WinHttpSetOption(hRequest, WINHTTP_OPTION_CONNECT_TIMEOUT, &dwConnectTimeout, sizeof(dwConnectTimeout));
		WinHttpSetOption(hRequest, WINHTTP_OPTION_RECEIVE_TIMEOUT, &dwReceiveTimeout, sizeof(dwReceiveTimeout));

		if (!hRequest) {
			closeAll();
			return { "������ �������: " + GetLastError(), false };
		}

		if (!WinHttpSendRequest(hRequest, WINHTTP_NO_ADDITIONAL_HEADERS, 0, WINHTTP_NO_REQUEST_DATA, 0, 0, 0)) {
			closeAll();
			return { "������ �������� �������: " + GetLastError(), false };
		}

		if (!WinHttpReceiveResponse(hRequest, NULL)) {
			closeAll();
			return { "������ ��������� ������: " + GetLastError(), false };
		}

		// ������ ������ �� ������
		DWORD dwSize = 0;
		DWORD dwDownloaded = 0;
		char* pszOutBuffer;
		std::string response;
		
		do {
			
			dwSize = 0;
			
			if (!WinHttpQueryDataAvailable(hRequest, &dwSize)) {
				closeAll();
				return { "������ ��������� ������� ������: " + GetLastError(), false };
			}

			pszOutBuffer = new char[dwSize + 1];
			if (!pszOutBuffer) {
				closeAll();
				return { "������ ��������� ������.", false };
			}

			ZeroMemory(pszOutBuffer, dwSize + 1);

			if (!WinHttpReadData(hRequest, (LPVOID)pszOutBuffer, dwSize, &dwDownloaded)) {
				delete[] pszOutBuffer;
				closeAll();
				return { "������ ������ ������: " + GetLastError(), false };
			}
			
			fillResponse(response, pszOutBuffer, dwDownloaded);

			delete[] pszOutBuffer;

		} while (dwSize > 0);

		return { response, true };
	}

private:
	std::wstring serverName_;
	std::wstring requestPath_;

	void fillResponse(std::string& response, char* pszOutBuffer, DWORD& dwDownloaded) {
		
		response.reserve(response.size() + dwDownloaded);

		for (DWORD i = 0; i < dwDownloaded; ++i) {
			
			bool check = (pszOutBuffer[i] > 32 && pszOutBuffer[i] < 92) || (pszOutBuffer[i] > 92 && pszOutBuffer[i] < 126);

			if (check) {
				response += pszOutBuffer[i];
			}

		}
	}

};