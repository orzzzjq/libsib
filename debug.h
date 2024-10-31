#pragma once
#include <chrono>
using namespace std::chrono;

#define DEBUGING
//#define RECORDTIME

#ifdef DEBUGING
#define DEBUG(fmt, ...) fprintf(stderr, fmt, __VA_ARGS__);
#else
#define DEBUG(fmt, ...) ;
#endif

#ifdef RECORDTIME
auto start = high_resolution_clock::now();
auto stop = high_resolution_clock::now();
#define RECORDSTART() start = high_resolution_clock::now();
#define RECORDSTOP(x) stop = high_resolution_clock::now(); fprintf(stderr, "%s %dms ", (x), duration_cast<milliseconds>(stop - start).count());
#else
#define RECORDSTART() ;
#define RECORDSTOP(x) ;
#endif
