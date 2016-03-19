#include <cpprest\http_client.h>
#include <codecvt>  //для конвертации (изменение кодировки)
#include <cpprest\json.h>

using namespace web;
using namespace web::http;
using namespace web::http::client;
using namespace std;
//using namespace concurrency::streams;  //для извлечения ответа

int main()
{
	http_client client(L"http://localhost:8080");

	json::value obj = json::value::object();
	obj[utility::conversions::to_string_t("login")] = json::value(utility::conversions::to_string_t("user"));
	obj[utility::conversions::to_string_t("password")] = json::value(utility::conversions::to_string_t("pass"));

	pplx::task<http::http_response> resp = client.request(methods::POST, L"/restdemo", obj);
	cout << "request" << endl;
	resp.then([=](http_response response)
	{
		cout << "OK" << endl;
		cout << utility::conversions::to_utf8string(response.extract_string().get()) << endl;
		//concurrency::streams::istream bodyStream = response.body();  //получение тела ответа в виде потока
		//concurrency::streams::container_buffer<string> inStringBuffer;  //буфер для извлечения потока тела ответа
		//bodyStream.read_to_end(inStringBuffer);
		//string &text = inStringBuffer.collection();
		//wstring_convert<codecvt_utf8_utf16<wchar_t>, wchar_t> utf16conv;
		//wostringstream ss;
		//ss << utf16conv.from_bytes(text.c_str()) << std::endl;
		////ss << response.headers().content_length() << endl;  //длина ответа
		//wcout << ss.str();
	});
	//resp.wait();

	getchar();
	return 0;
}