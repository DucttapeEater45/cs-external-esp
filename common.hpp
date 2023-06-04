#pragma once
#include <Windows.h>
#include <cstdint>
#include <cstddef>
#include <cstdio>
#include <string>
#include <string_view>
#include <TlHelp32.h>
#include <iostream>
#include <thread>

#include "memory.hpp"
#include "hazedumper.hpp"

inline std::uintptr_t g_game_base;
inline std::uintptr_t g_client_base;
 
#include "sdk.hpp"
#include "overlay.h"