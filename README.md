# *1. Sales point*   
1. You do not have to wait for C++ language extension.
2. C++ extenstion is sophisticated language specification.
3. Open source project.
 
# *2. Description*  
Convert C++ extension code to C++ code. 

Example  
```
// From C++ extension code. 
class Test
{
    private int abc = 20;

    public void hoge()
    {
        double array[] = { 1 ,2, 3 };

        array += 10.0;

        foreach( double d in array )
        {
            console::print( "%d %f\n", abc, d);
        }
    }
}
```
Output
```
20 1.000000
20 2.000000
20 3.000000
20 10.000000
```
```
// To C++ code.
#pragma once
class Test : public cppe::object
{
	private: int abc;
	public: void hoge() 
	{
		cppe::array< double > array(3);
		new ( &((cppe::array< double >&)array)[0] ) double(1);
		new ( &((cppe::array< double >&)array)[1] ) double(2);
		new ( &((cppe::array< double >&)array)[2] ) double(3);
		array += 10.0; 
		for( auto cppe_iterator1 = array.begin(); cppe_iterator1 != array.end(); ++cppe_iterator1 )
		{
			double d = *cppe_iterator1;
			cppe::console::print( cppe::STRING_0000 , abc , d ); 
		}
	}
	public: Test() : abc(20)
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
		  return typeid(Test);
	}
};

```

# *3. License*  
MIT License

Copyright (c) 2017 kyasu

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
