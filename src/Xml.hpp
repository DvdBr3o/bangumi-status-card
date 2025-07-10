#pragma once

#include <forward_list>
#include <initializer_list>
#include <memory>
#include <string_view>
#include <string>
#include <unordered_map>
#include <utility>
#include <variant>
#include <format>
#include "Utils.hpp"

namespace BangumiStatusCard::Xml {
	using Attribute	 = std::pair<std::string_view, std::string>;
	using Attributes = std::unordered_map<std::string_view, std::string>;
	using Content	 = std::string;
	class Node;
	using sub		 = std::initializer_list<std::variant<Node, Content>>;
	using append_sub = std::initializer_list<std::variant<Node, Content>>;

	class Node {
	public:
		explicit Node(const char* name) : _name(name) {}

		// Node(const Node&)			 = default;
		// Node& operator=(const Node&) = default;

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
			((_children.emplace_back(std::move(subs))), ...);
			return *this;
		}

		template<typename... Subs>
		auto&& sub(Subs&&... subs) && {
			_children.reserve(sizeof...(subs));
			((_children.emplace_back(std::move(subs))), ...);
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
            auto& name(int value) & { return attribute(#name, std::to_string(value)); } \
            auto&& name(int value) && { return std::move(attribute(#name, std::to_string(value))); }
		// clang-format on

		BSC_NODE_DEFINE_ATTRIBUTE(class_)
		BSC_NODE_DEFINE_ATTRIBUTE(transform)
		BSC_NODE_DEFINE_ATTRIBUTE(viewBox)
		BSC_NODE_DEFINE_ATTRIBUTE(width)
		BSC_NODE_DEFINE_ATTRIBUTE(height)
		BSC_NODE_DEFINE_ATTRIBUTE(x)
		BSC_NODE_DEFINE_NUM_ATTRIBUTE(x)
		BSC_NODE_DEFINE_ATTRIBUTE(y)
		BSC_NODE_DEFINE_NUM_ATTRIBUTE(y)

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
							content_list += std::format("    {}", content);
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
		std::string								 _name;
		Attributes								 _attributes;
		std::vector<std::variant<Node, Content>> _children;
	};

	// clang-format off
    #define BSC_DEFINE_NODE(name)                                                                      \
	    auto name() { return Node { #name }; } \
	    auto name(const char* content) { return Node { #name }.sub(content); }
	// clang-format on

	BSC_DEFINE_NODE(svg)
	BSC_DEFINE_NODE(text)
	BSC_DEFINE_NODE(g)
	BSC_DEFINE_NODE(style)

}  // namespace BangumiStatusCard::Xml