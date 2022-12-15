#pragma once

#include "ponderer/common/communicating/link_end.hpp"
#include "ponderer/common/communicating/none.hpp"

#include <catch2/catch_test_macros.hpp>

using namespace ponderer::com;

struct LinkEndTestContext {
	auto triggerFor(bool& b) {
		return [&b]() { b = true; };
	}

	auto onConnect() {
		return triggerFor(wasConnected);
	}

	auto onDisconnect() {
		return triggerFor(wasDisconnected);
	}

	auto onOtherConnect() {
		return triggerFor(wasOtherConnected);
	}

	auto onOtherDisconnect() {
		return triggerFor(wasOtherDisconnected);
	}

	bool wasConnected = false;
	bool wasDisconnected = false;

	bool wasOtherConnected = false;
	bool wasOtherDisconnected = false;
};

/// Scenarios:
///     Lifetime:
///         - A `LinkEnd` is default constructed.
///         - A `LinkEnd` is moved.
///         - A `LinkEnd` is destroyed.
///     - A `LinkEnd` is interconnected to a disconnected one.
///     - A `LinkEnd` is disconnected.
///     - A `LinkEnd` sends data.
///     - A `LinkEnd` receives data.

SCENARIO("A `LinkEnd` is connected to a default constructed one.") {
	auto ctx = LinkEndTestContext();

	GIVEN("A default constructed one") {
		auto end = LinkEnd();

		WHEN("It is connected to a default constructed one") {
			auto other =

			THEN("It is connected") {

			}
			THEN("The other one is connected") {

			}
		}
	}
	GIVEN("A moved one") {

	}
	GIVEN("An already connected one") {
		auto end = LinkEnd();
		auto previous = LinkEnd();
		interconnect(end, previous);

		onConnect(end, ctx.onConnect());
		onDisconnect(end, ctx.onDisconnect());

		WHEN("It is connected to a default constructed one") {
			auto other = LinkEnd();
			onConnect(other, ctx.onOtherConnect());
			onDisconnect(other, ctx.onOtherDisconnect());

			interconnect(end, other);

			THEN("It was not notified of its ") {

			}
			THEN("The other one is connected") {

			}
		}
	}
}

//SCENARIO("A default constructed `LinkEnd` is destroyed.") {
//	auto context = LinkContext();
//
//	GIVEN("") {
//		WHEN("It is destroyed") {
//			{
//				auto end = LinkEnd();
//				onDisconnect(end, context.onDisconnect());
//			}
//
//			THEN("it does not trigger its own `onDisconnect`.") {
//				REQUIRE(!context.wasDisconnected);
//			}
//		}
//	}
//}
//
//SCENARIO("A connected `LinkEnd` is destroyed.") {
//	auto context = LinkContext();
//
//	GIVEN("An end") {
//		auto other = LinkEnd();
//		onDisconnect(other, context.onOtherDisconnect());
//
//		WHEN("Another end is connected and destroyed") {
//			{
//				auto end = LinkEnd();
//				onDisconnect(end, context.onDisconnect());
//			}
//
//			THEN("It does not trigger its own `onDisconnect`.") {
//				REQUIRE(!context.wasDisconnected);
//			}
//			THEN("It triggers the other `onDisconnect`") {
//				REQUIRE(context.wasOtherDisconnected);
//			}
//		}
//	}
//}
//
//SCENARIO("A `LinkEnd` can be default constructed") {
//	GIVEN("A default constructed one") {
//		auto end = LinkEnd();
//
//		bool wasConnected = false;
//		onConnect(end, [&wasConnected]() { wasConnected = true; });
//
//		bool wasDisconnected = false;
//		onDisconnect(end, [&wasDisconnected]() { wasDisconnected = true; });
//
//		WHEN("It is connected") {
//			auto other = LinkEnd();
//			interconnect(end, other);
//
//			THEN("It triggers `onConnect`") {
//				// REQUIRE(wasConnected);
//			}
//			THEN("It does not trigger `onDisconnect`") {
//				// REQUIRE(!wasDisconnected);
//			}
//		}
//		WHEN("it is disconnected (precondition violation)") {
//
//
//			THEN("it triggers `onDisconnect`") {
//
//			}
//		}
//		WHEN("it is destroyed") {
//
//
//			THEN("it does not trigger `onDisconnect`") {
//
//			}
//		}
//	}
//}
//
//SCENARIO("A `LinkEnd` is notified when it is disconnected from the other end.") {
//	GIVEN("A connected one") {
//		auto le0 = LinkEnd();
//		auto le1 = LinkEnd();
//		interconnect(le0, le1);
//
//
//	}
//}
