#pragma once

#pragma region SendInPut

namespace Sendinput
{
	void SendVKcodesUp(BYTE vk)
	{
		INPUT input;
		ZeroMemory(&input, sizeof(INPUT));
		input.type = INPUT_KEYBOARD;
		input.ki.wVk = vk;

		input.ki.dwFlags = KEYEVENTF_KEYUP;

		SendInput(1, &input, sizeof(INPUT));
	}

	void SendVKcodes(BYTE vk)
	{
		INPUT input;
		ZeroMemory(&input, sizeof(INPUT));
		input.type = INPUT_KEYBOARD;
		input.ki.wVk = vk;

		SendInput(1, &input, sizeof(INPUT));

		input.ki.dwFlags = KEYEVENTF_KEYUP;

		SendInput(1, &input, sizeof(INPUT));
	}
}

#pragma endregion