#pragma once
#include <windows.h>
#include <iostream>

enum InjectedInputMouseOptions
{
	left_up = 4,
	left_down = 2,
	right_up = 16,
	right_down = 8
};

struct InjectedInputMouseInfo
{
	int move_direction_x;
	int move_direction_y;
	unsigned int mouse_data;
	InjectedInputMouseOptions mouse_options;
	unsigned int time_offset_in_miliseconds;
	void* extra_info;
};

namespace input
{
	bool(*NtUserInjectMouseInput)(InjectedInputMouseInfo*, int) = nullptr;

	auto init() -> void
	{
		LoadLibraryA("user32.dll");

		HMODULE win32u = LoadLibraryA("win32u.dll");

		void* NtUserInjectMouseInputAddress = (void*)GetProcAddress(win32u, "NtUserInjectMouseInput");
		*(void**)&NtUserInjectMouseInput = NtUserInjectMouseInputAddress;
	}

	auto mouse_move(int x = 0, int y = 0) -> bool
	{
		InjectedInputMouseInfo temp = {};
		temp.move_direction_x = x;
		temp.move_direction_y = y;
		return NtUserInjectMouseInput(&temp, 1);
	}

	auto left_down(int x = 0, int y = 0) -> bool
	{
		InjectedInputMouseInfo temp = {};
		temp.mouse_options = InjectedInputMouseOptions::left_down;
		temp.move_direction_x = x;
		temp.move_direction_y = y;
		return NtUserInjectMouseInput(&temp, 1);
	}

	auto left_up(int x = 0, int y = 0) -> bool
	{
		InjectedInputMouseInfo temp{};
		temp.mouse_options = InjectedInputMouseOptions::left_up;
		temp.move_direction_x = x;
		temp.move_direction_y = y;
		return NtUserInjectMouseInput(&temp, 1);
	}

	auto right_down(int x = 0, int y = 0) -> bool
	{
		InjectedInputMouseInfo temp{};
		temp.mouse_options = InjectedInputMouseOptions::right_down;
		temp.move_direction_x = x;
		temp.move_direction_y = y;
		return NtUserInjectMouseInput(&temp, 1);
	}

	auto right_up(int x = 0, int y = 0) -> bool
	{
		InjectedInputMouseInfo temp{};
		temp.mouse_options = InjectedInputMouseOptions::right_up;
		temp.move_direction_x = x;
		temp.move_direction_y = y;
		return NtUserInjectMouseInput(&temp, 1);
	}

};
