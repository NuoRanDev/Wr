#include <core/wrDynamicArray.hpp>
#include <iostream>
#include <string>

void print_daar(wr::dynamic_array<std::string>& indd)
{
	for (size_t i = 0; i < indd.num(); i++)
	{
		std::cout << i << ":\t" << indd.at(i) << "\n";
	}
}

int main()
{
	std::string b[] = { "1olp","2ddd","3dd","43fef","5gogo"};
	wr::dynamic_array<std::string> ind;
	ind.push_back("1cc");
	ind.push_back(b, 5);
	ind.pop_back();
	ind.capacity(4);
	print_daar(ind);
	return 0;
}