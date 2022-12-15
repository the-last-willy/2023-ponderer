#pragma once

#include <functional>
#include <memory>
#include <vector>

namespace ponderer::com {

template<typename... Message>
using Listener = std::function<void(Message...)>;

/// An object to attach listeners to.
/// One to many.
/// @todo Find the right name : subject, source, stream source, ...
/// @todo Decorrelate interface from std.
template<typename... Type>
class Emitter {
public:
	using Listener = com::Listener<Type...>;

private:
	std::vector<std::weak_ptr<Listener>> connections_;

public:
	void connect(const std::shared_ptr<Listener>& listener) {
		connections_.push_back(listener);
	}

	/// @todo Refactor with ranges.
	/// It's not straightforward to generalize because it works directly on the iterator.
	bool disconnect(const std::shared_ptr<Listener>& listener) {
		auto found = false;
		// @todo Linear time. Is it bad ? A source shouldn't have that many listeners ?
		for(auto it = begin(connections_); it != end(connections_);) {
			if(auto connection = it->lock()) {
				if(connection.get() != listener.get()) {
					// Continues searching.
					++it;
				} else {
					// Removes target and exits.
					found = true;
					connections_.erase(it);
					break;
				}
			} else {
				// Removes connections that are invalid.
				it = connections_.erase(it);
			}
		}
		return true;
	}

	void emit(Type&... t) const {
		for(auto it = begin(connections_); it != end(connections_);) {
			if(auto connection = it->lock()) {
				(*connection)(t...);
				++it;
			} else {
				// Removes connections that are invalid.
				it = connections_.erase(it);
			}
		}
	}
};

}