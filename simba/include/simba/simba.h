#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include <map>

namespace simba {
	constexpr auto VERSION_STRING = "1.0.0";

	class simba_value;

	template<typename T>
	std::pair<std::string, simba_value> pair(std::string str, T value);

	template<typename T>
	simba_value val(T&& value);

	template<typename ...Args>
	simba_value array(Args&& ...args);

	template<typename ...Args>
	simba_value object(Args&& ...args);

	namespace details {
		std::uint16_t swap_uint16(std::uint16_t val);
		std::int16_t swap_int16(std::int16_t val);
		std::uint32_t swap_uint32(std::uint32_t val);
		std::int32_t swap_int32(std::int32_t val);
		std::int64_t swap_int64(std::int64_t val);
		std::uint64_t swap_uint64(std::uint64_t val);
		std::uint8_t getEndianess();

		template<typename ...Args>
		struct array_impl;

		template<typename ...Args>
		struct object_impl;
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
		simba_value(simba_value&& other)
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
			*this = std::move(arrayValue);
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

		std::uint32_t size()
		{
			if (this->simbaType == simba_type_object) {
				return this->objectValue->size();
			}
			else if (this->simbaType == simba_type_array) {
				return this->arrayValue->size();
			}

			return 0u;
		}

		std::uint32_t length()
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

		auto& getArray()
		{
			return this->get<simba_array_type>();
		}

		auto& getObject()
		{
			return this->get<simba_object_type>();
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

	public: // operators
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
				throw std::exception("Cannot retrieve integer index from non array type");
			}

			return this->arrayValue->at(index);
		}

		const simba_value& operator[](const std::uint32_t& index) const
		{
			if (this->simbaType != simba_type_array) {
				throw std::exception("Cannot retrieve integer index from non array type");
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