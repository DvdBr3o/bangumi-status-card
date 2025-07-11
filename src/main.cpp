#include "Api.hpp"
#include "UserData.hpp"
#include "Xml.hpp"
#include "Card.hpp"

#include <argparse/argparse.hpp>
#include <string>

int main(int argc, char** argv) {
	argparse::ArgumentParser arg { "bangumi-status-card" };
	arg												//
		.add_argument("--port")						//
		.default_value(12301)						//
		.help("set the port where this app runs.")	//
		;

	try {
		arg.parse_args(argc, argv);
	} catch (const std::exception& e) {
		std::cerr << e.what() << '\n';
		std::cerr << arg;
		std::terminate();
	}

	const auto port = arg.get<int>("port");

	std::cout << std::format("Starting on localhost:{}\n", port);

	drogon::app()						 //
		.addListener("127.0.0.1", port)	 //
		.run()							 //
		;
}