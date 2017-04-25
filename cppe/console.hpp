#pragma once

namespace cppe
{
    class console
    {
    public:
        static void print( const wchar_t* format, ... )
        {
            setlocale( LC_ALL, CPPE_LANGUAGE );
            
            va_list arg;
            
            va_start( arg, format );
            vwprintf( format, arg );
            va_end( arg );
        }
    };
}
