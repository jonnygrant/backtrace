// g++-8 -Wall -g -o exception4 exception4.cpp
#include <vector>
#include <iostream>
#include <string>

#include <execinfo.h>
#include <stdio.h>
#include <stdlib.h>

int main2();

/* Obtain a backtrace and print it to stdout.
 https://www.gnu.org/software/libc/manual/html_node/Backtraces.html
 */
void print_trace(void)
{
    void *array[10];
    const size_t size = backtrace (array, 10);
    char **strings = backtrace_symbols (array, size);

    //printf ("Obtained %zd stack frames.\n", size);
    printf("\nBacktrace:\n");

    // skip first, as it is this handler
    for (size_t i = 1; i < size; i++)
    {
        // extract the exe name
        std::string exe(strings[i]);
        {
            const size_t s = exe.find("(");
            if(std::string::npos != s)
            {
                exe.erase(s, exe.length());
            }
        }

        // extract the address
        std::string addr(strings[i]);
        {
            size_t s = addr.find("(");
            if(std::string::npos != s)
            {
                ++s;
                addr.erase(0, s);

                s = addr.find(")");
                if(std::string::npos != s)
                {
                    addr.erase(s, addr.length());
                }
            }
        }

        //printf("exe '%s' addr '%s'\n", exe.c_str(), addr.c_str());

        char syscom[256];
        sprintf(syscom,"addr2line -s -a -p -f -C -e %s %s", exe.c_str(), addr.c_str());
        //printf("%s\n", syscom);
        system(syscom);

        printf ("[%zu]: %s\n", i, strings[i]);
    }

    free (strings);
}

void test()
{
    try
    {
        main2();
    }
    catch( const std::exception &e)
    {
        const std::string what(e.what());
        std::cout << "Unhandled C++ exception: [" << e.what() << "]\n";
        print_trace();
        //char * p = NULL;
        //*p = 1;
    }
}


int main(int argc, const char * argv[])
{
    test();
}


int main2()
{
    std::vector<int> v;
    return v.at(0);
}

