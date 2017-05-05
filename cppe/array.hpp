#pragma once

namespace  cppe
{
    template < class T >
    class array /*: public object*/
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
        
        array(std::initializer_list<T> init) : length( init.size() )
        {
            value = new T[ init.size() ];
            
            int index = 0;
            for( const auto& i = init.begin(); i != init.end(); ++i )
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
/*
        virtual const string& toString() const
        {
            static to_string = string( L"array" );
            return to_string;
        }

        inline T* operator()
        {
            return value;
        }

        inline const T* operator() const
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
                value = null;
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
*/
    };
}
