#pragma once

#include <string>
#include <vector>
#include <cstdint>

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
		std::string	  name;
		std::string	  avatar;
		std::string	  updated_at;
		std::uint32_t ep_status;
		std::uint32_t eps;
	};

	struct UserData {
		std::vector<Anime>	  animes;
		std::string			  title;
		BangumiCollectionType collection_type;
	};
}  // namespace BangumiStatusCard