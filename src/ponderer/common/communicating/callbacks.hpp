#pragma once

#include <functional>
#include <type_traits>

namespace ponderer::com {

template<typename Ret, typename... Params>
auto doNothing(Params...) {
	return Ret();
}

template<typename...>
class Callback;

template<typename Ret, typename... Params>
class Callback<Ret(Params...)> {
	using Function = Ret(Params...);

	static constexpr Function* doNothing_ = &doNothing<Ret, Params...>;

	Function* function_ = doNothing_;

public:
	Callback() = default;

	Callback(Function& fn)
	: function_(&fn)
	{}

	auto isEmpty() const -> bool {
		return function_ == doNothing_;
	}

	operator bool() const {
		return !isEmpty();
	}

	template<typename... Params_>
	auto operator()(Params_&&... ps) const -> decltype(auto) {
		return function_(std::forward<Params_>(ps)...);
	}
};

template<typename Ret, typename... Params>
Callback(Ret(*)(Params&&...)) -> Callback<Ret(Params...)>;

#if false

template<typename...>
class StatefulCallback;

template<typename Ret, typename... Params>
class StatefulCallback<Ret(Params...)> {
	using Function = Ret(void*, Params...);

	static constexpr Function* doNothing_ = &doNothing<Ret, void*, Params...>;

	Function* function_ = doNothing_;
	void* state_;

public:
	template<typename State>
	using StatefulFunction = Ret(State&, Params...);

	StatefulCallback() = default;

//	template<typename Callable>
//	explicit StatefulCallback(Callable& c)
//	: function_(reinterpret_cast<Function*>([](Callable& c) { return c(); }))
//	, state_(reinterpret_cast<void*>(&c))
//	{}
//
//	template<typename Callable>
//	explicit StatefulCallback(Callable* c)
//		: function_(reinterpret_cast<Function*>([](Callable& c) { return c(); }))
//		, state_(reinterpret_cast<void*>(c))
//	{}

	template<typename State>
	StatefulCallback(State& s, StatefulFunction<State>& fn)
	: function_(reinterpret_cast<Function*>(&fn))
	, state_(reinterpret_cast<void*>(&s))
	{}

	operator bool() const {
		return function_ != doNothing_;
	}

	template<typename... Params_>
	auto operator()(Params_&&... ps) const -> decltype(auto) {
		return function_(state_, std::forward<Params_>(ps)...);
	}
};

template<typename Ret, typename State, typename... Params>
StatefulCallback(Ret(*)(State&, Params&&...)) -> StatefulCallback<Ret(Params...)>;

};

#else

template<typename...>
class StatefulCallback;

template<typename Ret, typename... Params>
class StatefulCallback<Ret(Params...)> : private std::function<Ret(Params...)> {
	using Base = std::function<Ret(Params...)>;

public:
	using Base::Base;
	using Base::operator bool;
	using Base::operator();

	StatefulCallback()
	: Base(&doNothing<Ret(Params...)>)
	{}
};

#endif

}