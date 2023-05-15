# Simba: Simple Binary Storage

Simba is a header-only C++ library designed with simplicity and safe storage in mind. It provides a direct binary storage solution, automatically handling endianess conversion when necessary (e.g., when dealing with network communication). The library offers several benefits over JSON, such as significantly faster serialization and deserialization. It is specifically intended for files that are exclusively machine-written and read, not for configurable files where JSON or YAML would be more appropriate.

## Table of Contents

- [Basic Usage](#basic-usage)
  - [Retrieving Values](#retrieving-values)
  - [Serialization and Deserialization](#serialization-and-deserialization)
  - [Creating an Object](#creating-an-object)
- [License](#license)

## Basic Usage

Here is an example usage creating an array containing many different values including another array and an object.

```cpp
#include <simba/simba.h>
#include <iostream> // cout/cin

int main()
{
	// Initialize the main array
	auto myArr = simba::array(
		24,
		5676,
		"hello",
		"world",
		simba::array( // Another array
			1,
			23l,
			4566u,
			static_cast<std::uint16_t>(423),
			static_cast<std::int64_t>(43563463477)
		),
		simba::object( // and an object
			simba::pair("large-number", static_cast<std::int64_t>(4356346347456778887)),
			simba::pair("null-value", nullptr),
			simba::pair("single char", 'h')
		),
		"hey :)"
	);

	myArr.serialize().to("output.simba");

	// Deserialize the file "output.simba" into myArr2
	auto myArr2 = simba::val();
	myArr2.deserialize().from("output.simba");

	// Compare the two values.
	// the comparison is recursive if it contains arrays/objects
	// so be careful using the comparsions if you have very large objects,
	// you should instead manually check the size/length if needed.
	if (myArr == myArr2) {
		std::cout << "The two values are the same" << std::endl;
	}
	else {
		std::cout << "The two values are not the same" << std::endl;
	}
	std::cin.get();

	return 0;
}
```

### Retrieving values

You can retrieve the value using the simple get function. Note that this function will not tolerate any type differences. If the stored value is an unsigned int and you try to retrieve a signed int, an exception will be thrown. To retrieve a value that may be casted, you must use the separate cast function, which only works on arithmetic values.

Example for retrieving a string:

```cpp
value->get<std::string>()
```

Example for retrieving an object:

```cpp
value->get<simba::simba_value::simba_object_type>();
// or you can use the less verbose version:
value->getObject();
```

Similarly, to retrieve an array:

```cpp
value->get<simba::simba_value::simba_array_type>();
// less verbose:
value->getArray();
```

### Serialization and deserialization

You can easily serialize a Simba value into a file like such:

```cpp
auto value = simba::val(200);
value.serialize().to("myfile.simba")
```

You can also serialize the value into any std output stream, e.g., manually to a file:

```cpp
auto value = simba::val(200);
std::ofstream myFile{"myfile.simba", std::ios::binary};
myFile << value;
```

And deserialization is pretty much the same:

```cpp
auto value = simba::val();
value.deserialize().from("myfile.simba");
```

Deserialization also works with any std input stream:

```cpp
auto value = simba::val();
std::ifstream myFile{"myfile.simba", std::ios::binary};
myFile >> value;
```

### Creating an object

```cpp
auto myObject = simba::object(
	simba::pair("my-key", "my-value"),
	simba::pair("my-number", 1234),
	simba::pair("my-array", simba::array(12, 4576, 78))
);

// You can now access the keys directly like this:
auto strValue = myObject["my-key"];

std::cout << strValue->get<std::string>();
```

## License

BSD-3: See [LICENSE](./LICENSE)
