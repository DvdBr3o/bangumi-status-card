#pragma once

#include "Primitive.hpp"
#include "UserData.hpp"
#include "Utils.hpp"
#include "Xml.hpp"

#include <matchit.h>
#include <drogon/drogon.h>

#include <ranges>
#include <string>

namespace BangumiStatusCard {
	inline static constexpr auto style_text = R"(
        @keyframes width-grow {
            from {
                width: 0%;
            }

            to {
                width: 100%;
            }
        }

        @keyframes left-in {
            from {
                transform: translate(-20px, 0);
                opacity: 0;
            }

            to {
                transform: none;
                opacity: 1;
            }
        }

        @keyframes fade-in {
            from {
                opacity: 0;
            }

            to {
                opacity: 1;
            }
        }

		.bsc-title {
			font: 45px 'Segoe UI', Ubuntu, Sans-Serif;
			font-weight: bold;
			fill: fill: #040e1c;
			animation: left-in 1s ease;
		}

        .bsc-name {
            font: 600 30px 'Segoe UI', Ubuntu, Sans-Serif;
            fill: #040e1c;
            animation: left-in 1s ease;
        }

        .bsc-details {
            font: 18px 'Segoe UI', Ubuntu, Sans-Serif;
            fill: #4a4c4d;
            animation: fade-in 1s ease;
        }

        .bsc-progress-bar {
            animation: width-grow 1s ease forwards;
        }
)";

	inline static auto			 card_style() {
		  return Xml::style().sub(style_text);
	}

	/* Components */

	inline static auto done_icon(const Point& base = { 0, 0 }) {
		using namespace Xml;
		return		   //
			move_it |  //
			svg()
				.attribute("xmlns", "http://www.w3.org/2000/svg")
				.x(base.x + 7)
				.y(base.y + 62)
				.width(25)
				.height(25)
				.viewBox("0 0 2048 2048")
				.sub(
					R"(<path fill="currentColor"d="M1024 0q141 0 272 36t244 104t207 160t161 207t103 245t37 272q0 141-36 272t-104 244t-160 207t-207 161t-245 103t-272 37q-141 0-272-36t-244-104t-207-160t-161-207t-103-245t-37-272q0-141 36-272t104-244t160-207t207-161T752 37t272-37m603 685l-136-136l-659 659l-275-275l-136 136l411 411z" />)"
				);
	}

	[[deprecated]] inline static auto remote_image(const std::string& url) {
		using namespace drogon;

		auto http = HttpClient::newHttpClient(url);
		auto req  = HttpRequest::newHttpRequest();
		req->setContentTypeCode(ContentType::CT_IMAGE_JPG);
		req->setMethod(HttpMethod::Get);

		const auto resp = http->sendRequest(req);
	}

	inline static auto avatar_alt(const Point& base = { 0, 0 }) {
		static Counter counter {};
	}

	inline static auto avatar(std::string_view url, const Point& base = { 0, 0 }) {
		using namespace Xml;

		static Counter counter {};
		// static cppcodec::base64_url b {};

		const auto mask_name = std::format("bscMask{}", counter.count());

		return make_sub(								   //
			mask()										   //
				.id(mask_name)							   //
				.sub(									   //
					rect()								   //
						.x(base.x)						   //
						.y(base.y)						   //
						.width(100)						   //
						.height(100)					   //
						.rx(20)							   //
						.ry(20)							   //
						.fill("white")					   //
				),										   //
			image()										   //
				.x(base.x - 8)							   //
				.y(base.y - 20)							   //
				.width(110)								   //
				.mask(std::format("url(#{})", mask_name))  //
				.href(url)								   //
		);
	}

	inline static auto watched_anime_block(const Anime& anime, const Point& base = { 0, 0 }) {
		using namespace Xml;

		return														  //
			svg()													  //
				.x(base.x)											  //
				.y(base.y)											  //
				.sub(												  //
					text()											  //
						.class_("bsc-name")							  //
						.x(5)										  //
						.y(42)										  //
						.sub(anime.name),							  //
					text()											  //
						.class_("bsc-details")						  //
						.x(5)										  //
						.y(81)										  //
						.sub(std::format("✅ {}", anime.updated_at))  //
				);
	}

	inline static auto progress_bar(float progress, const Point& base = { 0, 0 }) {
		using namespace Xml;

		return svg()
			.x(base.x + 14)			 //
			.y(base.y + 72)			 //
			.width(350)				 //
			.height(15)				 //
			.sub(					 //
				rect()				 //
					.fill("gray")	 //
					.width("100%")	 //
					.height("100%")	 //
					.ry(8)			 //
					.rx(8)			 //
				,					 //
				svg()				 //
					.width(std::format(
						"{}%",
						progress * 100.f
					))									 //
					.height("100%")						 //
					.sub(								 //
						rect()							 //
							.class_("bsc-progress-bar")	 //
							.fill("mediumaquamarine")	 //
							.height("100%")				 //
							.rx(8)						 //
							.ry(8)						 //
					)									 //
			);
	}

	inline static auto onwatching_anime_block(const Anime& anime, const Point& base = { 0, 0 }) {
		using namespace Xml;

		return								 //
			svg()							 //
				.x(base.x)					 //
				.y(base.y)					 //
				.sub(						 //
											 // avatar(anime.avatar),	 //
					text()					 //
						.class_("bsc-name")	 //
						// .x(120)				 //
						.x(5)			   //
						.y(42)			   //
						.sub(anime.name),  //
					progress_bar(
						static_cast<float>(anime.ep_status) / static_cast<float>(anime.eps)
					),															//
					text()														//
						.x(375)													//
						.y(86)													//
						.class_("bsc-details")									//
						.sub(std::format("{}/{}", anime.ep_status, anime.eps))	//
				);
	}

	inline static auto
		watched_anime_list(const std::vector<Anime>& animes, const Point& base = { 0, 0 }) {
		using namespace Xml;

		Counter counter {};

		return make_sub(animes | std::views::transform([&](const auto& a) {
							const auto cur = counter.count();
							return watched_anime_block(
								a,
								{ base.x + 5, static_cast<int32_t>(base.y + 5 + cur * 110) }
							);
						}));
	}

	inline static auto
		onwatching_anime_list(const std::vector<Anime>& animes, const Point& base = { 0, 0 }) {
		using namespace Xml;

		Counter counter {};

		return make_sub(animes | std::views::transform([&](const auto& a) {
							const auto cur = counter.count();
							return onwatching_anime_block(
								a,
								{ base.x + 5, static_cast<int32_t>(base.y + 5 + cur * 110) }
							);
						}));
	}

	inline static auto card(const UserData& data, int width = 800, int view_box_width = 800) {
		using namespace Xml;
		using namespace matchit;
		if (!data.title.empty())
			return													   //
				move_it |											   //
				svg()												   //
					.attribute("xmlns", "http://www.w3.org/2000/svg")  //
					.viewBox(std::format("0 0 {} {}", view_box_width, 85 + data.animes.size() * 110)
					)												   //
					.width(width)
					.height((85 + data.animes.size() * 110) * width / view_box_width)
					.attribute("role", "img")		 //
					.sub(							 //
						card_style(),				 //
						text()						 //
							.class_("bsc-title")	 //
							.x(12)					 //
							.y(53)					 //
							.sub(data.title),		 //
						svg()						 //
							.x("1%")				 //
							.y(74)					 //
							.width("98%")			 //
							.height(3)				 //
							.sub(					 //
								rect()				 //
									.fill("gray")	 //
									.width("100%")	 //
									.height("100%")	 //
							)						 //
						,							 //
						data.collection_type == BangumiCollectionType::Watched ?
							watched_anime_list(data.animes, { 0, 80 }) :
						data.collection_type == BangumiCollectionType::OnWathing ?
							onwatching_anime_list(data.animes, { 0, 80 }) :
							GeneralNodes {}
					);
		else
			return move_it |											  //
				   svg()												  //
					   .attribute("xmlns", "http://www.w3.org/2000/svg")  //
					   .viewBox(
						   std::format("0 0 {} {}", view_box_width, 5 + data.animes.size() * 110)
					   )  //
					   .width(width)
					   .height((5 + data.animes.size() * 110) * width / view_box_width)
					   .attribute("role", "img")  //
					   .sub(					  //
						   card_style(),
						   data.collection_type == BangumiCollectionType::Watched ?
							   watched_anime_list(data.animes) :
						   data.collection_type == BangumiCollectionType::OnWathing ?
							   onwatching_anime_list(data.animes) :
							   GeneralNodes {}
					   );
	}

}  // namespace BangumiStatusCard