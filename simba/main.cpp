#include "include/simba/simba.h"
#include <iostream>

int main()
{
	auto myArr = simba::array(24, 5676, "hello", "world");
	auto obj = simba::object(
		simba::pair("hello", 5),
		simba::pair("item 2", "some string")
	);

	auto val_5676 = myArr[1].get<int>();
	auto asUint = myArr[0].cast<std::uint32_t>();

	auto str = obj["item 2"].get<std::string>();
	auto str2 = obj["hello"].get<int>();
	obj["test"] = 345;

	return 0;
}