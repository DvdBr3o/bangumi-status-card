#pragma once

#include "UserData.hpp"
#include "Utils.hpp"
#include "Xml.hpp"

#include <ranges>

namespace BangumiStatusCard {
	inline static constexpr auto style_text = R"(
.bsc-image {
    width: 150px;
    height: 150px;
    border-radius: 5px;
    object-fit: none;
    object-position: 50% 50%;
    display: block;
    position: relative;
    left: -10px;
    right: 0;
    top: -10px;
    bottom: 0;
}

.bsc-text {
    font: 600 30px 'Segoe UI', Ubuntu, Sans-Serif;
    fill: #040e1c;
}
)";

	inline static auto			 card_style = Xml::style(style_text);

	inline static auto			 avatar(std::string_view url) {
		  using namespace Xml;

		  static Counter counter {};

		  const auto	 mask_name = std::format("bscMask{}", counter.count());

		  return make_sub(									 //
			  mask()										 //
				  .id(mask_name)
				  .sub(										 //
					  rect()								 //
						  .x(0)								 //
						  .y(0)								 //
						  .width(100)						 //
						  .height(100)						 //
						  .rx(20)							 //
						  .ry(20)							 //
						  .fill("white")					 //
				  ),
			  image()										 //
				  .class_("bsc-image")						 //
				  .x(25)									 //
				  .y(20)									 //
				  .mask(std::format("url(#{})", mask_name))	 //
				  .href(url)								 //
		  );
	}

	inline static auto card(const UserData& data, int width, int height) {
		using namespace Xml;

		return move_it
			 | svg()												  //
				   .attribute("xmlns", "http://www.w3.org/2000/svg")  //
				   .width(width)									  //
				   .height(height)									  //
				   .attribute("role", "img")
				   .sub(											  //
					   card_style,
					   data.animes | std::views::transform([](const Anime& a) {
						   return avatar(a.avatar);
					   }) | std::views::join
				   );
	}

}  // namespace BangumiStatusCard