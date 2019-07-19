/************************************************************************************
MIT License

Copyright (c) 2013-2019 Yemiez

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*************************************************************************************/
#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>

namespace simba {
	constexpr auto VERSION_STRING = "1.0.0";
	constexpr const char SIMBA_HEADER[] = { 'S', 'I', 'M', 'B', 'A' };
	constexpr auto SIMBA_HEADER_LEN = sizeof(SIMBA_HEADER);

	class simba_value;

	template<typename T>
	std::pair<std::string, simba_value> pair(std::string str, T value);

	template<typename T>
	static simba_value val(T&& value);

	static simba_value val();

	template<typename ...Args>
	static simba_value array(Args&& ...args);

	template<typename ...Args>
	static simba_value object(Args&& ...args);

	namespace details {
		static std::uint8_t swap_uint8(std::uint8_t val);
		static std::int8_t swap_int8(std::int8_t val);
		static std::uint16_t swap_uint16(std::uint16_t val);
		static std::int16_t swap_int16(std::int16_t val);
		static std::uint32_t swap_uint32(std::uint32_t val);
		static std::int32_t swap_int32(std::int32_t val);
		static std::int64_t swap_int64(std::int64_t val);
		static std::uint64_t swap_uint64(std::uint64_t val);
		static std::uint8_t getEndianess();
		static bool hasTypeFlag(const std::uint8_t& type);

		template<typename ...Args>
		struct array_impl;

		template<typename ...Args>
		struct object_impl;

		// Output
		class simba_output_adapter;
		class simba_stream_output_adapter;
		class simba_serializer;

		// Input
		class simba_input_adapter;
		class simba_string_input_adapter;
		class simba_stream_input_adapter;
		class simba_deserializer;
	}

	enum simba_endianess : std::uint8_t {
		default_endianess,
		little_endian,
		big_endian
	};

	enum simba_type_t : std::uint8_t {
		simba_type_null,
		simba_type_int8,
		simba_type_int16,
		simba_type_int32,
		simba_type_int64,
		simba_type_float,
		simba_type_double,
		simba_type_array,
		simba_type_object,
		simba_type_map = simba_type_object,
		simba_type_string8,
		simba_type_string16,
		simba_type_string32,
		simba_type_string_w
	};

	enum simba_type_flag_t : std::uint8_t {
		simba_type_flag_signed,
		simba_type_flag_unsigned
	};

	class simba_value
	{
	public: // public types and func prototypes
		using simba_object_type = std::map<std::string, simba_value>;
		using simba_map_type = simba_object_type;
		using simba_array_type = std::vector<simba_value>;

		union simba_simple_type {
			std::int8_t int8;
			std::uint8_t uint8;
			std::int16_t int16;
			std::uint16_t uint16;
			std::int32_t int32;
			std::uint32_t uint32;
			std::int64_t int64;
			std::uint64_t uint64;
			float floatVal;
			double doubleVal;
		};

	public: // Constructors
		simba_value()
		{}
		simba_value(std::nullptr_t)
			: simba_value()
		{}
		simba_value(const simba_value& other)
		{
			*this = other;
		}
		simba_value(simba_value&& other) noexcept
		{
			*this = std::move(other);
		}
		simba_value(const std::int8_t& int8)
			: simba_value()
		{
			*this = int8;
		}
		simba_value(const std::uint8_t& uint8)
			: simba_value()
		{
			*this = uint8;
		}
		simba_value(const std::int16_t& int16)
			: simba_value()
		{
			*this = int16;
		}
		simba_value(const std::uint16_t& uint16)
			: simba_value()
		{
			*this = uint16;
		}
		simba_value(const std::int32_t& int32)
			: simba_value()
		{
			*this = int32;
		}
		simba_value(const std::uint32_t& uint32)
			: simba_value()
		{
			*this = uint32;
		}
		simba_value(const std::int64_t& int64)
			: simba_value()
		{
			*this = int64;
		}
		simba_value(const std::uint64_t& uint64)
			: simba_value()
		{
			*this = uint64;
		}
		simba_value(const float& floatVal)
			: simba_value()
		{
			*this = floatVal;
		}
		simba_value(const double& doubleVal)
			: simba_value()
		{
			*this = doubleVal;
		}
		simba_value(std::string string)
			: simba_value()
		{
			*this = std::move(string);
		}
		simba_value(std::wstring wstring)
			: simba_value()
		{
			*this = std::move(wstring);
		}
		simba_value(std::u16string u16string)
			: simba_value()
		{
			*this = std::move(u16string);
		}
		simba_value(std::u32string u32string)
			: simba_value()
		{
			*this = std::move(u32string);
		}
		simba_value(std::vector<simba_value> arrayValue)
			: simba_value()
		{
			*this = std::move(arrayValue);
		}
		simba_value(std::initializer_list<simba_value> arrayValue)
			: simba_value()
		{
			*this = simba_array_type{ std::move(arrayValue) };
		}
		simba_value(std::map<std::string, simba_value> objectValue)
			: simba_value()
		{
			*this = std::move(objectValue);
		}
		simba_value(std::initializer_list<std::pair<std::string, simba_value>> objectValue)
			: simba_value()
		{
			*this = std::map<std::string, simba_value>(objectValue.begin(), objectValue.end());
		}
		~simba_value()
		{
			this->destroyPtr();
		}

	public: // general functions
		void abandon() // abandon ownership of any ptrs
		{
			this->simbaType = simba_type_null;
			this->simpleValue = nullptr;
			this->objectValue = nullptr;
			this->arrayValue = nullptr;
			this->string = nullptr;
			this->u16string = nullptr;
			this->u32string = nullptr;
			this->wstring = nullptr;
		}

		// type, one of simba_type_t
		const std::uint8_t& getType() const noexcept
		{
			return this->simbaType;
		}

		// type flag, one of simba_type_flag_t
		// only releveant on types that can be signed/unsigned
		const std::uint8_t& getTypeFlag() const noexcept
		{
			return this->simbaTypeFlag;
		}

		inline bool isSigned() const noexcept
		{
			return this->simbaTypeFlag == simba_type_flag_signed;
		}

		inline bool isUnsigned() const noexcept
		{
			return this->simbaTypeFlag == simba_type_flag_unsigned;
		}

		// only relevant for object/map and array.
		std::uint32_t size() const noexcept
		{
			if (this->simbaType == simba_type_object) {
				return this->objectValue->size();
			}
			else if (this->simbaType == simba_type_array) {
				return this->arrayValue->size();
			}

			return 0u;
		}

		// only relevant for string types.
		std::uint32_t length() const noexcept
		{
			if (this->simbaType == simba_type_string8) {
				return this->string->length();
			}
			else if (this->simbaType == simba_type_string16) {
				return this->u16string->length();
			}
			else if (this->simbaType == simba_type_string32) {
				return this->u32string->length();
			}
			else if (this->simbaType == simba_type_string_w) {
				return this->wstring->length();
			}

			return 0u;
		}

		simba_array_type& getArray()
		{
			return this->get<simba_array_type>();
		}

		simba_object_type& getObject()
		{
			return this->get<simba_object_type>();
		}

		const simba_array_type& getArray() const
		{
			return this->get<simba_array_type>();
		}

		const simba_object_type& getObject() const
		{
			return this->get<simba_object_type>();
		}

#pragma region

		template<typename T>
		const T& get() const
		{
			static_assert("No possible conversion for T");
		}

		template<>
		const std::int8_t& get<>() const
		{
			if (this->simbaType != simba_type_int8 || this->simbaTypeFlag != simba_type_flag_signed) {
				throw std::exception("Attempted to retrieve int8_t when simba value isn't a int8_t (use cast instead).");
			}

			return this->simpleValue->int8;
		}

		template<>
		const std::int16_t& get<>() const
		{
			if (this->simbaType != simba_type_int16 || this->simbaTypeFlag != simba_type_flag_signed) {
				throw std::exception("Attempted to retrieve int16_t when simba value isn't a int16_t (use cast instead).");
			}

			return this->simpleValue->int16;
		}

		template<>
		const std::int32_t& get<>() const
		{
			if (this->simbaType != simba_type_int32 || this->simbaTypeFlag != simba_type_flag_signed) {
				throw std::exception("Attempted to retrieve int32 when simba value isn't a int32 (use cast instead).");
			}

			return this->simpleValue->int32;
		}

		template<>
		const std::int64_t& get<>() const
		{
			if (this->simbaType != simba_type_int64 || this->simbaTypeFlag != simba_type_flag_signed) {
				throw std::exception("Attempted to retrieve int64_t when simba value isn't a int64_t (use cast instead).");
			}

			return this->simpleValue->int64;
		}

		template<>
		const std::uint8_t& get<>() const
		{
			if (this->simbaType != simba_type_int8 || this->simbaTypeFlag != simba_type_flag_unsigned) {
				throw std::exception("Attempted to retrieve uint8_t when simba value isn't a uint8_t (use cast instead).");
			}

			return this->simpleValue->uint8;
		}

		template<>
		const std::uint16_t& get<>() const
		{
			if (this->simbaType != simba_type_int16 || this->simbaTypeFlag != simba_type_flag_unsigned) {
				throw std::exception("Attempted to retrieve uint16_t when simba value isn't a uint16_t (use cast instead).");
			}

			return this->simpleValue->uint16;
		}

		template<>
		const std::uint32_t& get<>() const
		{
			if (this->simbaType != simba_type_int32 || this->simbaTypeFlag != simba_type_flag_unsigned) {
				throw std::exception("Attempted to retrieve uint32_t when simba value isn't a uint32_t (use cast instead).");
			}

			return this->simpleValue->uint32;
		}

		template<>
		const std::uint64_t& get<>() const
		{
			if (this->simbaType != simba_type_int64 || this->simbaTypeFlag != simba_type_flag_unsigned) {
				throw std::exception("Attempted to retrieve uint64_t when simba value isn't a uint64_t (use cast instead).");
			}

			return this->simpleValue->uint64;
		}

		template<>
		const float& get<>() const
		{
			if (this->simbaType != simba_type_float) {
				throw std::exception("Attempted to retrieve a float when simba value isn't a float (use cast instead)");
			}

			return this->simpleValue->floatVal;
		}

		template<>
		const double& get<>() const
		{
			if (this->simbaType != simba_type_double) {
				throw std::exception("Attempted to retrieve a double when simba value isn't a double (use cast instead)");
			}

			return this->simpleValue->doubleVal;
		}

		template<>
		const simba_object_type& get<>() const
		{
			if (this->simbaType != simba_type_object) {
				throw std::exception("Attempted to retrieve an object when simba value isn't an object");
			}

			return *this->objectValue;
		}

		template<>
		const simba_array_type& get<>() const
		{
			if (this->simbaType != simba_type_array) {
				throw std::exception("Attempted to retrieve an array when simba value isn't an array");
			}

			return *this->arrayValue;
		}

		template<>
		const std::string& get<>() const
		{
			if (this->simbaType != simba_type_string8) {
				throw std::exception("Attempted to retrieve a string when simba value isn't a string");
			}

			return *this->string;
		}

		template<>
		const std::u16string& get<>() const
		{
			if (this->simbaType != simba_type_string16) {
				throw std::exception("Attempted to retrieve a u16string when simba value isn't a u16string");
			}

			return *this->u16string;
		}

		template<>
		const std::u32string& get<>() const
		{
			if (this->simbaType != simba_type_string32) {
				throw std::exception("Attempted to retrieve a u32string when simba value isn't a u32string");
			}

			return *this->u32string;
		}

		template<>
		const std::wstring& get<>() const
		{
			if (this->simbaType != simba_type_string_w) {
				throw std::exception("Attempted to retrieve a wstring when simba value isn't a wstring");
			}

			return *this->wstring;
		}

		template<typename T>
		T& get()
		{
			static_assert("No possible conversion for T");
		}

		template<>
		std::int8_t& get<>()
		{
			if (this->simbaType != simba_type_int8 || this->simbaTypeFlag != simba_type_flag_signed) {
				throw std::exception("Attempted to retrieve int8_t when simba value isn't a int8_t (use cast instead).");
			}

			return this->simpleValue->int8;
		}

		template<>
		std::int16_t& get<>()
		{
			if (this->simbaType != simba_type_int16 || this->simbaTypeFlag != simba_type_flag_signed) {
				throw std::exception("Attempted to retrieve int16_t when simba value isn't a int16_t (use cast instead).");
			}

			return this->simpleValue->int16;
		}

		template<>
		std::int32_t& get<>()
		{
			if (this->simbaType != simba_type_int32 || this->simbaTypeFlag != simba_type_flag_signed) {
				throw std::exception("Attempted to retrieve int32 when simba value isn't a int32 (use cast instead).");
			}

			return this->simpleValue->int32;
		}

		template<>
		std::int64_t& get<>()
		{
			if (this->simbaType != simba_type_int64 || this->simbaTypeFlag != simba_type_flag_signed) {
				throw std::exception("Attempted to retrieve int64_t when simba value isn't a int64_t (use cast instead).");
			}

			return this->simpleValue->int64;
		}

		template<>
		std::uint8_t& get<>()
		{
			if (this->simbaType != simba_type_int8 || this->simbaTypeFlag != simba_type_flag_unsigned) {
				throw std::exception("Attempted to retrieve uint8_t when simba value isn't a uint8_t (use cast instead).");
			}

			return this->simpleValue->uint8;
		}

		template<>
		std::uint16_t& get<>()
		{
			if (this->simbaType != simba_type_int16 || this->simbaTypeFlag != simba_type_flag_unsigned) {
				throw std::exception("Attempted to retrieve uint16_t when simba value isn't a uint16_t (use cast instead).");
			}

			return this->simpleValue->uint16;
		}

		template<>
		std::uint32_t& get<>()
		{
			if (this->simbaType != simba_type_int32 || this->simbaTypeFlag != simba_type_flag_unsigned) {
				throw std::exception("Attempted to retrieve uint32_t when simba value isn't a uint32_t (use cast instead).");
			}

			return this->simpleValue->uint32;
		}

		template<>
		std::uint64_t& get<>()
		{
			if (this->simbaType != simba_type_int64 || this->simbaTypeFlag != simba_type_flag_unsigned) {
				throw std::exception("Attempted to retrieve uint64_t when simba value isn't a uint64_t (use cast instead).");
			}

			return this->simpleValue->uint64;
		}

		template<>
		float& get<>()
		{
			if (this->simbaType != simba_type_float) {
				throw std::exception("Attempted to retrieve a float when simba value isn't a float (use cast instead)");
			}

			return this->simpleValue->floatVal;
		}

		template<>
		double& get<>()
		{
			if (this->simbaType != simba_type_double) {
				throw std::exception("Attempted to retrieve a double when simba value isn't a double (use cast instead)");
			}

			return this->simpleValue->doubleVal;
		}

		template<>
		simba_object_type& get<>()
		{
			if (this->simbaType != simba_type_object) {
				throw std::exception("Attempted to retrieve an object when simba value isn't an object");
			}

			return *this->objectValue;
		}

		template<>
		simba_array_type& get<>()
		{
			if (this->simbaType != simba_type_array) {
				throw std::exception("Attempted to retrieve an array when simba value isn't an array");
			}

			return *this->arrayValue;
		}

		template<>
		std::string& get<>()
		{
			if (this->simbaType != simba_type_string8) {
				throw std::exception("Attempted to retrieve a string when simba value isn't a string");
			}

			return *this->string;
		}

		template<>
		std::u16string& get<>()
		{
			if (this->simbaType != simba_type_string16) {
				throw std::exception("Attempted to retrieve a u16string when simba value isn't a u16string");
			}

			return *this->u16string;
		}

		template<>
		std::u32string& get<>()
		{
			if (this->simbaType != simba_type_string32) {
				throw std::exception("Attempted to retrieve a u32string when simba value isn't a u32string");
			}

			return *this->u32string;
		}

		template<>
		std::wstring& get<>()
		{
			if (this->simbaType != simba_type_string_w) {
				throw std::exception("Attempted to retrieve a wstring when simba value isn't a wstring");
			}

			return *this->wstring;
		}

		template<typename T>
		T cast()
		{
			if constexpr (!std::is_arithmetic_v<T>) {
				static_assert("Cannot cast to a non-arithmetic value");
			}

			switch (this->simbaType) {
			case simba_type_int8:
				if (this->simbaTypeFlag == simba_type_flag_signed) {
					return static_cast<T>(this->simpleValue->int8);
				}
				return static_cast<T>(this->simpleValue->uint8);
				break;
			case simba_type_int16:
				if (this->simbaTypeFlag == simba_type_flag_signed) {
					return static_cast<T>(this->simpleValue->int16);
				}
				return static_cast<T>(this->simpleValue->uint16);
				break;
			case simba_type_int32:
				if (this->simbaTypeFlag == simba_type_flag_signed) {
					return static_cast<T>(this->simpleValue->int32);
				}
				return static_cast<T>(this->simpleValue->uint32);
				break;
			case simba_type_int64:
				if (this->simbaTypeFlag == simba_type_flag_signed) {
					return static_cast<T>(this->simpleValue->int64);
				}
				return static_cast<T>(this->simpleValue->uint64);
				break;
			case simba_type_float:
				return static_cast<T>(this->simpleValue->floatVal);
				break;
			case simba_type_double:
				return static_cast<T>(this->simpleValue->doubleVal);
				break;
			}

			throw std::exception("Unknown conversion to T");
		}

#pragma endregion getters

		inline details::simba_serializer serialize() const;
		inline details::simba_deserializer deserialize();

	public: // operators
#pragma region
		simba_value& operator=(simba_value&& other)
		{
			this->simbaType = other.simbaType;
			this->simbaTypeFlag = other.simbaTypeFlag;
			this->simpleValue = other.simpleValue;
			this->objectValue = other.objectValue;
			this->arrayValue = other.arrayValue;
			this->string = other.string;
			this->u16string = other.u16string;
			this->u32string = other.u32string;
			this->wstring = other.wstring;

			// Ensure other wont delete any ptrs
			other.abandon();

			return *this;
		}

		simba_value& operator=(const simba_value& other)
		{
			this->destroyPtr(); // Destroy current obj

			this->simbaType = other.simbaType;
			this->simbaTypeFlag = other.simbaTypeFlag;

			switch (this->simbaType) {
			case simba_type_null:
				break;
			case simba_type_int8:
				this->simpleValue = new simba_value::simba_simple_type();
				if (this->simbaTypeFlag == simba_type_flag_signed) {
					this->simpleValue->int8 = other.simpleValue->int8;
				}
				else {
					this->simpleValue->uint8 = other.simpleValue->uint8;
				}
				break;
			case simba_type_int16:
				this->simpleValue = new simba_value::simba_simple_type();
				if (this->simbaTypeFlag == simba_type_flag_signed) {
					this->simpleValue->int16 = other.simpleValue->int16;
				}
				else {
					this->simpleValue->uint16 = other.simpleValue->uint16;
				}
				break;
			case simba_type_int32:
				this->simpleValue = new simba_value::simba_simple_type();
				if (this->simbaTypeFlag == simba_type_flag_signed) {
					this->simpleValue->int32 = other.simpleValue->int32;
				}
				else {
					this->simpleValue->uint32 = other.simpleValue->uint32;
				}
				break;
			case simba_type_int64:
				this->simpleValue = new simba_value::simba_simple_type();
				if (this->simbaTypeFlag == simba_type_flag_signed) {
					this->simpleValue->int64 = other.simpleValue->int64;
				}
				else {
					this->simpleValue->uint64 = other.simpleValue->uint64;
				}
				break;
			case simba_type_float:
				this->simpleValue = new simba_value::simba_simple_type();
				this->simpleValue->floatVal = other.simpleValue->floatVal;
				break;
			case simba_type_double:
				this->simpleValue = new simba_value::simba_simple_type();
				this->simpleValue->doubleVal = other.simpleValue->doubleVal;
				break;
			case simba_type_object:
				//case simba_type_map:
				this->objectValue = new simba_value::simba_object_type(*other.objectValue);
				//this->objectValue->insert(other.objectValue->cbegin(), other.objectValue->cend());

				//std::copy(other.objectValue->begin(), other.objectValue->end(), this->objectValue->begin());

				/*
				for (auto it = other.objectValue->cbegin(), end = other.objectValue->cend();
					it != end;
					++it) {
					this->objectValue->emplace(it->first, it->second);
				}*/
				break;
			case simba_type_array:
				this->arrayValue = new simba_value::simba_array_type(*other.arrayValue);
				/*
				for (auto it = other.arrayValue->cbegin(), end = other.arrayValue->cend();
					it != end;
					++it) {
					this->arrayValue->emplace_back(*it);
				}*/
				break;
			case simba_type_string8:
				this->string = new std::string(*other.string);
				break;
			case simba_type_string16:
				this->u16string = new std::u16string(*other.u16string);
				break;
			case simba_type_string32:
				this->u32string = new std::u32string(*other.u32string);
				break;
			case simba_type_string_w:
				this->wstring = new std::wstring(*other.wstring);
				break;
			}

			return *this;
		}

		simba_value& operator=(const std::nullptr_t& nullValue)
		{
			this->destroyPtr();
			return *this;
		}

		simba_value& operator=(const std::int8_t& int8)
		{
			this->simpleAlloc();

			this->simbaType = simba_type_int8;
			this->simbaTypeFlag = simba_type_flag_signed;
			this->simpleValue->int8 = int8;
			return *this;
		}

		simba_value& operator=(const std::uint8_t& uint8)
		{
			this->simpleAlloc();

			this->simbaType = simba_type_int8;
			this->simbaTypeFlag = simba_type_flag_unsigned;
			this->simpleValue->uint8 = uint8;
			return *this;
		}

		simba_value& operator=(const std::int16_t& int16)
		{
			this->simpleAlloc();

			this->simbaType = simba_type_int16;
			this->simbaTypeFlag = simba_type_flag_signed;
			this->simpleValue->int16 = int16;
			return *this;
		}

		simba_value& operator=(const std::uint16_t& uint16)
		{
			this->simpleAlloc();

			this->simbaType = simba_type_int16;
			this->simbaTypeFlag = simba_type_flag_unsigned;
			this->simpleValue->uint16 = uint16;
			return *this;
		}

		simba_value& operator=(const std::int32_t& int32)
		{
			this->simpleAlloc();

			this->simbaType = simba_type_int32;
			this->simbaTypeFlag = simba_type_flag_signed;
			this->simpleValue->int32 = int32;
			return *this;
		}

		simba_value& operator=(const std::uint32_t& uint32)
		{
			this->simpleAlloc();

			this->simbaType = simba_type_int32;
			this->simbaTypeFlag = simba_type_flag_unsigned;
			this->simpleValue->uint32 = uint32;
			return *this;
		}

		simba_value& operator=(const std::int64_t& int64)
		{
			this->simpleAlloc();

			this->simbaType = simba_type_int64;
			this->simbaTypeFlag = simba_type_flag_signed;
			this->simpleValue->int64 = int64;
			return *this;
		}

		simba_value& operator=(const std::uint64_t& uint64)
		{
			this->simpleAlloc();

			this->simbaType = simba_type_int64;
			this->simbaTypeFlag = simba_type_flag_unsigned;
			this->simpleValue->uint64 = uint64;
			return *this;
		}

		simba_value& operator=(const float& floatVal)
		{
			this->simpleAlloc();

			this->simbaType = simba_type_float;
			this->simpleValue->floatVal = floatVal;
			return *this;
		}

		simba_value& operator=(const double& doubleVal)
		{
			this->simpleAlloc();

			this->simbaType = simba_type_double;
			this->simpleValue->doubleVal = doubleVal;
			return *this;
		}

		simba_value& operator=(const simba_object_type& object)
		{
			this->destroyPtr();

			this->simbaType = simba_type_object;
			this->objectValue = new simba_object_type(object);
			return *this;
		}

		simba_value& operator=(const simba_array_type& array)
		{
			this->destroyPtr();

			this->simbaType = simba_type_array;
			this->arrayValue = new simba_array_type(array);
			return *this;
		}

		simba_value& operator=(const std::string& string)
		{
			if (this->simbaType == simba_type_string8) {
				*this->string = string;
			}
			else {
				this->destroyPtr();
				this->string = new std::string(string);
				this->simbaType = simba_type_string8;
			}

			return *this;
		}

		simba_value& operator=(const std::u16string& u16string)
		{
			if (this->simbaType == simba_type_string16) {
				*this->u16string = u16string;
			}
			else {
				this->destroyPtr();
				this->u16string = new std::u16string(u16string);
				this->simbaType = simba_type_string16;
			}

			return *this;
		}

		simba_value& operator=(const std::u32string& u32string)
		{
			if (this->simbaType == simba_type_string32) {
				*this->u32string = u32string;
			}
			else {
				this->destroyPtr();
				this->u32string = new std::u32string(u32string);
				this->simbaType = simba_type_string32;
			}

			return *this;
		}

		simba_value& operator=(const std::wstring& wstring)
		{
			if (this->simbaType == simba_type_string_w) {
				*this->wstring = wstring;
			}
			else {
				this->destroyPtr();
				this->wstring = new std::wstring(wstring);
				this->simbaType = simba_type_string_w;
			}

			return *this;
		}

		simba_value& operator[](const std::uint32_t& index)
		{
			if (this->simbaType != simba_type_array) {
				throw std::exception("Cannot retrieve with integer index from non array type");
			}

			return this->arrayValue->at(index);
		}

		const simba_value& operator[](const std::uint32_t& index) const
		{
			if (this->simbaType != simba_type_array) {
				throw std::exception("Cannot retrieve with integer index from non array type");
			}

			return this->arrayValue->at(index);
		}

		simba_value& operator[](const std::string& index)
		{
			if (this->simbaType != simba_type_object) {
				throw std::exception("Cannot retrieve string index from non object type");
			}

			auto it = this->objectValue->find(index);

			if (it != this->objectValue->end()) {
				return it->second;
			}

			(*this->objectValue)[index] = simba::val(nullptr);

			return (*this->objectValue)[index];
		}

		const simba_value& operator[](const std::string& index) const
		{
			if (this->simbaType != simba_type_object) {
				throw std::exception("Cannot retrieve string index from non object type");
			}

			auto it = this->objectValue->find(index);

			if (it != this->objectValue->end()) {
				return it->second;
			}

			throw std::exception("Cannot create item with operator[] when in const scope");
		}

		bool operator==(const simba_value& other) const
		{
			// Basic type check
			if (other.simbaType != this->simbaType) {
				return false;
			}

			// signed/unsigned check
			if (simba::details::hasTypeFlag(this->simbaType) && other.simbaTypeFlag != this->simbaTypeFlag) {
				return false;
			}

			switch (this->simbaType) {
			case simba_type_null:
				return true; // both are null
				break;

			case simba_type_int8:
				if (this->isSigned()) {
					return this->simpleValue->int8 == other.simpleValue->int8;
				}
				return this->simpleValue->uint8 == other.simpleValue->uint8;
				break;
			case simba_type_int16:
				if (this->isSigned()) {
					return this->simpleValue->int16 == other.simpleValue->int16;
				}
				return this->simpleValue->uint16 == other.simpleValue->uint16;
				break;
			case simba_type_int32:
				if (this->isSigned()) {
					return this->simpleValue->int32 == other.simpleValue->int32;
				}
				return this->simpleValue->uint32 == other.simpleValue->uint32;
				break;
			case simba_type_int64:
				if (this->isSigned()) {
					return this->simpleValue->int64 == other.simpleValue->int64;
				}
				return this->simpleValue->uint64 == other.simpleValue->uint64;
				break;
			case simba_type_float:
				return this->simpleValue->floatVal == other.simpleValue->floatVal;
				break;
			case simba_type_double:
				return this->simpleValue->doubleVal = other.simpleValue->doubleVal;
				break;
			case simba_type_array:
				if (this->arrayValue->size() != other.arrayValue->size()) {
					return false;
				}

				{ // compare all elements of the array
					auto it1 = this->arrayValue->cbegin(),
						end1 = this->arrayValue->cend(),
						it2 = other.arrayValue->cbegin(),
						end2 = other.arrayValue->cend();

					for (; it1 != end1 && it2 != end2; ++it1, ++it2) {
						if (*it1 != *it2) {
							return false;
						}
					}

					return true;
				}
				break;
			case simba_type_object:
				if (this->objectValue->size() != other.objectValue->size()) {
					return false;
				}

				{ // recursively check all key/value pairs
					for (auto it = this->objectValue->cbegin(), end = this->objectValue->cend();
						it != end; ++it) {
						auto found = other.objectValue->find(it->first);

						if (found == other.objectValue->end()) {
							return false;
						}

						if (it->second != found->second) {
							return false;
						}
					}

					return true;
				}
				break;
			case simba_type_string8:
				return *this->string == *other.string;
				break;
			case simba_type_string16:
				return *this->u16string == *other.u16string;
				break;
			case simba_type_string32:
				return *this->u32string == *other.u32string;
				break;
			case simba_type_string_w:
				return *this->wstring == *other.wstring;
				break;
			}


			return false; // unknown types??
		}

		bool operator!=(const simba_value& other) const
		{
			return !(*this == other);
		}
#pragma endregion operators

	private: // general private functions

		// allocate a simple value (but an allocation is only actually done if the current type is not a simple value)
		void simpleAlloc()
		{
			if (this->simpleValue == nullptr) {
				// not a simple value...
				this->destroyPtr();
				this->simpleValue = new simba_value::simba_simple_type;
			}
		}

		void destroyPtr()
		{
			if (this->simpleValue != nullptr) {
				delete this->simpleValue;
				this->simpleValue = nullptr;
			}

			if (this->objectValue != nullptr) {
				delete this->objectValue;
				this->objectValue = nullptr;
			}

			if (this->arrayValue != nullptr) {
				delete this->arrayValue;
				this->arrayValue = nullptr;
			}

			if (this->string != nullptr) {
				delete this->string;
				this->string = nullptr;
			}

			if (this->wstring != nullptr) {
				delete this->wstring;
				this->wstring = nullptr;
			}

			if (this->u16string != nullptr) {
				delete this->u16string;
				this->u16string = nullptr;
			}

			if (this->u32string != nullptr) {
				delete this->u32string;
				this->u32string = nullptr;
			}

			this->simbaType = simba_type_null;
		}

	private:
		std::uint8_t simbaType = simba_type_null,
			simbaTypeFlag = simba_type_flag_signed;
		simba_simple_type* simpleValue = nullptr;
		simba_object_type* objectValue = nullptr;
		simba_array_type* arrayValue = nullptr;
		std::string* string = nullptr;
		std::wstring* wstring = nullptr;
		std::u16string* u16string = nullptr;
		std::u32string* u32string = nullptr;
	};

	constexpr auto SIMBA_SIZE = sizeof(simba_value);
}

std::uint8_t simba::details::swap_uint8(std::uint8_t val)
{
	return static_cast<std::uint8_t>(swap_uint16(val));
}

//! Byte swap short
std::int8_t simba::details::swap_int8(std::int8_t val)
{
	return static_cast<std::int8_t>(swap_int16(val));
}

std::uint16_t simba::details::swap_uint16(std::uint16_t val)
{
	return (val << 8) | (val >> 8);
}

//! Byte swap short
std::int16_t simba::details::swap_int16(std::int16_t val)
{
	return (val << 8) | ((val >> 8) & 0xFF);
}

//! Byte swap unsigned int
std::uint32_t simba::details::swap_uint32(std::uint32_t val)
{
	val = ((val << 8) & 0xFF00FF00) | ((val >> 8) & 0xFF00FF);
	return (val << 16) | (val >> 16);
}

//! Byte swap int
std::int32_t simba::details::swap_int32(std::int32_t val)
{
	val = ((val << 8) & 0xFF00FF00) | ((val >> 8) & 0xFF00FF);
	return (val << 16) | ((val >> 16) & 0xFFFF);
}

std::int64_t simba::details::swap_int64(std::int64_t val)
{
	val = ((val << 8) & 0xFF00FF00FF00FF00ULL) | ((val >> 8) & 0x00FF00FF00FF00FFULL);
	val = ((val << 16) & 0xFFFF0000FFFF0000ULL) | ((val >> 16) & 0x0000FFFF0000FFFFULL);
	return (val << 32) | ((val >> 32) & 0xFFFFFFFFULL);
}

std::uint64_t simba::details::swap_uint64(std::uint64_t val)
{
	val = ((val << 8) & 0xFF00FF00FF00FF00ULL) | ((val >> 8) & 0x00FF00FF00FF00FFULL);
	val = ((val << 16) & 0xFFFF0000FFFF0000ULL) | ((val >> 16) & 0x0000FFFF0000FFFFULL);
	return (val << 32) | (val >> 32);
}

std::uint8_t simba::details::getEndianess()
{
	short int word = 0x0001;
	return reinterpret_cast<std::int8_t*>(&word)[0] ? little_endian : big_endian;
}

bool simba::details::hasTypeFlag(const std::uint8_t & type)
{
	return type > simba_type_null&& type <= simba_type_int64;
}

template<typename T>
std::pair<std::string, simba::simba_value> simba::pair(std::string str, T value)
{
	return { std::move(str), simba::simba_value(std::move(value)) };
}

template<typename T>
simba::simba_value simba::val(T && value)
{
	return simba_value(std::forward<T>(value));
}

simba::simba_value simba::val()
{
	return { nullptr };
}

template<typename ...Args>
simba::simba_value simba::array(Args && ...args)
{
	simba::simba_value arr = simba_value::simba_array_type{};
	simba::details::array_impl<Args...>::append(arr, std::forward<Args>(args)...);
	return arr;
}

template<typename T, typename ...Args>
struct simba::details::array_impl<T, Args...>
{
	static void append(simba_value& arr, T&& t, Args&& ...args)
	{
		arr.getArray().emplace_back(simba::val(std::forward<T>(t)));
		array_impl<Args...>::append(arr, std::forward<Args>(args)...);
	}
};

template<typename T>
struct simba::details::array_impl<T>
{
	static void append(simba_value& arr, T&& t)
	{
		arr.getArray().emplace_back(simba::val(std::forward<T>(t)));
	}
};

template<>
struct simba::details::array_impl<>
{
	static void append(simba_value& arr)
	{}
};


template<typename ...Args>
simba::simba_value simba::object(Args && ...args)
{
	simba::simba_value obj = simba_value::simba_object_type{};
	simba::details::object_impl<Args...>::append(obj, std::forward<Args>(args)...);
	return obj;
}


template<typename T1, typename T2, typename...Args>
struct simba::details::object_impl<std::pair<T1, T2>, Args...>
{
	static void append(simba_value& obj, std::pair<T1, T2> pair, Args&& ...args)
	{
		obj.getObject()[pair.first] = simba::val(pair.second);
		object_impl<Args...>::append(obj, std::forward<Args>(args)...);
	}
};

template<typename T1, typename T2>
struct simba::details::object_impl<std::pair<T1, T2>>
{
	static void append(simba_value& obj, std::pair<T1, T2> pair)
	{
		obj.getObject()[pair.first] = simba::val(pair.second);
	}
};

template<>
struct simba::details::object_impl<>
{
	static void append(simba_value& obj)
	{}
};

class simba::details::simba_input_adapter
{
public:
	// return the size of the input buffer
	virtual std::streamsize size() const = 0;

	// return the cursor position
	virtual std::streamsize cur() const = 0;

	// read length amount of bytes into buffer
	virtual std::streamsize read(char* buffer, std::streamsize length) = 0;
};

class simba::details::simba_string_input_adapter : public simba::details::simba_input_adapter
{
public:
	simba_string_input_adapter(std::string str)
		: str(std::move(str))
	{}

	std::streamsize size() const
	{
		return static_cast<std::streamsize>(this->str.length());
	}

	std::streamsize cur() const
	{
		return this->cursor;
	}

	std::streamsize read(char* buffer, std::streamsize length)
	{
		const auto beg = this->cursor;
		auto end = this->cursor + length;

		if (end >= this->str.length()) {
			end = this->str.length() - 1;
		}

		const char* data = this->str.data();
		while (length--) {
			buffer[this->cursor - beg] = data[this->cursor];
			++this->cursor;
		}

		return end - beg;
	}

private:
	std::string str;
	std::streamsize cursor = 0u;
};

class simba::details::simba_stream_input_adapter : public simba::details::simba_input_adapter
{
public:
	simba_stream_input_adapter(std::basic_istream<char>& file)
		: inputFile(&file)
	{
		this->inputFile->ignore(std::numeric_limits<std::streamsize>::max());
		this->inputFile->seekg(0, std::ios::end);
		this->fileLength = this->inputFile->gcount();
		this->inputFile->clear();
		this->inputFile->seekg(0, std::ios::beg);
	}

	std::streamsize size() const
	{
		return this->fileLength;
	}

	std::streamsize cur() const
	{
		return this->inputFile->tellg();
	}

	std::streamsize read(char* buffer, std::streamsize length)
	{
		this->inputFile->read(buffer, length);
		return this->inputFile->gcount();
	}

private:
	std::basic_istream<char>* inputFile = nullptr;
	std::streamsize fileLength = 0u;
};

class simba::details::simba_output_adapter
{
public:
	virtual std::streamsize write(const char* buffer, std::streamsize len) = 0;
};

class simba::details::simba_stream_output_adapter : public simba::details::simba_output_adapter
{
public:
	simba_stream_output_adapter(std::basic_ostream<char>& file)
		: file(&file)
	{}

	std::streamsize write(const char* buffer, std::streamsize len)
	{
		this->file->write(buffer, len);
		return len;
	}

private:
	std::basic_ostream<char>* file;
};

class simba::details::simba_serializer
{
public:
	using adapter_t = simba::details::simba_output_adapter;

public:
	simba_serializer(const simba_value* value)
		: value(value)
	{}

	void to(adapter_t& stream)
	{
		this->writeHeader(stream);
		this->writeElement(stream, this->value);
	}

	void to(const std::string& filename)
	{
		std::ofstream fileStream{ filename, std::ios::binary };
		simba::details::simba_stream_output_adapter adapter{ fileStream };
		this->to(adapter);
	}

private:
	void writeHeader(adapter_t& stream)
	{
		stream.write(simba::SIMBA_HEADER, simba::SIMBA_HEADER_LEN);

		auto endianess = simba::details::getEndianess();
		stream.write(reinterpret_cast<const char*>(&endianess), sizeof(endianess));
	}

	void writeElement(adapter_t& stream, const simba_value* value)
	{
		this->writeElementType(stream, value->getType(), value->getTypeFlag());

		auto size = [&stream](const std::uint32_t & size) {
			static_assert(sizeof(std::uint32_t) == 4, "Invalid integer size type");
			stream.write(reinterpret_cast<const char*>(&size), sizeof(std::uint32_t)); // always write the size as a 4 byte unsigned integer.
		};

		switch (value->getType()) {
		case simba_type_null:
			// dont write anything
			break;
		case simba_type_int8:
			if (value->isSigned()) {
				size(sizeof(std::int8_t));
				stream.write(reinterpret_cast<const char*> (&value->get<std::int8_t>()), sizeof(std::int8_t));
			}
			else {
				size(sizeof(std::uint8_t));
				stream.write(reinterpret_cast<const char*> (&value->get<std::uint8_t>()), sizeof(std::uint8_t));
			}
			break;
		case simba_type_int16:
			if (value->isSigned()) {
				size(sizeof(std::int16_t));
				stream.write(reinterpret_cast<const char*>(&value->get<std::int16_t>()), sizeof(std::int16_t));
			}
			else {
				size(sizeof(std::uint16_t));
				stream.write(reinterpret_cast<const char*> (&value->get<std::uint16_t>()), sizeof(std::uint16_t));
			}
			break;
		case simba_type_int32:
			if (value->isSigned()) {
				size(sizeof(std::int32_t));
				stream.write(reinterpret_cast<const char*> (&value->get<std::int32_t>()), sizeof(std::int32_t));
			}
			else {
				size(sizeof(std::uint32_t));
				stream.write(reinterpret_cast<const char*> (&value->get<std::uint32_t>()), sizeof(std::uint32_t));
			}
			break;
		case simba_type_int64:
			if (value->isSigned()) {
				size(sizeof(std::int64_t));
				stream.write(reinterpret_cast<const char*> (&value->get<std::int64_t>()), sizeof(std::int64_t));
			}
			else {
				size(sizeof(std::uint64_t));
				stream.write(reinterpret_cast<const char*> (&value->get<std::uint64_t>()), sizeof(std::uint64_t));
			}
			break;
		case simba_type_float:
			size(sizeof(float));
			stream.write(reinterpret_cast<const char*> (&value->get<float>()), sizeof(float));
			break;
		case simba_type_double:
			size(sizeof(double));
			stream.write(reinterpret_cast<const char*> (&value->get<double>()), sizeof(double));
			break;
		case simba_type_array:
			size(value->size());
			for (auto& el : value->getArray()) {
				this->writeElement(stream, &el);
			}
			break;
		case simba_type_object:
			size(value->size());
			for (auto& el : value->getObject()) {
				auto strEl = simba::val(el.first);
				this->writeElement(stream, &strEl);
				this->writeElement(stream, &el.second);
			}
			break;
		case simba_type_string8:
			size(sizeof(char));
			size(value->length());
			stream.write(reinterpret_cast<const char*>(value->get<std::string>().c_str()), value->length());
			break;
		case simba_type_string16:
			size(sizeof(std::u16string::value_type));
			size(value->length());
			stream.write(reinterpret_cast<const char*>(value->get<std::u16string>().c_str()), value->length() * sizeof(std::u16string::value_type));
			break;
		case simba_type_string32:
			size(sizeof(std::u32string::value_type));
			size(value->length());
			stream.write(reinterpret_cast<const char*>(value->get<std::u32string>().c_str()), value->length() * sizeof(std::u32string::value_type));
			break;
		case simba_type_string_w:
			size(sizeof(std::wstring::value_type));
			size(value->length());
			stream.write(reinterpret_cast<const char*>(value->get<std::wstring>().c_str()), value->length() * sizeof(std::wstring::value_type));
			break;
		}
	}

	void writeElementType(adapter_t& stream, const std::uint8_t& type, const std::uint8_t& typeFlag)
	{
		// Write type
		stream.write(reinterpret_cast<const char*>(&type), 1);

		if (simba::details::hasTypeFlag(type)) {
			stream.write(reinterpret_cast<const char*>(&typeFlag), 1);
		}
	}

private:
	const simba_value* value = nullptr;
};

class simba::details::simba_deserializer
{
public:
	using adapter_t = simba::details::simba_input_adapter;

public:
	simba_deserializer(simba_value* value)
		: value(value)
	{}

	void from(adapter_t& adapter)
	{
		this->readHeader(adapter);
		this->readElement(adapter, this->value);
	}

	void from(const std::string& filename)
	{
		std::ifstream file{ filename, std::ios::binary };
		simba::details::simba_stream_input_adapter adapter{ file };
		this->from(adapter);
	}

	void fromString(const std::string& input)
	{
		simba::details::simba_string_input_adapter adapter{ input };
		this->from(adapter);
	}

private:
	void readHeader(adapter_t& adapter)
	{
		char header[5];

		adapter.read(header, 5);

		if (memcmp(header, simba::SIMBA_HEADER, simba::SIMBA_HEADER_LEN)) {
			throw std::exception("Not a valid simba header!");
		}

		std::uint8_t endianess{ 0u };

		adapter.read(reinterpret_cast<char*>(&endianess), 1);

		if (endianess != simba::details::getEndianess()) {
			this->needSwapEndianess = true;
		}
	}

	std::pair<std::uint8_t, std::uint8_t> readElementType(adapter_t& adapter)
	{
		std::pair<std::uint8_t, std::uint8_t> result{ 0u, 0u };
		adapter.read(reinterpret_cast<char*>(&result.first), 1);

		if (this->needSwapEndianess) {
			result.first = simba::details::swap_uint8(result.first);
		}

		if (simba::details::hasTypeFlag(result.first)) {
			adapter.read(reinterpret_cast<char*>(&result.second), 1);

			if (this->needSwapEndianess) {
				result.second = simba::details::swap_uint8(result.first);
			}
		}

		return result;
	}

	void readElement(adapter_t& adapter, simba_value* value)
	{
		auto typeInfo = this->readElementType(adapter);

		switch (typeInfo.first) {
		case simba_type_null:
			*value = nullptr;
			break;
		case simba_type_int8:
			if (typeInfo.second == simba_type_flag_signed) {
				*value = this->readNextValue<std::int8_t>(adapter);

				if (this->needSwapEndianess) {
					*value = simba::details::swap_int8(value->get<std::int8_t>());
				}
			}
			else {
				*value = this->readNextValue<std::uint8_t>(adapter);

				if (this->needSwapEndianess) {
					*value = simba::details::swap_uint8(value->get<std::uint8_t>());
				}
			}
			break;
		case simba_type_int16:
			if (typeInfo.second == simba_type_flag_signed) {
				*value = this->readNextValue<std::int16_t>(adapter);

				if (this->needSwapEndianess) {
					*value = simba::details::swap_int16(value->get<std::int16_t>());
				}
			}
			else {
				*value = this->readNextValue<std::uint16_t>(adapter);

				if (this->needSwapEndianess) {
					*value = simba::details::swap_uint16(value->get<std::uint16_t>());
				}
			}
			break;
		case simba_type_int32:
			if (typeInfo.second == simba_type_flag_signed) {
				*value = this->readNextValue<std::int32_t>(adapter);

				if (this->needSwapEndianess) {
					*value = simba::details::swap_int32(value->get<std::int32_t>());
				}
			}
			else {
				*value = this->readNextValue<std::uint32_t>(adapter);

				if (this->needSwapEndianess) {
					*value = simba::details::swap_uint32(value->get<std::uint32_t>());
				}
			}
			break;
		case simba_type_int64:
			if (typeInfo.second == simba_type_flag_signed) {
				*value = this->readNextValue<std::int64_t>(adapter);


				if (this->needSwapEndianess) {
					*value = simba::details::swap_int64(value->get<std::int64_t>());
				}
			}
			else {
				*value = this->readNextValue<std::uint64_t>(adapter);

				if (this->needSwapEndianess) {
					*value = simba::details::swap_uint64(value->get<std::uint64_t>());
				}
			}
			break;
		case simba_type_float:
			*value = this->readNextValue<float>(adapter);
			break;
		case simba_type_double:
			*value = this->readNextValue<double>(adapter);
			break;
		case simba_type_object:
			*value = simba::object();
			this->readObject(adapter, value);
			break;
		case simba_type_array:
			*value = simba::array();
			this->readArray(adapter, value);
			break;
		case simba_type_string8:
			this->readString<char>(adapter, value);
			break;
		case simba_type_string16:
			this->readString<char16_t>(adapter, value);
			break;
		case simba_type_string32:
			this->readString<char32_t>(adapter, value);
			break;
		case simba_type_string_w:
			this->readString<wchar_t>(adapter, value);
			break;

		default:
			throw std::exception("Unknown simba_value type read, corrupted file?");
			break;
		}
	}

	void readObject(adapter_t& adapter, simba_value* value)
	{
		auto objSize = this->getSize(adapter);

		for (auto i = 0u; i < objSize; ++i) {
			simba_value index{ nullptr },
				element{ nullptr };

			this->readElement(adapter, &index);
			this->readElement(adapter, &element);

			(*value)[index.get<std::string>()] = std::move(element);
		}
	}

	void readArray(adapter_t& adapter, simba_value* value)
	{
		auto arrSize = this->getSize(adapter);
		auto& arr = value->getArray();
		arr.resize(arrSize);

		for (auto i = 0u; i < arrSize; ++i) {
			simba_value element{ nullptr };

			this->readElement(adapter, &element);

			arr[i] = std::move(element);
		}
	}

	template<typename CharType = char>
	void readString(adapter_t& adapter, simba_value* value)
	{
		std::basic_string<CharType> str;
		auto strCharSize = this->getSize(adapter);
		auto strLen = this->getSize(adapter);

		str.resize(strLen);

		adapter.read(reinterpret_cast<char*>(str.data()), strCharSize * strLen);

		// Assign str
		*value = std::move(str);
	}

	std::uint32_t getSize(adapter_t& adapter)
	{
		static_assert(sizeof(std::uint32_t) == 4, "Invalid integer size type");
		std::uint32_t sz{ 0u };
		adapter.read(reinterpret_cast<char*>(&sz), sizeof(std::uint32_t));

		if (this->needSwapEndianess) {
			sz = simba::details::swap_uint32(sz);
		}

		return sz;
	}

	template<typename T>
	T readNextValue(adapter_t& adapter)
	{
		T t{ 0 };

		auto size = this->getSize(adapter);

		if (size > sizeof(T)) {
			throw std::exception("Cannot read value into T because stored size is larger than size of T");
		}

		adapter.read(reinterpret_cast<char*>(&t), size);
		return t;
	}


private:
	simba_value* value;
	bool needSwapEndianess = false;
};

simba::details::simba_serializer simba::simba_value::serialize() const
{
	return { this };
}

simba::details::simba_deserializer simba::simba_value::deserialize()
{
	return { this };
}

static std::basic_ostream<char>& operator<<(std::basic_ostream<char> & output, const simba::simba_value & value)
{
	simba::details::simba_stream_output_adapter adapter{ output };
	value.serialize().to(adapter);
	return output;
}

static std::basic_istream<char>& operator>>(std::basic_istream<char>& input, simba::simba_value& value)
{
	simba::details::simba_stream_input_adapter adapter{ input };
	simba::details::simba_deserializer deserializer{ &value };
	deserializer.from(adapter);
	return input;
}
