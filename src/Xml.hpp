#pragma once

#include "Utils.hpp"

#include <memory>
#include <string_view>
#include <string>
#include <unordered_map>
#include <utility>
#include <variant>
#include <format>
#include <concepts>
#include <ranges>

namespace BangumiStatusCard::Xml {
	using Attribute	 = std::pair<std::string_view, std::string>;
	using Attributes = std::unordered_map<std::string_view, std::string>;
	using Content	 = std::string;
	class Node;
	using GeneralNode  = std::variant<Node, Content>;
	using GeneralNodes = std::vector<GeneralNode>;

	class Node {
	public:
	public:
		explicit Node(const char* name) : _name(name) {}

		Node(Node&& another) noexcept {
			if (this != std::addressof(another)) {
				_name		= std::move(another._name);
				_attributes = std::move(another._attributes);
				_children	= std::move(another._children);
			}
		}

		auto& operator=(Node&& another) noexcept {
			if (this != std::addressof(another)) {
				_name		= std::move(another._name);
				_attributes = std::move(another._attributes);
				_children	= std::move(another._children);
			}
			return *this;
		}

	public:
		template<typename... Subs>
		auto& sub(Subs&&... subs) & {
			_children.reserve(sizeof...(subs));
			((_append_child(std::move(subs))), ...);
			return *this;
		}

		template<typename... Subs>
		auto&& sub(Subs&&... subs) && {
			_children.reserve(sizeof...(subs));
			((_append_child(std::move(subs))), ...);
			return std::move(*this);
		}

	public:
		auto& attribute(std::string_view name, std::string_view value) & {
			_attributes[name] = value;
			return *this;
		}

		auto&& attribute(std::string_view name, std::string_view value) && {
			_attributes[name] = value;
			return *this;
		}

		// clang-format off
        #define BSC_NODE_DEFINE_ATTRIBUTE(name)                                                            \
            auto& name(std::string_view value) & { return attribute(#name, value); } \
            auto&& name(std::string_view value) && { return std::move(attribute(#name, value)); }
        #define BSC_NODE_DEFINE_NUM_ATTRIBUTE(name)                                                            \
		BSC_NODE_DEFINE_ATTRIBUTE(name) \
            auto& name(int value) & { return attribute(#name, std::to_string(value)); } \
            auto&& name(int value) && { return std::move(attribute(#name, std::to_string(value))); }
		// clang-format on

		BSC_NODE_DEFINE_ATTRIBUTE(class_)
		BSC_NODE_DEFINE_ATTRIBUTE(id)
		BSC_NODE_DEFINE_ATTRIBUTE(transform)
		BSC_NODE_DEFINE_ATTRIBUTE(viewBox)
		BSC_NODE_DEFINE_ATTRIBUTE(fill)
		BSC_NODE_DEFINE_ATTRIBUTE(mask)
		BSC_NODE_DEFINE_ATTRIBUTE(href)
		BSC_NODE_DEFINE_NUM_ATTRIBUTE(width)
		BSC_NODE_DEFINE_NUM_ATTRIBUTE(height)
		BSC_NODE_DEFINE_NUM_ATTRIBUTE(x)
		BSC_NODE_DEFINE_NUM_ATTRIBUTE(y)
		BSC_NODE_DEFINE_NUM_ATTRIBUTE(rx)
		BSC_NODE_DEFINE_NUM_ATTRIBUTE(ry)

	public:
		[[nodiscard]] auto to_string_loose(bool indent = false) const -> std::string {
			std::string attribute_list;
			std::string content_list;
			for (const auto& attr : _attributes)
				attribute_list += std::format("{}=\"{}\" ", attr.first, attr.second);
			for (const auto& child : _children) {
				std::visit(
					overload {
						[&content_list](const Content& content) {
							content_list += std::format("    {}\n", content);
						},
						[&content_list](const Node& node) {
							content_list += node.to_string_loose(true);
						},
					},
					child
				);
			}

			if (!_children.empty())
				return std::format(
					"{3}<{0} {1}>\n"
					"{3}{2}\n"
					"{3}</{0}>\n",
					_name,
					std::move(attribute_list),
					std::move(content_list),
					indent ? "    " : ""
				);
			else
				return std::format(
					"{2}<{0} {1}> </{0}>\n",
					_name,
					std::move(attribute_list),
					indent ? "    " : ""
				);
		}

		[[nodiscard]] auto to_string_compact() const -> std::string {
			std::string attribute_list;
			std::string content_list;
			for (const auto& attr : _attributes)
				attribute_list += std::format("{}=\"{}\" ", attr.first, attr.second);
			for (const auto& child : _children) {
				std::visit(
					overload {
						[&content_list](const Content& content) { content_list += content; },
						[&content_list](const Node& node) {
							content_list += node.to_string_compact();
						},
					},
					child
				);
			}
			return std::format(
				"<{0} {1}>{2}</{0}>",
				_name,
				std::move(attribute_list),
				std::move(content_list)
			);
		}

	private:
		auto _append_child(auto&& child) -> void {
			if constexpr (std::same_as<std::decay_t<decltype(child)>, GeneralNodes>
						  || std::ranges::view<decltype(child)>
						  || std::ranges::range<decltype(child)>)
				for (auto&& c : child) _children.emplace_back(std::move(c));
			else
				_children.emplace_back(std::forward<decltype(child)>(child));
		}

	private:
		std::string	 _name;
		Attributes	 _attributes;
		GeneralNodes _children;
	};

	template<typename... Ts>
	auto make_sub(Ts&&... ts) -> GeneralNodes {
		GeneralNodes nodes;
		nodes.reserve(sizeof...(ts));
		((nodes.emplace_back(std::forward<Ts>(ts))), ...);
		return nodes;
	}

	// clang-format off
    #define BSC_DEFINE_NODE(name)                                                                      \
	    auto name() { return Node { #name }; } \
	    auto name(const char* content) { return Node { #name }.sub(content); }
	// clang-format on

	BSC_DEFINE_NODE(svg)
	BSC_DEFINE_NODE(text)
	BSC_DEFINE_NODE(g)
	BSC_DEFINE_NODE(style)
	BSC_DEFINE_NODE(image)
	BSC_DEFINE_NODE(mask)
	BSC_DEFINE_NODE(rect)

}  // namespace BangumiStatusCard::Xml
