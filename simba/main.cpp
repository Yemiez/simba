#include "include/simba/simba.h"
#include <iostream>

int main()
{
	auto myArr = simba::array(
		24, 
		5676, 
		"hello", 
		"world", 
		simba::array(
			1, 
			23l, 
			4566u, 
			static_cast<std::uint16_t>(423), 
			static_cast<std::int64_t>(43563463477)
		),
		simba::object(
			simba::pair("large-number", static_cast<std::int64_t>(4356346347456778887)),
			simba::pair("null-value", nullptr),
			simba::pair("single char", 'h')
		),
		"hey :)"
	);

	{
		myArr.serialize().to("output.simba");
	}

	auto myArr2 = simba::val();

	{
		myArr2.deserialize().from("output.simba");
	}

	if (myArr == myArr2) {
		std::cout << "The two values are the same" << std::endl;
	}
	else {
		std::cout << "The two values are not the same" << std::endl;
	}
	std::cin.get();

	return 0;
}