#include <core/wrDynamicArray.hpp>
#include <iostream>
#include <string>

class TestDel
{
public:
	TestDel() { puts("TestDel"); }
	~TestDel() { puts("~~~TestDel"); };
private:
};

int main()
{
	TestDel tdel[2];
	wr::dynamic_array<TestDel> ind;
	ind.form(tdel, 2);
	return 0;
}