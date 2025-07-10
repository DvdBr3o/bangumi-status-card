#include <drogon/drogon.h>

namespace BangumiStatusCard {
	class Api : public drogon::HttpController<Api> {
	public:
		METHOD_LIST_BEGIN
		ADD_METHOD_TO(Api::api, "/api", drogon::Get);
		METHOD_LIST_END

	public:
		void api(
			const drogon::HttpRequestPtr&						  req,
			std::function<void(const drogon::HttpResponsePtr&)>&& callback
		);

	private:
		struct UserData {};

		auto _get_user_data(std::string_view username) -> drogon::Task<UserData>;
		auto _get_user_data(const std::string& username) -> drogon::Task<UserData>;
	};
}  // namespace BangumiStatusCard

namespace BangumiStatusCard {
	void Api::api(
		const drogon::HttpRequestPtr&						  req,
		std::function<void(const drogon::HttpResponsePtr&)>&& callback
	) {
		const auto params = req->getParameters();
		auto	   resp	  = drogon::HttpResponse::newHttpResponse();

		for (const auto& p : params) std::cout << std::format("{}: {}\n", p.first, p.second);

		const auto username = params.at("username");
		if (username.empty()) {
			resp->setStatusCode(drogon::k400BadRequest);
			callback(resp);
			return;
		}

		const auto userdata = _get_user_data(username);

		callback(resp);
	}

	auto Api::_get_user_data(std::string_view username) -> drogon::Task<UserData> {
		return _get_user_data(std::string { username });
	}

	auto Api::_get_user_data(const std::string& username) -> drogon::Task<UserData> {
		using drogon::HttpClient;
		using drogon::HttpRequest;

		auto bangumi = HttpClient::newHttpClient("");
		auto req	 = HttpRequest::newHttpRequest();
		req->setMethod(drogon::HttpMethod::Get);
		req->setParameter("username", username);
		const auto resp = co_await bangumi->sendRequestCoro(req);

		co_return {};
	}
}  // namespace BangumiStatusCard