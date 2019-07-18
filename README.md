# Simba
**Sim**ple **B**inary stor**a**ge is a header only library that requires C++17 features (`if constexpr`) that I have made with
simplicity and safe storage in mind. The library will automatically convert the endianess of integers
and such if it is needed (for example if files are sent over the net etc). There are many other benefits
of using a direct binary storage instead of JSON, for example much much faster serialization and deserialization.
Obviously you shouldn't use this for configurable files, in that case JSON or YAML etc would probably be better,
this library is only for files that are exclusively machine written and read.

# Index
- [Basic usage](#basic-usage)
	- [Retrieving values](#retrieving-values)
	- [Serialization and deserialization](#serialization-and-deserialization)
	- [Creating an object](#creating-an-object)
- [License](#license)

# Basic usage
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
You can retrive the value using the simple `get` function, note that this function will **not** tolerate
any type differences, if the stored value is an `unsigned int` and you try to retrieve a signed `int` an exception
will be thrown. To retrieve a value that may be casted you must use the separate `cast` function, which **only**
works on arithmetic values.

Example string:
```cpp
value->get<std::string>()
```

Example object:
```cpp
value->get<simba::simba_value::simba_object_type>();
// for less verbose version you can use
value->getObject();
```

Similarly retrieving an array:
```cpp
value->get<simba::simba_value::simba_array_type>();
// less verbose:
value->getArray();
```

### Serialization and deserialization
You can easily serialize a simba value into a file like such:
```cpp
auto value = simba::val(200);
value.serialize().to("myfile.simba")
```
You can also serialize the value into any std output stream, e.g manually to file:
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

<img align="right" src="http://opensource.org/trademarks/opensource/OSI-Approved-License-100x137.png">

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