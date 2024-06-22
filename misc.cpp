#include<iostream>
#include"def.hpp"

#include <sys/select.h>
#include <unistd.h>

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

// // to check if there is any input waiting or not
// int input_waiting(){
//     #ifndef WIN32
//         fd_set readfds;
//         struct timeval tv;
//         FD_ZERO (&readfds);
//         FD_SET (fileno(stdin), &readfds);
//         tv.tv_sec=0; tv.tv_usec=0;
//         select(16, &readfds, 0, 0, &tv);

//         return (FD_ISSET(fileno(stdin), &readfds));
//     #else
//         static int init = 0, pipe;
//         static HANDLE inh;
//         DWORD dw;

//         if (!init) {
//             init = 1;
//             inh = GetStdHandle(STD_INPUT_HANDLE);
//             pipe = !GetConsoleMode(inh, &dw);
//             if (!pipe) {
//             SetConsoleMode(inh, dw & ~(ENABLE_MOUSE_INPUT|ENABLE_WINDOW_INPUT));
//             FlushConsoleInputBuffer(inh);
//             }
//         }
//         if (pipe) {
//             if (!PeekNamedPipe(inh, NULL, 0, NULL, &dw, NULL)) return 1;
//             return dw;
//         } else {
//             GetNumberOfConsoleInputEvents(inh, &dw);
//             return dw <= 1 ? 0 : dw;
//         }
//     #endif
// }

// // if waiting then read those bytes out of input
// void read_input(s_search_info *info) {

//     int bytes;
//     char input[256] = "", *endc;

//     if(input_waiting()){
//         info->stopped = true;
//         do{
//             bytes=read(fileno(stdin),input,256);
//         } while (bytes<0);

//         endc = strchr(input,'\n');
//         if(endc) *endc=0;

//         if(strlen(input) > 0){
//             if(!strncmp(input, "quit", 4)){
//                 info->quit = TRUE;
//             }
//         }
//         return;
//     }
// }