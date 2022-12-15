#pragma once

#include <optional>

namespace ponderer::com {

struct {} constexpr emptyReceipt;

template<typename Traits>
class ReceiverSlot {
	using Receiver = typename Traits::Receiver;

	std::optional<Receiver> receiver_;

public:
	using Receipt = typename Traits::Receipt;

	ReceiverSlot() = default;

	explicit ReceiverSlot(Receiver&&r)
	: receiver_(std::move(r))
	{}

	template<typename... Args>
	auto receive(Args&&... args) -> std::optional<Receipt> {
		if(receiver_) {
			return receiver_(std::forward<Args>(args)...);
		} else {
			return std::nullopt;
		}
	}
};

}
