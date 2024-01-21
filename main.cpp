#include <dlfcn.h>

#ifndef RTLD_LAZY
    #define RTLD_LAZY 0x00001
#endif
#ifndef RTLD_NOW
    #define RTLD_NOW 0x00002
#endif
#ifndef RTLD_GLOBAL
    #define RTLD_GLOBAL 0x00100
#endif
#ifndef RTLD_LOCAL
    #define RTLD_LOCAL 0x00000
#endif


typedef int (*printf_t)(const char *format, ...);
#define DEFINE_FUNCTION(lib_name, func_name, func_type, alias_name)               \
    class FunctionLoader_##func_name                                                 \
    {                                                                                \
    public:                                                                          \
        static func_type load()                                                      \
        {                                                                            \
            void *handle = dlopen(lib_name, RTLD_LAZY);                              \
            if (!handle)                                                             \
            {                                                                        \
                return nullptr;                                                      \
            }                                                                        \
            func_type func = reinterpret_cast<func_type>(dlsym(handle, #func_name)); \
            if (!func)                                                               \
            {                                                                        \
                dlclose(handle);                                                     \
                return nullptr;                                                      \
            }                                                                        \
            return func;                                                             \
        }                                                                            \
    };                                                                               \
    func_type alias_name = FunctionLoader_##func_name::load();

DEFINE_FUNCTION("libc.so.6", printf, printf_t, my_printf)

int main()
{
    my_printf("Hello, World! %d\n", 123);
    return 0;
}