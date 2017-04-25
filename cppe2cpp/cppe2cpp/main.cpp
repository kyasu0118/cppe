#include "Lexical.hpp"

// MAC
#ifdef __APPLE__
    #include <sys/types.h>
    #include <sys/types.h>
    #include <sys/stat.h>
    #include <dirent.h>
    #include <unistd.h>

    static void convert( const std::string& target )
    {
        DIR* dp = opendir( target.c_str() );
        
        if ( dp != nullptr )
        {
            dirent* dent = nullptr;
            while( (dent = readdir(dp)) != nullptr )
            {
                const std::string name = dent->d_name;
                
                if( name == "." || name == "..")
                {
                    continue;
                }

                struct stat st;
                
                std::string path = target + name;
                stat( path.c_str(), &st);
                
                if ((st.st_mode & S_IFMT) == S_IFDIR)
                {
                    convert( path + "/" );
                }
                else
                {
                    Lexical::Convert(path, name );
                }
            }
            closedir(dp);
        }
    }


    static void delete_file( const std::string& directory)
    {
        DIR* dp = opendir( directory.c_str() );
        
        if ( dp != nullptr )
        {
            dirent* dent = nullptr;
            while( (dent = readdir(dp)) != nullptr )
            {
                const std::string name = dent->d_name;
                
                if( name == "." || name == "..")
                {
                    continue;
                }
                
                struct stat st;
                
                std::string path = directory + name;
                stat( path.c_str(), &st);
                
                if ((st.st_mode & S_IFMT) == S_IFREG)
                {
                    unlink(path.c_str());
                }
            }
            closedir(dp);
        }
    }

// Windows
#elif defined _WIN32 || defined _WIN64

#endif

int main(int argc, const char * argv[])
{
    if( argc == 3 )
    {
        Lexical::SetOutputDirectory(argv[1]);
        delete_file( std::string(argv[1]) + "/user/" );
        convert( argv[2] );
        Lexical::WriteStringList();
        Lexical::WriteClass();
        Lexical::WriteUserInclude();
    }
    else
    {
        printf( "cppe argsc error\n");
    }
    return 0;
}
