#pragma once
#include <string>
#include <map>


class NetworkRequest
{
public:


    static bool sendRequest(const std::string& url, const std::string& data, std::string& response);


    static bool post(const std::string& url, const std::string& data, std::string& response);


    static void setRequestTimeout(long timeout);
private:
    static long s_timeout;
};



