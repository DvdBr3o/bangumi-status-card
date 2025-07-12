#include "UserData.hpp"
#include "Card.hpp"

#include <drogon/drogon.h>

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
		static void details(
			const drogon::HttpRequestPtr&						  req,
			std::function<void(const drogon::HttpResponsePtr&)>&& callback
		);

	private:
		struct GetUserDataParam {
			const std::string&	  username;
			std::uint32_t		  top;
			BangumiCollectionType collection_type;
			std::string			  title;
			bool				  chinese;
			std::string_view	  theme_color;
			int					  width;
			int					  view_box_width;
		};

		static auto _get_user_data(GetUserDataParam&& params) -> drogon::Task<UserData>;
	};
}  // namespace BangumiStatusCard

namespace BangumiStatusCard {
	void Api::details(
		const drogon::HttpRequestPtr&						  req,
		std::function<void(const drogon::HttpResponsePtr&)>&& callback
	) {
		const auto params = req->getParameters();
		auto	   resp	  = drogon::HttpResponse::newHttpResponse();

		// for (const auto& p : params) std::cout << std::format("{}: {}\n", p.first, p.second);

		const auto username = params.at("username");
		if (username.empty()) {
			resp->setStatusCode(drogon::k400BadRequest);
			callback(resp);
			return;
		}

		// To be elegant requires drogon return optional/expected monad instead of exception!!!
		auto get_params = GetUserDataParam {
			.username = params.at("username"),
			.top	  = !params.contains("top") ? 0 : std::stoul(params.at("top")),
			.collection_type =
				!params.contains("collection_type") ?
					BangumiCollectionType::Watched :
					static_cast<BangumiCollectionType>(std::stoul(params.at("collection_type"))),
			.title	 = !params.contains("title") ? "" : params.at("title"),
			.chinese = !params.contains("chinese")	  ? false :
					   params.at("chinese") == "true" ? true :
														false,
			.width	 = !params.contains("width") ? 800 : std::stol(params.at("width")),
			.view_box_width =
				!params.contains("view_box_width") ? 800 : std::stol(params.at("view_box_width")),
		};

		const auto userdata = drogon::sync_wait(_get_user_data(std::move(get_params)));

		const auto cardd =
			card(userdata, get_params.width, get_params.view_box_width).to_string_compact();

		// std::cout << cardd << '\n';
		resp->setBody(cardd);
		resp->setContentTypeCode(drogon::ContentType::CT_IMAGE_SVG_XML);

		callback(resp);
	}

	auto Api::_get_user_data(GetUserDataParam&& params) -> drogon::Task<UserData> {
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
		userdata.collection_type = params.collection_type;
		userdata.title			 = std::move(params.title);

		userdata.animes.reserve(data.size());
		for (const auto& anime : data)
			userdata.animes.emplace_back(Anime {
				.name		= params.chinese ? anime["subject"]["name_cn"].asString() :
											   anime["subject"]["name"].asString(),	 //
				.avatar		= anime["subject"]["images"]["common"].asString(),		 //
				.updated_at = anime["updated_at"].asString(),						 //
				.ep_status	= anime["ep_status"].as<std::uint32_t>(),				 //
				.eps		= anime["subject"]["eps"].as<std::uint32_t>(),			 //
			});

		co_return userdata;
	}
}  // namespace BangumiStatusCard