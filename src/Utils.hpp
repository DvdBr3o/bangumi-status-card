#pragma once

namespace BangumiStatusCard {
	template<typename... Ts>
	struct overload : Ts... {
		using Ts::operator()...;
	};
}  // namespace BangumiStatusCard