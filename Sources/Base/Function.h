#pragma once
#include "Cross.h"

#include <cassert>

template<class T>
class Function;

/*	Function class needed for lazy binding. For example, you can bind a function or method 
	to this class object and call it later. */
template<class Ret, class... Input>
class Function<Ret(Input...)> {
public:
							Function() = default;
	template<class Lambda>	Function(const Lambda& lambda);
	template<class Class>	Function(Class* obj, Ret(Class::*meth)(Input... args));
							Function(const Function& other);
							Function(Function&& other);
							~Function();

	void operator = (const Function& other);
	void operator = (Function&& other);

	Ret operator ()(Input... args);
	operator bool() const;

private:
	void* lambda 					= nullptr;
	Ret(*executor)(void*, Input...) = nullptr;
	void*(*copier)(void*) 			= nullptr;
	void(*deleter)(void*)			= nullptr;

	template<class Lambda>
	void Init(const Lambda& other);
};

template<class Ret, class... Input>
void Function<Ret(Input...)>::operator = (const Function& other) {
	lambda = other.copier(other.lambda);
	executor = other.executor;
	copier = other.copier;
	deleter = other.deleter;
}

template<class Ret, class... Input>
void Function<Ret(Input...)>::operator = (Function&& other) {
	lambda = other.lambda;
	executor = other.executor;
	copier = other.copier;
	deleter = other.deleter;
	other.lambda = nullptr;
	other.executor = nullptr;
	other.copier = nullptr;
	other.deleter = nullptr;
}

template<class Ret, class... Input>
template<class Lambda>
Function<Ret(Input...)>::Function(const Lambda& lambda) {
	Init(lambda);
}

template<class Ret, class... Input>
template<class Class>
Function<Ret(Input...)>::Function(Class* obj, Ret(Class::*meth)(Input... args)) {
	Init([obj, meth](Input... args) {
		(obj->*meth)(args...);
	});
}

template<class Ret, class... Input>
Function<Ret(Input...)>::Function(const Function& other) {
	lambda = other.copier(other.lambda);
	executor = other.executor;
	copier = other.copier;
	deleter = other.deleter;
}

template<class Ret, class... Input>
Function<Ret(Input...)>::Function(Function&& other) {
	lambda = other.lambda;
	executor = other.executor;
	copier = other.copier;
	deleter = other.deleter;
	other.lambda = nullptr;
	other.executor = nullptr;
	other.copier = nullptr;
	other.deleter = nullptr;
}

template<class Ret, class... Input>
Function<Ret(Input...)>::~Function() {
	if(lambda) {
		deleter(lambda);
	}
}

template<class Ret, class... Input>
Ret Function<Ret(Input...)>::operator()(Input... args) {
	assert(lambda);
	return executor(lambda, args...);
}

template<class Ret, class... Input>
Function<Ret(Input...)>::operator bool() const {
	return lambda != nullptr;
}

template<class Ret, class... Input>
template<class Lambda>
void Function<Ret(Input...)>::Init(const Lambda& other) {
	lambda = CREATE Lambda(other);

	executor = [](void* lamb, Input... args) -> Ret {
		return ((Lambda*)lamb)->operator()(args...);
	};

	copier = [](void* source) -> void* {
		return CREATE Lambda(*(Lambda*)source);
	};

	deleter = [](void* lamb) {
		delete (Lambda*)lamb;
	};
}
