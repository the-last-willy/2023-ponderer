#pragma once

#include "ponderer/common/communicating/none.hpp"

#include <functional>

namespace ponderer::com {

/// A link end to send to or receive from another one.
/// This abstraction is meant to allow sending/receiving data between two modules
/// without requiring knowledge about the other one.
/// Connection/disconnection is considered to be a third-party concern.
/// Therefore an end is notified when:
/// - it is connected;
/// - it is disconnected;
/// - it receives data.
/// A link is reciprocal; ends are always interconnected.
///
/// @note The other end is voluntarily not exposed to the user.
/// The goal of this interface is to provide a data channel
/// without exposing the sender or the receiver.
/// An end might be replaced unknowingly from the other end.
///
/// @TODO Move this section as it is an implementation detail.
///
/// @TODO Replace std::function.
/// `sizeof(LinkEnd) = 104 = 3 * sizeof(std::function) + sizeof(*)`
/// This is obscenely huge for what's meant to be a lightweight utility.
/// `std::function` is the obvious culprit (96 out of 104 bytes).
///
/// @TODO Accept multiple data types.
/// It agrees with replacing `std::function` which is too limiting .
///
/// @TODO Abstract away implementation choices.
///
/// @TODO Separate connection and notification concerns in implementation?
///
/// @TODO Add a `replace` function that does not notify the other end.
/// Otherwise the other end is successively notified twice:
/// once for the disconnection and once for the connection.
/// This warrants unnecessary work for the other end.
///
/// @TODO Thread safety? None. Does it need to have any?
template<typename Type = decltype(None)>
class LinkEnd {
public:
	using OnConnect = std::function<void()>;
	using OnDisconnect = std::function<void()>;
	using OnReceive = std::function<void(const Type&)>;

	/// A default constructed end is connected to itself.
	LinkEnd() = default;

	/// Disconnects the other end.
	/// @note It does not call `onDisconnect` on the *current* end.
	/// @TODO Should it ?
	~LinkEnd() {
		if(isConnected(*this)) {
			auto& other = otherRef();
			other.disconnectSelf();
		}
	}

	// Connecting.

	/// Interconnects two ends.
	/// Previously connected ends are disconnected before being connected.
	/// @pre `&e0 != &e1`; an end should not be interconnected to itself.
	/// On violation, the behaviour is incorrect but well-defined.
	/// The `onConnect` callbacks will be called twice when it shouldn't be called at all.
	/// @post  if `isConnected(e0)`, `onDisconnect(e0)` has been called
	/// @post `onConnect(e0)` has been called.
	friend void interconnect(LinkEnd& e0, LinkEnd& e1) {
		e0.connectSelfTo(e1);
		e1.connectSelfTo(e0);
	}

	friend void onReceive(LinkEnd& e, OnReceive&& fn) {
		e.onReceive_ = std::move(fn);
	}

	/// Sends data from the *current* end to the *other*.
	/// @post `onReceive(other)` has been called.
	template<typename... Args>
	friend void send(LinkEnd& e, Args&&... args) {
		auto& other = e.otherRef();
		other.callOnReceive(std::forward<Args>(args)...);
	}

	/// Tests if the end is connected.
	friend auto isConnected(const LinkEnd& e) -> bool {
		// A disconnected end is represented as an end connected to itself.
		return e.other_ != &e;
	}

	/// Tests if two ends are interconnected.
	friend auto areInterconnected(const LinkEnd& e0, const LinkEnd& e1) {
		// As per the reciprocity invariant, there is no need to test the other way.
		return e0.other_ == &e1;
	}

	/// Bilaterally disconnects an end.
	/// Calls `onDisconnect` on both ends.
	/// Connects both ends to themselves.
	/// @pre `isConnected(end)`
	/// On violation, calls `onDisconnect(end)` twice.
	/// @post `!isConnected(end)`
	/// @post `onDisconnect(end)` has been called.
	/// @post `!isConnected(other)`
	/// @post `onDisconnect(other)` has been called.
	friend void disconnect(LinkEnd& e) {
		auto& other = e.otherRef();
		other.disconnectSelf();
		e.disconnectSelf();
	}

	// Notifying.

	friend void onConnect(LinkEnd& e, OnConnect&& fn) {
		e.onConnect_ = std::move(fn);
	}

	friend void onDisconnect(LinkEnd& e, OnDisconnect&& fn) {
		e.onDisconnect_ = std::move(fn);
	}

private:
	void callOnConnect() {
		onConnect_();
	}

	void connectSelfTo(LinkEnd& e) {
		other_ = &e;
		if(!isConnected(*this)) {
			callOnConnect();
		}
	}

	auto otherRef() const -> LinkEnd& {
		return *other_;
	}

	template<typename... Args>
	auto callOnReceive(Args&&... args) const {
		onReceive_(std::forward<Args>(args)...);
	}

	void callOnDisconnect() {
		onDisconnect_();
	}

	void disconnectSelf() {
		callOnDisconnect();
		other_ = this;
	}

	/// @invariant `this = other_->other_`; a link is reciprocal.
	/// If an end is connected to another one
	/// then the other one is connected to this one.
	///
	/// @invariant `other_ != nullptr`
	/// An end is always connected to another one (which might be itself).
	/// @note A disconnected end is represented as an end connected to itself.
	/// An end is connected to itself (disconnected end) when:
	/// - it has been default constructed;
	/// - it has been disconnected.
	/// Calling certain functions on an end connected to itself
	/// will result in incorrect but well-defined behaviour.
	LinkEnd* other_ = this;

	/// @invariant `onConnect_` is a callable object.
	OnConnect onConnect_ = [](){};
	/// @invariant `onDisconnect_` is a callable object.
	OnDisconnect onDisconnect_ = [](){};
	/// @invariant `onReceive_` is a callable object.
	OnReceive onReceive_ = [](auto){};
};

}