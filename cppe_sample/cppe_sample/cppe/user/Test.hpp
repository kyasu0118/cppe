#pragma once
class Test : public Base
{
	public: static double age;
	private: const int a;
	public: void hoge() 
	{
		cppe::console::print( cppe::STRING_0002 + * this , age ); 
	}
	public: Test() : a(769)
	{
	}
	public: const cppe::string& toString() const
	{
		  return cppe::STRING_0003;
	}
	public: cppe::string operator +(const cppe::string& right) const
	{
		  return toString() + right;
	}
	public: const std::type_info& type() const
	{
		  return typeid(Test);
	}
};
double Test::age = 20;
