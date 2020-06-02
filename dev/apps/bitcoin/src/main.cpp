// TODO add Copyright

#include <array>
#include <cstdio>
#include <functional>
#include <iostream>
#include <memory>

#include <curl/curl.h>
#include <nlohmann/json.hpp>

using std::size_t;
using CURL_ptr = std::unique_ptr<CURL, std::function<void(CURL*)>>;
extern "C" size_t DataHandler(const char* buffer, size_t size, size_t nmemb, std::string* user_data)
{
  if (user_data == nullptr)
  {
    return 0;
  }

  user_data->append(buffer, size * nmemb);
  return size * nmemb;
}

class CurlHandle
{
 private:
  CURL_ptr curlptr_;
  constexpr static auto deleter = [](CURL *curl)
  {
    curl_easy_cleanup(curl);
    curl_global_cleanup();
  };
  std::string data_;

 public:
  CurlHandle() : curlptr_(curl_easy_init(), deleter)
  {
    curl_global_init(CURL_GLOBAL_ALL);
    curl_easy_setopt(curlptr_.get(), CURLOPT_WRITEFUNCTION, DataHandler);
    curl_easy_setopt(curlptr_.get(), CURLOPT_WRITEDATA, &data_);
  }

  void SetUrl(const std::string& url)
  {
    curl_easy_setopt(curlptr_.get(), CURLOPT_URL, url.c_str());
  }

  CURLcode Fetch()
  {
    return curl_easy_perform(curlptr_.get());
  }

  std::string GetFetchedData()
  {
    return data_;
  }
};

class Bitcoin
{
  using json = nlohmann::json;

 private:
  CurlHandle curl_handle_;
  static constexpr const char* API_URL = "https://blockchain.info/ticker";

 public:
  Bitcoin() : curl_handle_({})
 {
    curl_handle_.SetUrl(API_URL);
 }

 json FetchBitcoinData()
 {
   curl_handle_.Fetch();
   return json::parse(curl_handle_.GetFetchedData());
 }
};

int main()
{
  using namespace std;
  using nlohmann::json;

  try
  {
    Bitcoin bitcoin;

    json bitcoin_data = bitcoin.FetchBitcoinData();

    cout << "1 BTC = \n";
    for (auto it = bitcoin_data.begin(); it != bitcoin_data.end(); ++it)
    {
      printf("\t(%3s)%10d %s\n",
             it.key().c_str(),
             it.value()["last"].get<int>(),
             it.value()["symbol"].get<string>().c_str());
    }
  }
  catch(...)
  {
    cerr << "Failed to fetch bitcoin exchange rates" << endl;
  }

  return 0;
}
