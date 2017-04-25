#pragma once

namespace  cppe
{
    class string : public object
    {
    private:
        wchar_t* value;
    
        string(const size_t _lenght) : length(_lenght)
        {
            value = new wchar_t[length+1];
            value[length] = L'\0';
        }
    
    public:
        const size_t length;
    
        string() : length(0)
        {
            value = new wchar_t[1];
            value[0] = L'\0';
        }
    
        string(const string& copy) : length(copy.length)
        {
            value = new wchar_t[length+1];
        
            for( size_t i=0; i<=length; ++i )
            {
                value[i] = copy.value[i];
            }
        }
    
        string(const wchar_t* text) : length(0)
        {
            while( text[length] != L'\0' )
            {
                ++((size_t&)length);
            }
            value = new wchar_t[length+1];
        
            for( size_t i=0; i<=length; ++i )
            {
                value[i] = text[i];
            }
        }
        virtual ~string()
        {
            delete[] value;
        }
        
        virtual const string& toString() const
        {
            return *this;
        }
        
        inline operator const wchar_t*() const
        {
            return value;
        }
        
        inline const wchar_t& operator[]( size_t index ) const
        {
            return value[index];
        }
    
        inline string operator+( const string& right ) const
        {
            string result( length + right.length );
        
            size_t i=0;
            for( ; i<length; ++i )
            {
                result.value[i] = value[i];
            }
            for( ; i<length+right.length; ++i )
            {
                result.value[i] = right.value[i-length];
            }
            return result;
        }
    
        inline string& operator=( const string& right )
        {
            delete[] value;
            
            ((size_t&)length) = right.length;
            
            value = new wchar_t[length+1];
            
            for( size_t i=0; i<=length; ++i )
            {
                value[i] = right[i];
            }
            return *this;
        }

        
        inline bool operator==( const string& right ) const
        {
            if( length != right.length )
            {
                return false;
            }
            for( size_t i=0; i<length; ++i )
            {
                if( value[i] != right.value[i] )
                {
                    return false;
                }
            }
            return true;
        }
    };
}

cppe::string operator +( const cppe::object& left, const cppe::object& right )
{
    return left.toString() + right.toString();
}
