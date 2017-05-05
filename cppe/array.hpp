#pragma once

namespace  cppe
{
    static const string ARRAY_TO_STRING = L"array";

    template < class T >
    class array : public object
    {
    private:
        T* value;
    
        array(const size_t _lenght) : length(_lenght)
        {
            value = new T[length];
        }
    
    public:
        const size_t length;
    
        array() : length(0)
        {
            value = nullptr;
        }
    
        array(const array& copy) : length(copy.length)
        {
            value = new T[length];
        
            for( size_t i=0; i<length; ++i )
            {
                value[i] = copy.value[i];
            }
        }
        
        array( std::initializer_list<T> init ) : length( init.size() )
        {
            value = new T[ init.size() ];
            
            int index = 0;
            for( auto i = init.begin(); i != init.end(); ++i )
            {
                value[ index++ ] = *i;
            }
        }

        T* begin()
        {
            return value;
        }
        T* end()
        {
            return value + length;
        }
    
        const T* begin() const
        {
            return value;
        }
        const T* end() const
        {
            return value + length;
        }

        virtual ~array()
        {
            if( value != nullptr )
            {
                delete[] value;
            }
        }

        virtual const string& toString() const
        {
            return ARRAY_TO_STRING;
        }

        inline operator T*()
        {
            return value;
        }

        inline const operator T*() const
        {
            return value;
        }
        
        inline T& operator[]( size_t index )
        {
            return value[index];
        }

        inline const T& operator[]( size_t index ) const
        {
            return value[index];
        }
        
        inline array& operator=( const string& right )
        {
            if( value != nullptr )
            {
                delete[] value;
                value = nullptr;
            }
            
            ((size_t&)length) = right.length;
            
            if( length != 0 )
            {
                value = new T[length];
            
                for( size_t i=0; i<length; ++i )
                {
                    value[i] = right[i];
                }
            }
            return *this;
        }
    };
}

