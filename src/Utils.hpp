#pragma once

#include <utility>
#include <cstdint>

namespace BangumiStatusCard {
	template<typename... Ts>
	struct overload : Ts... {
		using Ts::operator()...;
	};

	struct move_it_t {
		template<typename T>
		friend auto operator|(move_it_t, T&& value) {
			return static_cast<std::decay_t<T>&&>(value);
		}
	};

	inline static constexpr auto move_it = move_it_t {};

	class Counter {
	public:
		Counter() = default;

		explicit Counter(std::uint32_t init_count) : _count(init_count) {}

	public:
		auto count() -> std::uint32_t { return _count++; }

	private:
		std::uint32_t _count = 0;
	};

}  // namespace BangumiStatusCard