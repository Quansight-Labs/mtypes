#include <stdio.h>
// Check windows
#if _WIN32 || _WIN64
   #if _WIN64
     #define ENV64BIT
  #else
    #define ENV32BIT
  #endif
#endif

// Check GCC
#if __GNUC__
  #if __x86_64__ || __ppc64__
    #define ENV64BIT
  #else
    #define ENV32BIT
  #endif
#endif

int main() {
    if (sizeof(void*) == 8)
    {
        printf("ENV64BIT: Error: pointer should be 8 bytes. Exiting.");
	return 0; 
    }
    if (sizeof(void*) == 4)
    {
        printf("ENV32BIT: Error: pointer should be 4 bytes. Exiting.");
	return 0; 
    }
}
