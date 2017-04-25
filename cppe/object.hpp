#pragma once

namespace cppe
{
    class object
    {
    public:
        object(){}
        virtual ~object(){}
        
        virtual const std::type_info& type() const
        {
            return typeid(object);
        }
        virtual const class string& toString() const = 0;
    };
}
