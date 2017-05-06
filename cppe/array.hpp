#pragma once

namespace  cppe
{
    static const string ARRAY_TO_STRING = L"array";

    template < class T >
    class array : public object
    {
    private:
        T* value;
        
    public:
        const size_t length;

    private:
        inline void release()
        {
            for( size_t i=0; i<length; ++i )
            {
                value[i].~T();
            }
            free( value );
            value = NULL;
        }
        
    public:
        array(const array& copy) : length(copy.length)
        {
            value = (T*)malloc( sizeof(T)*length );
            
            for( size_t i=0; i<length; ++i )
            {
                new( &value[i] ) T();
                value[i] = copy.value[i];
            }
        }
        
        array() : length(0)
        {
            value = nullptr;
        }
    
        array( size_t _length, size_t initialize_count, ... ) : length( _length )
        {
            value = (T*)calloc( length, sizeof(T) );
            
            va_list list;
            va_start(list, initialize_count);
            
            for( size_t i=0; i<initialize_count; ++i )
            {
                new( &value[i] ) T();
                value[i] = va_arg( list , T );
            }
            
            va_end(list);
        }
    
/* C++11
        array( std::initializer_list<T> init ) : length( init.size() )
*/
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

        const T* cbegin() const
        {
            return value;
        }
        const T* cend() const
        {
            return value + length;
        }

        virtual ~array()
        {
            release();
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
        
        inline array& operator=( const array& right )
        {
            release();
            
            ((size_t&)length) = right.length;
            
            if( length != 0 )
            {
                value = (T*)calloc( sizeof(T)*length );
                
                for( size_t i=0; i<length; ++i )
                {
                    new( &value[i] ) T();
                    value[i] = right.value[i];
                }
            }
            return *this;
        }
    };
}

