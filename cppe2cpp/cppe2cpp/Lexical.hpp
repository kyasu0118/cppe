#pragma once
#include <cstdio>
#include <cstdlib>
#include <string>
#include <list>
#include <vector>
#include <map>
#include <algorithm>
#include <exception>

enum TokenType
{
	TT_LEXEME, TT_STRING, TT_INT, TT_FLOAT,
};

struct Token
{
	TokenType   type;
	std::string lexeme;
};

struct VariableData
{
    std::string name, access_modifier, type, initial_value, array_length;
    bool is_const;
};

struct ArrayData
{
    std::string name, access_modifier, type, length;
    std::vector< std::string > initial_value;
    bool is_const;
    
    ArrayData()
    {
        is_const = false;
    }
};

struct MethodData
{
    std::string name, access_modifier, return_type, args;
    std::list< std::string > values;
    std::map< std::string, bool > refType;
};

struct ClassData
{
    std::string name;
    std::string base;
    std::vector< VariableData > classVariable;
    std::vector< VariableData > memberVariable;
    std::vector< MethodData > classMethod;
    std::vector< MethodData > memberMethod;
    std::map< std::string, bool > refType;
    std::vector< std::string > initializeClassVariableArray;
};

class Lexical
{
private:
	static void Scanner( const std::string& source, std::list<std::string>& lex )
	{
		unsigned int i,j;
		std::vector< char > buf( source.length() + 1 );
		char key;
        
        lex.clear();
        
		for( i=0, j=0; i<source.length()+1; ++i, ++j )
		{
			buf[j] = source[i];

			switch( source[i] )
			{
				case '\0':
					if( j != 0 )
					{
						lex.push_back( &buf[0] );
					}
					break;

				case '\'':
				case '\"':
					key = source[i];
					i++;
					j++;

					while( source[i] != key )
					{
						if( source[i] == '\0' )
						{
							lex.clear();
							return;
						}
						buf[j] = source[i];
						i++;
						j++;
					}
					buf[j] = '\0';
					lex.push_back( &buf[0] );
					j = -1;
					break;

				case '\r':
				case '\n':
				case '\t':
				case ' ':
					if( j != 0 )
					{
						buf[j] = '\0';
						lex.push_back( &buf[0] );
					}
					j = -1;
					break;

                case '+':
                case '-':
                case '*':
                case '/':
                    if( j > 0 )
                    {
                        buf[j] = '\0';
                        lex.push_back( &buf[0] );
                        buf[0] = source[i];
                        j=0;
                    }
                    if( source[i+1] == source[i] )
					{
						buf[j+1] = source[i];
						buf[j+2] = '\0';
						lex.push_back( &buf[0] );
						++i;
					}
                    else if( source[i+1] == '=' )
                    {
                        buf[j+1] = '=';
                        buf[j+2] = '\0';
                        lex.push_back( &buf[0] );
                        ++i;
                    }
                    else if( source[i+1] == '>' ) // arrow operator
                    {
                        buf[j+1] = '>';
                        buf[j+2] = '\0';
                        lex.push_back( &buf[0] );
                        ++i;
                    }
                    else
					{
						buf[j+1] = '\0';
						lex.push_back( &buf[0] );
					}
					j = -1;
					break;
                case ':':
                    if( source[i+1] == ':' )
                    {
                        buf[j] = '\0';
                        lex.push_back( &buf[0] );
                        lex.push_back( "::" );
                        ++i;
                    }
                    else
                    {
                        if( j != 0 )
                        {
                            buf[j] = '\0';
                            lex.push_back( &buf[0] );
                        }
                        lex.push_back( ":" );
                    }
                    j = -1;
                    break;
				case '=':
                case '[':
                case ']':
                case '(':
				case ')':
				case '{':
				case '}':
				case ';':
				case ',':
				case '.':
                case '&':
                case '<':
                case '>':
                case '#':
					if( j != 0 )
					{
						buf[j] = '\0';
						lex.push_back( &buf[0] );
					}
					buf[0] = source[i];
					buf[1] = '\0';
					lex.push_back( &buf[0] );
					j = -1;
					break;
			}
		}
	}

	static void Tokenizer( const std::list<std::string>& lex, std::list<Token>& tokens )
	{
		unsigned int i, j;
		Token token;
		std::list< std::string >::const_iterator ite;

		tokens.clear();

		for( ite=lex.begin(), i=0; ite != lex.end(); ++ite )
		{
            if( (*ite)[0] == '\''  )
			{
				token.type   = TT_INT;
				token.lexeme = &(*ite)[1];
			}
			else if( (*ite)[0] == '\"'  )
			{
				token.type = TT_STRING;
				token.lexeme = &(*ite)[1];
			}
			else if( (*ite)[0] >= '0' && (*ite)[0] <= '9' )
			{
				for( j=0; j<ite->length(); ++j )
				{
					if( (*ite)[j] == '.' )
					{
						break;
					}
				}

				if( j == ite->length() )
				{
					token.type = TT_INT;
				}
				else
				{
					token.type = TT_FLOAT;
				}
				token.lexeme = (*ite);
			}
			else
			{
				token.type   = TT_LEXEME;
				token.lexeme = (*ite);
			}
            tokens.push_back( token );
		}
	}

	static std::list< Token > GetTokens( const std::string& source )
	{
		std::list< std::string > lex;
		std::list< Token > tokens;

		Scanner( source, lex );

        Tokenizer( lex, tokens );

        return tokens;
	}
    
    static std::list< Token > m_Tokens;
    static std::vector< std::string > m_StringList;
    static std::vector< ClassData > m_Classes;
    static int m_ScopeDepth;
    static std::string m_Outputirectory;
    static std::vector< std::string > m_Preprocess;

    static void Step( std::list< Token >::const_iterator& i)
    {
        if( i == m_Tokens.end() )
        {
            throw std::exception();
        }
        ++i;
    }
    
    static void Class( std::list< Token >::const_iterator& i )
    {
        m_Classes.emplace_back(ClassData());
        ClassData& data = m_Classes.back();

        Step(i);
        data.name = i->lexeme;
        
        // inherit
        Step(i);
        if( i->lexeme == ":" )
        {
            Step(i);
            while( i->lexeme != "{" )
            {
                if( i->lexeme != ",")
                {
                    data.refType[ i->lexeme ] = true;
                }
                data.base += i->lexeme;
                Step(i);
            }
        }
        else if( i->lexeme == "{" )
        {
            data.base = "cppe::object";
        }
        
        Step(i);
        
        while( i->lexeme != "}" )
        {
            MethodOrVariavle( i );
        }
        
        Step(i);
        
        std::string initialize_list = "";
        for( int index=0; index<data.memberVariable.size(); ++index)
        {
            if( data.memberVariable[index].initial_value != "" )
            {
                if( initialize_list != "" )
                {
                    initialize_list += ", ";
                }
                else
                {
                    initialize_list = " : ";
                }
                initialize_list += data.memberVariable[index].name + "(" + data.memberVariable[index].initial_value + ")";
            }
        }

        bool isConstructor = false;
        for( int index=0; index<data.memberMethod.size(); ++index)
        {
            if( data.memberMethod[index].name == data.name )
            {
                data.memberMethod[index].args += initialize_list;
                
                auto ite = data.memberMethod[index].values.begin();
                ++ite;
                data.memberMethod[index].args += initialize_list;
                
                auto insert = data.memberMethod[index].values.begin();
                ++insert;
                isConstructor = true;
            }
        }
        if( isConstructor == false )
        {
            MethodData constructor;
            constructor.access_modifier = "public";
            constructor.return_type = "";
            constructor.name = data.name;
            constructor.args = "()" + initialize_list;
            constructor.values.push_back("{");
            constructor.values.push_back("}");
            data.memberMethod.push_back(constructor);
        }

        bool isToString = false;
        for( int index=0; index<data.memberMethod.size(); ++index)
        {
            if( data.memberMethod[index].name == "toString" )
            {
                isConstructor = true;
                break;
            }
        }
        if( isToString == false )
        {
            MethodData toString;
            toString.access_modifier = "public";
            toString.return_type = "const cppe::string&";
            toString.name = "toString";
            toString.args = "() const";
            toString.values.push_back("{");
            toString.values.push_back("  return "+String(data.name)+";");
            toString.values.push_back("}");
            data.memberMethod.push_back(toString);
        }
        
        MethodData operatorString;
        operatorString.access_modifier = "public";
        operatorString.return_type = "cppe::string";
        operatorString.name = "operator +";
        operatorString.args = "(const cppe::string& right) const";
        operatorString.values.push_back("{");
        operatorString.values.push_back("  return toString() + right;");
        operatorString.values.push_back("}");
        data.memberMethod.push_back(operatorString);

        MethodData type;
        type.access_modifier = "public";
        type.return_type = "const std::type_info&";
        type.name = "type";
        type.args = "() const";
        type.values.push_back("{");
        type.values.push_back("  return typeid("+data.name+");" );
        type.values.push_back("}");
        data.memberMethod.push_back(type);
    }
    
    static void MethodOrVariavle( std::list< Token >::const_iterator& i  )
    {
        ClassData& data = m_Classes.back();
        std::string access_modifier = "";
        bool isClass = false;
        
        if( i->lexeme == "public" || i->lexeme == "protected" || i->lexeme == "private" )
        {
            access_modifier = i->lexeme;
            Step(i);
        }
        else
        {
            throw std::exception();
        }

        if( i->lexeme == "static" )
        {
            isClass = true;
            Step(i);
        }
        
        const std::list< Token >::const_iterator pos = i;
        while( i->lexeme != "{" && i->lexeme != ";" )
        {
            Step(i);
        }
        // variable
        if( i->lexeme == ";" )
        {
            VariableData v;
            v.access_modifier = access_modifier;
            i = pos;
            
            v.is_const = false;
            if( i->lexeme == "const" )
            {
                v.is_const = true;
                Step(i);
            }
            
            v.type = PlaneChange(i);
            data.refType[v.type] = true;
            v.name = PlaneChange(i);
            
            if( i->lexeme == "=" )
            {
                Step(i);

                while( i->lexeme != ";" )
                {
                    v.initial_value += Change(i);
                }
            }
            // array.
            if( i->lexeme == "[" )
            {
                Step(i);
                v.type = "cppe::array< " + v.type + " >";
                if( i->type == TT_INT )
                {
                    v.array_length = PlaneChange(i);
                    if( isClass == false )
                    {
                        v.initial_value = v.array_length;
                    }
                }
                
                while( i->lexeme != ";" )
                {
                    Step(i);
                }
            }
            else if( i->lexeme != ";" )
            {
                throw std::exception();
            }
            Step(i);
            if( isClass )
            {
                data.classVariable.push_back(v);
            }
            else
            {
                data.memberVariable.push_back(v);
            }
        }
        // method
        else
        {
            MethodData m;
            m.access_modifier = access_modifier;
            i = pos;
            
            if( i->lexeme != data.name ) // constructor
            {
                m.return_type = PlaneChange(i);
                if( m.return_type != "void" )
                {
                    m.refType[ m.return_type ] = true;
                }
            }
            m.name = PlaneChange(i);
 
            MethodValues( m, i );

            if( isClass )
            {
                data.classMethod.push_back(m);
            }
            else
            {
                data.memberMethod.push_back(m);
            }
        }
    }

    static void MethodValues( MethodData& m, std::list< Token >::const_iterator& i  )
    {
        while( i->lexeme != "{" )
        {
            m.args += Change(i);
        }
        ScopeDetail(m,i);
    }

    static std::string PlaneChange( std::list< Token >::const_iterator& i  )
    {
        std::string result;
        
        if( i->type == TT_STRING)
        {
            result = String( i );
        }
        else if( i->lexeme == "string" || i->lexeme == "console" )
        {
            result = std::string("cppe::") + i->lexeme;
            Step(i);
        }
        else
        {
            result = i->lexeme;
            Step(i);
        }
        return result;
    }
    
    static std::string Change( std::list< Token >::const_iterator& i  )
    {
        std::string now = i->lexeme;
        std::string result = PlaneChange(i);
        std::string next = i->lexeme;
        
        if(
           (
           now == "." || now == "::" || now == "++" || now == "--" ||
           next == "(" || next == "." || next == ":" || next == "::" || next == ";" ||
           (now == "(" && next == ")")
           ) == false )
        {
            result += " ";
        }
        return result;
    }
    static void Foreach(MethodData& m, std::list< Token >::const_iterator& i  )
    {
        bool is_const = false;
        Step(i); //foreach
        Step(i); //(
        if( i->lexeme == "const")
        {
            is_const = true;
            Step(i);
        }
        std::string type = PlaneChange(i);
        
        while( i->lexeme == "*" || i->lexeme == "&" )
        {
            type += PlaneChange(i);
        }
        
        std::string name = PlaneChange(i);
        Step(i); //in
        std::string collection = PlaneChange(i);
        
        char buf[256] = {};
        
        if( is_const )
        {
            sprintf(buf, "for( auto cppe_iterator%d = %s.cbegin(); cppe_iterator%d != %s.cend(); ++cppe_iterator%d )",
                    m_ScopeDepth, collection.c_str(), m_ScopeDepth, collection.c_str(), m_ScopeDepth );
        }
        else
        {
            sprintf(buf, "for( auto cppe_iterator%d = %s.begin(); cppe_iterator%d != %s.end(); ++cppe_iterator%d )",
                    m_ScopeDepth, collection.c_str(), m_ScopeDepth, collection.c_str(), m_ScopeDepth );
        }
        m.values.emplace_back(buf);
        
       Step(i); // )
        
        m.values.push_back("{");
        Step(i);
        
        if( is_const )
        {
            sprintf( buf, "const %s %s = *cppe_iterator%d;", type.c_str(), name.c_str(), m_ScopeDepth );
        }
        else
        {
            sprintf( buf, "%s %s = *cppe_iterator%d;", type.c_str(), name.c_str(), m_ScopeDepth );
        }
        m.values.push_back(buf);
        
        ScopeDetail(m,i,false);
    }
    static void ForWhileIf(MethodData& m, std::list< Token >::const_iterator& i  )
    {
        std::string result;
        while( i->lexeme != "{")
        {
            result += Change(i);
        }
        m.values.emplace_back(result);
    }
    
    static void Switch(MethodData& m, std::list< Token >::const_iterator& i  )
    {
        std::string result;
        while( i->lexeme != "{")
        {
            result += Change(i);
        }
        m.values.emplace_back(result);
    }
    
    static bool Variable( MethodData& m, std::list< Token >::const_iterator& i  )
    {
        std::string type = "";
        bool isVarible = false;
        bool isArray = false;
        ArrayData array;
        int element_index = 0;
        std::string line;
        char array_initialize[256] = {};
        std::list< Token >::const_iterator back_point;
        
        if( i->lexeme == "static")
        {
            array.access_modifier = "static";
            isVarible = true;
            back_point = i;

            Step(i);
            if( i->lexeme == "const" )
            {
                Step(i);
                array.is_const = true;
                m.refType[i->lexeme] = true;
            }
            i = back_point;
        }
        else if( i->lexeme == "const" )
        {
            back_point = i;
            isVarible = true;
            array.is_const = true;
            Step(i);
            m.refType[i->lexeme] = true;
            i = back_point;
        }
        else
        {
            back_point = i;
            Step(i);
            if( i->lexeme != "("  && i->lexeme != "{" &&
               i->lexeme != "::" && i->lexeme != "." &&
               i->lexeme != "->" )
            {
                isVarible = true;
                m.refType[i->lexeme] = true;
            }
            i = back_point;
        }

        if( isVarible )
        {
            i = back_point;
            if( i->lexeme == "const" )
            {
                Step(i);
            }
            type += PlaneChange(i);
            Step(i);
            while( i->lexeme != ";" && i->lexeme != "=" )
            {
                --i;
                if( i->lexeme != "*" && i->lexeme != "&" &&
                   i->lexeme != "[" && i->lexeme != "]" && i->type != TT_INT  )
                {
                    type += " ";
                }
                if( i->lexeme == "[")
                {
                    isArray = true;
                    --i;
                    array.name = i->lexeme;
                    Step(i);
                    
                    Step(i);
                    if( i->type == TT_INT )
                    {
                        array.length = i->lexeme;
                    }
                    --i;
                }
                type += PlaneChange(i);
                Step(i);
            }
            --i;
            type += i->lexeme;
            i = back_point;
            
            if( isArray )
            {
                std::string arrayType;

                for( int index=0; type[index] != ' '; ++ index )
                {
                    arrayType += type[index];
                }
                array.type = arrayType;
                while( i->lexeme != ";" && i->lexeme != "=" )
                {
                    Step(i);
                }
                
                if( array.is_const )
                {
                    line += "const ";
                }
                
                line += "cppe::array< " + arrayType + " > " + array.name;
                
                if( i->lexeme == "=" )
                {
                    Step(i);
                    if( i->lexeme != "{" )
                    {
                        throw std::exception();
                    }
                    Step(i);
                    
                    int length = 1;
                    
                    back_point = i;
                    
                    if( i->lexeme == "}" )
                    {
                        length = 0;
                    }
                    else
                    {
                        while( i->lexeme != "}" )
                        {
                            if( i->lexeme == "," )
                            {
                                ++length;
                            }
                            Step(i);
                        }
                    }
                    i = back_point;
                    if( length != 0 )
                    {
                        char count_string[32] = {};
                        if( array.length != "" )
                        {
                            sprintf( count_string, "(%s);", array.length.c_str() );
                        }
                        else
                        {
                            sprintf( count_string, "(%d);", length );
                        }
                        line += std::string( count_string );
                        m.values.emplace_back(line);
                        
                        sprintf( array_initialize, "new ( &((cppe::array< %s >&)%s)[%d] ) %s(",
                                array.type.c_str(), array.name.c_str(), element_index, array.type.c_str() );
                        line = array_initialize;
                        
                        while( i->lexeme != "}" )
                        {
                            if( i->lexeme == "," )
                            {
                                line += ");";
                                m.values.emplace_back(line);
                                ++element_index;
                                sprintf( array_initialize, "new ( &((cppe::array< %s >&)%s)[%d] ) %s(",
                                        array.type.c_str(), array.name.c_str(), element_index, array.type.c_str() );
                                line = array_initialize;
                                ++i;
                            }
                            else
                            {
                                line += PlaneChange(i);
                            }
                        }
                        line += ");";
                        m.values.emplace_back(line);
                        ++element_index;
                    }
                    else
                    {
                        line += array.length+");";
                        m.values.emplace_back(line);
                    }
                }
                else
                {
                    if( array.length != "" )
                    {
                        line += std::string("(") + array.length + ")";
                    }
                    line += ";";
                    m.values.emplace_back(line);
                }
                
                for( ; element_index<atoi(array.length.c_str()); ++element_index )
                {
                    sprintf( array_initialize, "new ( &((cppe::array< %s >&)%s)[%d] ) %s();",
                            array.type.c_str(), array.name.c_str(), element_index, array.type.c_str() );
                    line = array_initialize;
                    
                    m.values.emplace_back(line);
                }
                while( i->lexeme != ";")
                {
                    Step(i);
                }
                Step(i);
                return true;
            }
        }
        return false;
    }
    
    static void Line( MethodData& m, std::list< Token >::const_iterator& i  )
    {
        if( Variable( m, i ) == true )
        {
            return;
        }
        std::string line = "";
        
        while( i->lexeme != ";" )
        {
            line += Change(i);
        }
        line += Change(i);
        m.values.emplace_back(line);
    }

    static void ScopeDetail(MethodData& m, std::list< Token >::const_iterator& i, bool begin = true  )
    {
        if( begin )
        {
            BeginScope( m, i );
        }
        else
        {
            ++m_ScopeDepth;
        }
        while( i->lexeme != "}" )
        {
            if( i->lexeme == "{")
            {
                ScopeDetail( m, i );
            }
            else if ( i->lexeme == "do" )
            {
                m.values.emplace_back( Change(i) );
                ScopeDetail( m, i);
                Line( m, i );
            }
            else if ( i->lexeme == "foreach" )
            {
                Foreach( m, i );
            }
            else if ( i->lexeme == "for" || i->lexeme == "while" || i->lexeme == "if" )
            {
                ForWhileIf( m, i );
            }
            else if ( i->lexeme == "switch" )
            {
                Switch( m, i );
            }
            else
            {
                Line( m, i );
            }
        }
        
        EndScope( m, i );
    }
    
    static void BeginScope(MethodData& m, std::list< Token >::const_iterator& i  )
    {
        m.values.push_back("{");
        Step(i);
        ++m_ScopeDepth;
    }
    static void EndScope(MethodData& m, std::list< Token >::const_iterator& i )
    {
        m.values.push_back("}");
        Step(i);
        --m_ScopeDepth;
    }

    static std::string String( const std::string lexeme )
    {
        int find_index = -1;
        for( int index=0; index<m_StringList.size(); ++index )
        {
            if( m_StringList[index] == lexeme)
            {
                find_index = index;
                break;
            }
        }
        if( find_index == -1 )
        {
            m_StringList.push_back(lexeme);
            find_index = (int)m_StringList.size()-1;
        }
        char string_name[32] = {};
        sprintf( string_name, "cppe::STRING_%04d", find_index );
        
        return string_name;
    }
    
    static std::string String( std::list< Token >::const_iterator& i )
    {
        int find_index = -1;
        for( int index=0; index<m_StringList.size(); ++index )
        {
            if( m_StringList[index] == i->lexeme)
            {
                find_index = index;
                break;
            }
        }
        if( find_index == -1 )
        {
            m_StringList.push_back(i->lexeme);
            find_index = (int)m_StringList.size()-1;
        }
        char string_name[32] = {};
        sprintf( string_name, "cppe::STRING_%04d", find_index );
        Step(i);
        
        return string_name;
    }

public:
    static void Analysis(const std::string& source )
	{
		m_Tokens = GetTokens( source );
        
        for( auto i=m_Tokens.cbegin(); i!=m_Tokens.end(); Step(i) )
		{
            if( i->lexeme == "class" )
            {
                Class(i);
                --i;
            }
            else if( i->lexeme == "#" )
            {
                std::string preprocess = "#";
                ++i;
                
                while( i->lexeme != "#" && i->lexeme != "class" )
                {
                    preprocess += i->lexeme;
                    ++i;
                }
                m_Preprocess.push_back( preprocess );
                --i;
            }
		}
	}

    static std::vector< std::string > GetStringList()
    {
        return m_StringList;
    }
    
    static void Convert( const std::string& filepath, const std::string& name )
    {
        std::string ext;
        size_t ext_pos = name.rfind('.');
        
        if( ext_pos == std::string::npos )
        {
            return;
        }
        
        for( size_t i = ext_pos+1; i < name.size(); ++i )
        {
            ext += name[i];
        }
        if( ext != "cppe")
        {
            return;
        }

        FILE* fp = fopen( filepath.c_str(), "r" );
        size_t size = 0;
        std::vector< char > data;
        
        fseek( fp, 0, SEEK_END );
        size = ftell( fp );
        
        if( size == 0 )
        {
            fclose( fp );
            return;
        }
        
        data.resize(size);
        fseek( fp, 0, SEEK_SET);
        fread( &data[0], size, 1, fp );
        
        fclose( fp );
        
        Analysis( &data[0] );
    }

    static std::map<std::string, bool> GetRefType(const ClassData& c)
    {
        std::map<std::string, bool> result;
        
        for( auto ite = c.refType.begin(); ite != c.refType.end(); ++ite )
        {
            result[ ite->first ] = true;
        }
        
        for( int i = 0; i<c.classMethod.size(); ++i )
        {
            for( auto ite = c.classMethod[i].refType.cbegin(); ite != c.classMethod[i].refType.cend(); ++ite )
            {
                result[ ite->first ] = true;
            }
            for( auto ite = c.memberMethod[i].refType.cbegin(); ite != c.memberMethod[i].refType.cend(); ++ite )
            {
                result[ ite->first ] = true;
            }
        }
        
        return result;
    }
    static int DepthClass(ClassData c)
    {
        int result = 1;
        auto type = GetRefType( c );
        
        for( int i=0; i<m_Classes.size(); ++i )
        {
            if( m_Classes[i].name == c.name )
            {
                continue;
            }
            
            for( auto ite = type.begin(); ite != type.end(); ++ite )
            {
                if( m_Classes[i].name == ite->first )
                {
                    result += DepthClass( m_Classes[i] );
                }
            }
        }
        return result;
    }
    
    static bool CompareClass(ClassData left, ClassData right)
    {
        return DepthClass( left ) < DepthClass( right );
    }
    
    static void WriteMethodValues( FILE* fp, const std::list< std::string >& v )
    {
        int depth = 0;
        
        for( auto i=v.cbegin(); i != v.cend(); ++i )
        {
            if( *i == "}" )
            {
                --depth;
            }
            
            if( i->substr(0,4) == "case" )
            {
                --depth;
            }
            
            for( int l=0; l<depth; ++l )
            {
                fprintf( fp, "\t");
            }
            
            if( i->substr(0,4) == "case" )
            {
                ++depth;
            }
            
            if( *i == "{" )
            {
                ++depth;
            }
            fprintf( fp, "\t%s\n", (*i).c_str());
        }
    }
    
    static void WriteClass()
    {
        std::vector<ClassData> sort = m_Classes;
        std::sort(sort.begin(),sort.end(),CompareClass);
        m_Classes = sort;

        std::vector<ClassData>& c = m_Classes;

        std::vector<std::string> stringList = Lexical::GetStringList();
        
        for( int i=0; i<c.size(); ++i )
        {
            const std::string filepath = m_Outputirectory + "user/" + c[i].name + ".hpp";

            FILE* fp = fopen( filepath.c_str(), "w" );

            fprintf( fp, "#pragma once\n");
            
            fprintf( fp, "class %s", c[i].name.c_str());
            if( c[i].base != "" )
            {
                fprintf( fp, " : public %s", c[i].base.c_str());
            }
            fprintf( fp, "\n");
            fprintf( fp, "{\n");
            
            for( int j=0; j<c[i].classVariable.size(); ++j )
            {
                fprintf( fp, "\t%s: static ", c[i].classVariable[j].access_modifier.c_str());
                if( c[i].classVariable[j].is_const )
                {
                    fprintf( fp, "const ");
                }
                fprintf( fp, "%s ", c[i].classVariable[j].type.c_str());
                fprintf( fp, "%s", c[i].classVariable[j].name.c_str());
                
                if( c[i].classVariable[j].is_const && c[i].classVariable[j].type == "int" )
                {
                    fprintf( fp, " = %s", c[i].classVariable[j].initial_value.c_str());
                }
                fprintf( fp, ";\n" );
            }
            
            for( int j=0; j<c[i].memberVariable.size(); ++j )
            {
                fprintf( fp, "\t%s: ", c[i].memberVariable[j].access_modifier.c_str());
                if( c[i].memberVariable[j].is_const )
                {
                    fprintf( fp, "const ");
                }
                fprintf( fp, "%s ", c[i].memberVariable[j].type.c_str());
                fprintf( fp, "%s;\n", c[i].memberVariable[j].name.c_str());
            }
            
            for( int j=0; j<c[i].classMethod.size(); ++j )
            {
                for( auto ite=c[i].classMethod[j].refType.begin(); ite != c[i].classMethod[j].refType.end(); ++ite )
                {
                    printf( "\t\t%s\n", ite->first.c_str());
                }

                fprintf( fp, "\t%s: static ", c[i].classMethod[j].access_modifier.c_str());
                fprintf( fp, "%s ", c[i].classMethod[j].return_type.c_str());
                fprintf( fp, "%s", c[i].classMethod[j].name.c_str());
                fprintf( fp, "%s\n", c[i].classMethod[j].args.c_str());
                
                WriteMethodValues( fp, c[i].classMethod[j].values );
            }
            for( int j=0; j<c[i].memberMethod.size(); ++j )
            {
                fprintf( fp, "\t%s: ", c[i].memberMethod[j].access_modifier.c_str());
                if( c[i].memberMethod[j].return_type != "" )
                {
                    fprintf( fp, "%s ", c[i].memberMethod[j].return_type.c_str());
                }
                fprintf( fp, "%s", c[i].memberMethod[j].name.c_str());
                fprintf( fp, "%s\n", c[i].memberMethod[j].args.c_str());
                
                WriteMethodValues( fp, c[i].memberMethod[j].values );
            }
            fprintf( fp, "};\n");

            for( int j=0; j<c[i].classVariable.size(); ++j )
            {
                if( c[i].classVariable[j].type != "int" ||
                    c[i].classVariable[j].is_const == false )
                {
                    if( c[i].classVariable[j].is_const )
                    {
                        fprintf( fp, "const ");
                    }
                    fprintf( fp, "%s %s::%s",
                            c[i].classVariable[j].type.c_str(),
                            c[i].name.c_str(),
                            c[i].classVariable[j].name.c_str() );
                    if( c[i].classVariable[j].initial_value != "" )
                    {
                        fprintf( fp, " = %s", c[i].classVariable[j].initial_value.c_str());
                    }
                    if( c[i].classVariable[j].array_length != "" )
                    {
                        fprintf( fp, "(%s)", c[i].classVariable[j].array_length.c_str());
                    }
                    fprintf( fp, ";\n");
                }
            }
            
            fclose( fp );
        }
    }
    
    static void WriteUserInclude()
    {
        FILE* fp = fopen( (m_Outputirectory + "user/include.hpp").c_str(), "w");
        fprintf( fp, "#pragma once\n");

        fprintf( fp, "namespace cppe\n" );
        fprintf( fp, "{\n" );
        
        for( int i=0; i<m_StringList.size(); ++i )
        {
            fprintf( fp, "\tstatic const string STRING_%04d = L\"%s\";\n", i, m_StringList[i].c_str());
        }
        fprintf( fp, "}\n\n" );

        for( int i=0; i<m_Preprocess.size(); ++i )
        {
            fprintf( fp, "%s\n", m_Preprocess[i].c_str());
        }
        fprintf( fp, "\n\n" );
        
        for( int i=0; i<m_Classes.size(); ++i )
        {
            fprintf( fp, "#include \"%s.hpp\"\n", m_Classes[i].name.c_str());
        }

        fclose( fp );
    }
    
    static void SetOutputDirectory( const std::string& directory )
    {
        m_Outputirectory = directory + "/";
    }
};

std::list< Token > Lexical::m_Tokens;
std::vector< std::string > Lexical::m_Preprocess;
std::vector< std::string > Lexical::m_StringList;
std::vector<ClassData> Lexical::m_Classes;
int Lexical::m_ScopeDepth = 0;
std::string Lexical::m_Outputirectory;
