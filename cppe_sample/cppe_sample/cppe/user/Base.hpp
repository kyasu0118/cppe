#pragma once
class Base : public cppe::object
{
	public: void aaa( int a ) 
	{
		cppe::console::print( cppe::STRING_0000 , a ); 
	}
	public: Base()
	{
	}
	public: const cppe::string& toString() const
	{
		  return cppe::STRING_0001;
	}
	public: cppe::string operator +(const cppe::string& right) const
	{
		  return toString() + right;
	}
	public: const std::type_info& type() const
	{
		  return typeid(Base);
	}
};
