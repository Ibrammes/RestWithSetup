#pragma once
#include <cpprest\http_listener.h>
#include <boost\property_tree\json_parser.hpp>

using namespace std;
using namespace web;
using namespace web::http;
using namespace web::http::experimental::listener;

class MyListener
{
public:
	MyListener(const http::uri& url);
	void start();

private:
	void handle_get(http_request request);
	//void handle_put(http_request request);
	void handle_post(http_request request);
	//	void handle_delete(http_request request);

	http_listener m_listener;
};