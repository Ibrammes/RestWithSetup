#include "MyListener.h"

MyListener::MyListener(const http::uri& url) : m_listener(http_listener(url))
{
	m_listener.support(methods::GET, std::tr1::bind(&MyListener::handle_get, this, std::tr1::placeholders::_1)); 
	//m_listener.support(methods::PUT, std::tr1::bind(&MyListener::handle_put, this, std::tr1::placeholders::_1));
	m_listener.support(methods::POST, std::tr1::bind(&MyListener::handle_post, this, std::tr1::placeholders::_1));
	//	m_listener.support(methods::DEL, std::tr1::bind(&MyListener::handle_delete, this, std::tr1::placeholders::_1));
}

void MyListener::handle_get(http_request message)
{
	web::uri req_uri = message.request_uri();
	utility::string_t req_query = req_uri.query();
	auto req_split_query = web::uri::split_query(req_query);
	//utility::string_t params;
	std::string error;
	for (auto it = req_split_query.begin(); it != req_split_query.end(); ++it)
	{
		//params += (*it).first + utility::conversions::to_string_t(": ") + (*it).second + utility::conversions::to_string_t("\n");
		// ***** Проверка логина *****
		if (utility::conversions::to_utf8string((*it).first) == "login")
		{
			std::string login = utility::conversions::to_utf8string((*it).second);
			for (auto iter = login.begin(); iter != login.end(); ++iter)
			{
				if (!(*iter >= 'a' && *iter <= 'z'))
					error += "Error in login. Unacceptable symbols\n";
			}
		}
		// ***** Проверка пароля *****
		if (utility::conversions::to_utf8string((*it).first) == "password")
		{
		
			std::string pass = utility::conversions::to_utf8string((*it).second);
			if (pass.length() < 8)
				error += "Error in password. Insufficient password length\n";
			for (auto iter = pass.begin(); iter != pass.end(); ++iter)
			{
				//if (!isdigit(*iter) && !isalpha(*iter) && !ispunct(*iter))
				if (!(*iter >= 33 && *iter <= 126))
				{			
					error += "Error in password. Unacceptable symbols\n";
				}	
			}
		}
	}
	if (error == "")
		message.reply(status_codes::OK, "OK");// "OK\n" + utility::conversions::to_utf8string(params));
	else
		message.reply(status_codes::BadRequest, error);// + '\n' + utility::conversions::to_utf8string(params));
	
		
	////message.reply(status_codes::OK, params);
}

void MyListener::handle_post(http_request message)
{
	string resp;
	json::value jvalue = message.extract_json().get();
	if (jvalue.is_null())
		message.reply(status_codes::BadRequest, "No json");
	else
	{
		//utility::stringstream_t stream;
		std::stringstream stream;
		jvalue.serialize(stream);
		boost::property_tree::ptree pt;
		try
		{
			boost::property_tree::read_json(stream, pt);
		}
		catch (boost::property_tree::json_parser_error& error)
		{
			cout
				<< error.message() << ": "
				<< error.filename() << ", line "
				<< error.line() << endl;
		}
		try
		{
			string login = pt.get<string>("login");
			//// ***** Проверка логина ****
			for (auto iter = login.begin(); iter != login.end(); ++iter)
			{
				if (!(*iter >= 'a' && *iter <= 'z'))
					resp += "Error in login. Unacceptable symbols\n";
			}

			string password = pt.get<string>("password");
			// ***** Проверка пароля *****
				if (password.length() < 8)
					resp += "Error in password. Insufficient password length\n";
				for (auto iter = password.begin(); iter != password.end(); ++iter)
				{
					//if (!isdigit(*iter) && !isalpha(*iter) && !ispunct(*iter))
					if (!(*iter >= 33 && *iter <= 126))
					{
						resp += "Error in password. Unacceptable symbols\n";
					}
				}

			//resp += login + " " + password;
		}
		catch (const boost::property_tree::ptree_bad_data& error)
		{
			//cout << error.what() << endl;
			resp += error.what();
		}
		catch (const boost::property_tree::ptree_bad_path& error)
		{
			//cout << error.what() << endl;
			resp += error.what();
		}
		
		////message.reply(status_codes::OK, "Yes json");
		//// ***** Проверка логина *****
		//if (!jvalue[L"login"].is_null())
		//{
		//	string login = utility::conversions::to_utf8string(jvalue[L"login"].as_string());
		//	for (auto iter = login.begin(); iter != login.end(); ++iter)
		//	{
		//		if (!(*iter >= 'a' && *iter <= 'z'))
		//			resp += "Error in login. Unacceptable symbols\n";
		//	}
		//}
		//// ***** Проверка пароля *****
		//if (!jvalue[L"password"].is_null())
		//{
		//	string password = utility::conversions::to_utf8string(jvalue[L"password"].as_string());
		//	if (password.length() < 8)
		//		resp += "Error in password. Insufficient password length\n";
		//	for (auto iter = password.begin(); iter != password.end(); ++iter)
		//	{
		//		//if (!isdigit(*iter) && !isalpha(*iter) && !ispunct(*iter))
		//		if (!(*iter >= 33 && *iter <= 126))
		//		{
		//			resp += "Error in password. Unacceptable symbols\n";
		//		}
		//	}
		//}
		if (resp.empty())
			resp = "OK";
		json::value response = json::value::string(utility::conversions::to_string_t(resp));
		message.reply(status_codes::OK, response);
		//message.reply(status_codes::OK, message.to_string());
	}

}

void MyListener::start()
{
	m_listener.open().then([this]()
	{
		//cout << "Server start" << endl;
		//	m_listener.close();
	}).wait();
}