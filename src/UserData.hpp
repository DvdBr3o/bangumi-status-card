#pragma once

#include <string>
#include <vector>

namespace BangumiStatusCard {
	enum class BangumiSubjectType : std::uint32_t {
		Book   = 1,
		Anime  = 2,
		Music  = 3,
		Game   = 4,
		NonACG = 5,
	};

	enum class BangumiCollectionType : std::uint32_t {
		WantToWatch = 1,
		Watched		= 2,
		OnWathing	= 3,
		Delayed		= 4,
		Deserted	= 5,
	};

	struct Anime {
		std::string				 name;
		std::string				 avatar;
		std::vector<std::string> tags;
	};

	struct UserData {
		std::vector<Anime> animes;
	};
}  // namespace BangumiStatusCard