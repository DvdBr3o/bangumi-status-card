#include "UserData.hpp"
#include "Card.hpp"

#include <drogon/drogon.h>

#include <algorithm>

namespace BangumiStatusCard {

	inline static auto to_string(BangumiSubjectType type) -> std::string {
		return std::to_string(static_cast<std::uint32_t>(type));
	}

	inline static auto to_string(BangumiCollectionType type) -> std::string {
		return std::to_string(static_cast<std::uint32_t>(type));
	}

	class Api : public drogon::HttpController<Api> {
	public:
		METHOD_LIST_BEGIN
		ADD_METHOD_TO(Api::details, "/details", drogon::Get);
		METHOD_LIST_END

	public:
		void details(
			const drogon::HttpRequestPtr&						  req,
			std::function<void(const drogon::HttpResponsePtr&)>&& callback
		);

	private:
		struct GetUserDataParam {
			const std::string&	  username;
			std::uint32_t		  top;
			BangumiCollectionType collection_type;
			bool				  title;
			bool				  chinese;
		};

		auto _get_user_data(const GetUserDataParam& params) -> drogon::Task<UserData>;
	};
}  // namespace BangumiStatusCard

namespace BangumiStatusCard {
	void Api::details(
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

		// To be elegant requires drogon return optional/expected monad instead of exception!!!
		const auto get_params = GetUserDataParam {
			.username = params.at("username"),
			.top	  = !params.contains("top") ? 0 : std::stoul(params.at("top")),
			.collection_type =
				!params.contains("collection_type") ?
					BangumiCollectionType::Watched :
					static_cast<BangumiCollectionType>(std::stoul(params.at("collection_type"))),
			.title	 = !params.contains("title")	 ? true :
					   params.at("title") == "false" ? false :
													   true,
			.chinese = !params.contains("chinese")	  ? false :
					   params.at("chinese") == "true" ? true :
														false,
		};

		const auto userdata = drogon::sync_wait(_get_user_data(get_params));

		// std::ranges::for_each(userdata.animes, [](auto&& a) { std::cout << a.name << '\n'; });

		resp->setBody(
			card(
				{ .animes = { Anime {
					  .name	  = "败犬",
					  .avatar = "https://lain.bgm.tv/r/400/pic/cover/l/e4/dc/464376_NsZRw.jpg",
					  .tags	  = { "a", "b", "c" },
				  } } },
				800,
				600
			)
				.to_string_loose()
		);

		callback(resp);
	}

	auto Api::_get_user_data(const GetUserDataParam& params) -> drogon::Task<UserData> {
		using drogon::HttpClient;
		using drogon::HttpRequest;

		// https://api.bgm.tv/v0/users/dvdbr3o/collections?subject_type=2&type=3&limit=30&offset=0
		auto bangumi = HttpClient::newHttpClient("https://api.bgm.tv");
		auto req	 = HttpRequest::newHttpRequest();
		req->setMethod(drogon::HttpMethod::Get);
		req->setParameter("username", params.username);
		req->setParameter("subject_type", to_string(BangumiSubjectType::Anime));
		req->setParameter("type", to_string(params.collection_type));
		req->setParameter("limit", "30");
		req->setParameter("offset", "0");
		req->addHeader("Accept", "application/json");
		req->setPath(std::format("/v0/users/{}/collections", params.username));
		const auto resp = co_await bangumi->sendRequestCoro(req);
		const auto data = (*resp->getJsonObject())["data"];

		UserData   userdata;
		userdata.animes.reserve(data.size());

		SetConsoleOutputCP(CP_UTF8);

		for (const auto& anime : data)
			userdata.animes.emplace_back(Anime {
				.name = params.chinese ? anime["subject"]["name_cn"].asString() :
										 anime["subject"]["name"].asString(),
			});

		co_return userdata;
	}
}  // namespace BangumiStatusCard