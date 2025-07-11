#pragma once

#include <string>
#include <type_traits>
#include <utility>
#include <cstdint>
#include <ranges>

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

	namespace details::element_type_of {
		template<typename T>
		struct _element_type_of {
			using type = std::void_t<>;
		};

		template<typename T>
			requires std::is_pointer_v<T>
		struct _element_type_of<T> {
			using type = std::remove_cv_t<std::remove_pointer_t<T>>;
		};

		template<typename T>
			requires std::is_array_v<T> && std::ranges::view<T>
		struct _element_type_of<T> {
			using type = std::remove_cv_t<std::decay_t<T>>;
		};

		template<typename T>
			requires std::ranges::view<T>
		struct _element_type_of<T> {
			using type = std::remove_cvref_t<decltype(*(std::declval<T>().begin()))>;
		};
	}  // namespace details::element_type_of

	template<typename T>
	using element_type_of =
		details::element_type_of::_element_type_of<std::remove_reference_t<T>>::type;

}  // namespace BangumiStatusCard