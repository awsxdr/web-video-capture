#pragma once

#include <nlohmann/json.hpp>

#include <memory>
#include <string>
#include <thread>
#include <websocketpp/config/asio_no_tls_client.hpp>
#include <websocketpp/client.hpp>

namespace web_video_capture
{
	class chrome_debug_connector
	{
	private:
		typedef websocketpp::client<websocketpp::config::asio_client> websocket_client;
		typedef websocketpp::config::asio_client::message_type::ptr message_ptr;

		std::unique_ptr<websocket_client> socket_;
		websocket_client::connection_ptr connection_;
		std::thread listen_thread_;
		bool is_open_;
		int next_message_id_;

		void configure_socket(const std::string& web_socket_url);
		void open_socket(const std::string& web_socket_url);

		void send_message(std::string method, nlohmann::json parameters);

		void handle_message(websocketpp::connection_hdl connection, const message_ptr& message);
		void handle_socket_open(websocketpp::connection_hdl connection);

	public:
		chrome_debug_connector(int port);
		void start_capture();
	};
}
