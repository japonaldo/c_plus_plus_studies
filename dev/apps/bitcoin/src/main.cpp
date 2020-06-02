// TODO add Copyright

#include <array>
#include <cstdio>
#include <functional>
#include <iostream>
#include <memory>

#include <curl/curl.h>
#include <nlohmann/json.hpp>

using CURL_ptr = std::unique_ptr<CURL, std::function<void(CURL*)>>;

class CurlHandle
{
 private:
  CURL_ptr curlptr_;
  constexpr static auto deleter = [](CURL *curl)
  {
    curl_easy_cleanup(curl);
    curl_global_cleanup();
  };

 public:
  CurlHandle() : curlptr_(curl_easy_init(), deleter)
  {
    curl_global_init(CURL_GLOBAL_ALL);
  }

  void SetUrl(const std::string& url)
  {
    curl_easy_setopt(curlptr_.get(), CURLOPT_URL, url.c_str());
  }

  CURLcode Fetch()
  {
    return curl_easy_perform(curlptr_.get());
  }
};

class Bitcoin
{
 private:
  CurlHandle curl_handle_;
  static constexpr const char* API_URL = "https://blockchain.info/ticker";

 public:
  Bitcoin() : curl_handle_({})
 {
    curl_handle_.SetUrl(API_URL);
 }

 void FetchBitcoinData()
 {
   curl_handle_.Fetch();
 }
};

int main()
{
  Bitcoin bitcoin;

  bitcoin.FetchBitcoinData();

  return 0;
}
