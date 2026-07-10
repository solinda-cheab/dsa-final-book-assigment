#ifndef YUKKK_H
#define YUKKK_H

#include <cstddef>
#include <cstdint>
#include <initializer_list>
#include <limits>
#include <new>
#include <typeinfo>
#include <exception>
#include <stdexcept>
#include <iostream>
#include <istream>
#include <ostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <array>
#include <vector>
#include <deque>
#include <list>
#include <forward_list>
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <stack>
#include <queue>
#include <string>
#include <string>
#include <cstring>
#include <cctype>
// #include <charconv>
#include <algorithm>
#include <functional>
#include <utility>
#include <iterator>
#include <numeric>
// #include <compare>
#include <memory>
#include <scoped_allocator>
#include <chrono>
#include <ctime>
#include <cmath>
#include <complex>
#include <random>
#include <ratio>
#include <valarray>
#include <thread>
#include <mutex>
#include <shared_mutex>
#include <condition_variable>
#include <future>
#include <atomic>
// #include <filesystem>
#include <regex>
#include <locale>
#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <cerrno>
#include <climits>
#include <cfloat>
#include <csignal>
#include <csetjmp>
#include <cstdarg>
#include <windows.h>
#include <mmsystem.h>

#define RED1 "\033[38;2;237;23;23m"
#define RED2 "\033[38;2;242;50;17m"
#define RED3 "\033[38;2;246;100;12m"
#define RED4 "\033[38;2;251;125;6m"
#define RED5 "\033[38;2;255;150;0m"
#define RED6 "\033[38;2;255;170;0m"

#define LIGHTGREEN1 "\033[38;2;175;255;0m"
#define LIGHTGREEN2 "\033[38;2;175;255;95m"
#define LIGHTGREEN3 "\033[38;2;175;255;135m"
#define LIGHTGREEN4 "\033[38;2;175;255;175m"
#define LIGHTGREEN5 "\033[38;2;175;255;200m"
#define LIGHTGREEN6 "\033[38;2;175;255;215m"

#define BLUE1 "\033[38;2;0;37;246m"
#define BLUE2 "\033[38;2;0;74;248m"
#define BLUE3 "\033[38;2;0;111;249m"
#define BLUE4 "\033[38;2;0;148;251m"
#define BLUE5 "\033[38;2;0;185;252m"

#define GREEN1 "\033[38;2;0;171;8m"
#define GREEN2 "\033[38;2;0;195;1m"
#define GREEN3 "\033[38;2;38;215;1m"
#define GREEN4 "\033[38;2;77;237;48m"
#define GREEN5 "\033[38;2;149;249;133m"

#define PINK1 "\033[38;2;198;50;100m"
#define PINK2 "\033[38;2;212;76;135m"
#define PINK3 "\033[38;2;227;102;177m"
#define PINK4 "\033[38;2;241;127;197m"
#define PINK5 "\033[38;2;255;153;218m"

#define RESET "\033[0m"

using namespace std;

void success()
{

    cout << "\n";
    Beep(750, 300);

    cout << "\033[38;2;0;255;0m                                                ____      ____     \033[0m" << endl;
    cout << "\033[38;2;0;255;95m           _______  _______________  __________/ __/_  __/ / /_  __\033[0m" << endl;
    cout << "\033[38;2;0;255;135m          / ___/ / / / ___/ ___/ _ \\/ ___/ ___/ /_/ / / / / / / / /\033[0m" << endl;
    cout << "\033[38;2;0;255;135m         (__  ) /_/ / /__/ /__/  __(__  |__  ) __/ /_/ / / / /_/ / \033[0m" << endl;
    cout << "\033[38;2;0;255;175m        /____/\\__,_/\\___/\\___/\\___/____/____/_/  \\__,_/_/_/\\__, /  \033[0m" << endl;
    cout << "\033[38;2;0;255;175m                                                          /____/   \033[0m" << endl;

    cout << "\n";
}

void fail()
{

    cout << "\n";
    Beep(750, 300);

    cout << "\033[38;2;215;0;0m\t    ______      _ __         __\033[0m" << endl;
    cout << "\033[38;2;215;0;0m\t   / ____/___ _(_) /__  ____/ /\033[0m" << endl;
    cout << "\033[38;2;215;0;0m\t  / /_  / __ `/ / / _ \\/ __  / \033[0m" << endl;
    cout << "\033[38;2;215;0;0m\t / __/ / /_/ / / /  __/ /_/ /  \033[0m" << endl;
    cout << "\033[38;2;215;0;0m\t/_/    \\__,_/_/_/\\___/\\__,_/   \033[0m" << endl;

    cout << "\n";
}

#endif
