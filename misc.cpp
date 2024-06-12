#include<iostream>
#include"def.hpp"
using namespace std;


//fun for get time in mili sec
#ifdef WIN32
#include <windows.h>
#elif __APPLE__
#include <mach/mach_time.h>
#else
#include <time.h>
#endif

int get_time_ms(){
    #ifdef WIN32
        return GetTickCount();
    #elif __APPLE__
        static mach_timebase_info_data_t timebase;
        // Initialize the timebase info if not already done
        if (timebase.denom == 0) {
            mach_timebase_info(&timebase);
        }
        // Get the current absolute time
        uint64_t time = mach_absolute_time();
        // Convert the absolute time to milliseconds
        uint64_t milliseconds = (time * timebase.numer) / (timebase.denom * 1000000);
        
        return milliseconds;
    #else 
        struct timeval t;
        gettimeofday(&t,NULL);   
        return t.tv_sec*1000+t.tv_usec/1000;
    #endif
}