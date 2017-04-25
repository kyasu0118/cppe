#pragma once
class Test : public Base
{
	public: static double age;
	private: const int a;
	public: void hoge() 
	{
		switch( a ) 
		{
			case 10: 
			cppe::console::print( cppe::STRING_0002 ); 
			break; 
			case 20: 
			break; 
		}
	}
	public: Test() : a(10)
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
