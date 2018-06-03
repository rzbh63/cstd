/**
 *	Any
 *	Nana C++ Library(http://www.nanapro.org)
 *	Copyright(C) 2003-2016 Jinhao(cnjinhao@hotmail.com)
 *
 *	Distributed under the Boost Software License, Version 1.0.
 *	(See accompanying file LICENSE_1_0.txt or copy at
 *	http://www.boost.org/LICENSE_1_0.txt)
 *
 *	@file  nana/any.hpp
 *
 *	@brief An implementation of experimental library any of C++ standard(http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2015/n4480.html#any)
 */

#ifndef NANA_ANY_HPP
#define NANA_ANY_HPP
#include <typeinfo>
#include <type_traits>

#include "c++defines.hpp"

namespace nana
{

	class bad_any_cast
		: public bad_cast
	{
	};

	class any
	{
		class content_interface
		{
		public:
			virtual ~content_interface() = default;
			
			virtual const type_info& type() const noexcept = 0;
			virtual content_interface* clone() const = 0;
		};

		template<typename Value>
		class holder : public content_interface
		{
			holder& operator=(const holder&) {}
		public:
			holder(const Value& other)
				: value(other)
			{}

			holder(Value&& other)
				: value(static_cast<Value&&>(other))
			{}
		public:
			const type_info& type() const noexcept
			{
				return typeid(Value);
			}

			content_interface* clone() const
			{
				return new holder(value);
			}
		public:
			Value value;	//representation accessable for friend of any
		};
	public:
		//constructors and destructor
		any() noexcept;

		any(const any& other);
		any(any&& other) noexcept;

		template<typename Value>
		any(Value && value,
				typename enable_if<!is_same<any&, Value>::value>::type * = NULL,
				typename enable_if<!is_const<Value>::value>::type* = NULL)
			: content_(new holder<typename decay<Value>::type>(static_cast<Value&&>(value)))
		{
		}

		~any();

		//assignments
		any& operator=(const any& other);
		any& operator=(any&& other) noexcept;

		template<class Value>
		any& operator=(Value&& other)
		{
			any(other).swap(*this);
			return *this;
		}

		//modifiers
		void clear() noexcept;
		void swap(any& other) noexcept;

		//observers
		bool empty() const noexcept;
		const type_info& type() const noexcept;
	private:
		template<typename Value>
		friend Value* any_cast(any*) noexcept;
	private:
		content_interface * content_;
	};

	// Non-member functions
	inline void swap(any& x, any& y) noexcept
	{
		x.swap(y);
	}

	template<typename Value>
	Value any_cast(const any& operand)
	{
		using value_type = typename remove_reference<Value>::type;
		return any_cast<const value_type&>(const_cast<any&>(operand));
	}

	template<typename Value>
	Value any_cast(any& operand)
	{
		using value_type = typename remove_reference<Value>::type;

		auto value_ptr = any_cast<value_type>(&operand);
		if (!value_ptr)
			throw bad_any_cast();

		using ref_type = typename conditional<is_reference<Value>::value, Value, typename add_lvalue_reference<Value>::type>::type;
		return static_cast<ref_type>(*value_ptr);
	}

	template<typename Value>
	Value any_cast(any && operand)
	{
		static_assert(is_rvalue_reference<Value&&>::value || is_const<typename ::remove_reference<Value>::type>::value, "any_cast shall not be used for getting non-const reference to temporary objects");
		return any_cast<Value>(operand);
	}

	template<typename Value>
	const Value* any_cast(const any* operand) noexcept
	{
		return any_cast<Value>(const_cast<any*>(operand));
	}

	template<typename Value>
	Value* any_cast(any* operand) noexcept
	{
		if (!operand)
			return NULL;

		auto holder = dynamic_cast<any::holder<typename decay<Value>::type>*>(operand->content_);
		return (holder ? &holder->value : NULL);
	}

}//end namespace nana

#endif