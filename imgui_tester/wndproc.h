#pragma once
#ifndef WNDPROC_H
#define WNDPROC_H

#include<Windows.h>

#include"imgui/imgui.h"
#include"imgui/imgui_impl_dx11.h"
#include"imgui/imgui_impl_win32.h"

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND, UINT, WPARAM, LPARAM);

namespace wndproc
{
	LRESULT __stdcall WndProc(HWND, UINT, WPARAM, LPARAM);
}

inline LRESULT __stdcall wndproc::WndProc(HWND hWindow, UINT uInt, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWindow, uInt, wParam, lParam))
		return true;

	switch (uInt)
	{
	case WM_SYSCOMMAND:
		if ((wParam & 0xfff0) == SC_KEYMENU)
			return 0;
		break;
	case WM_DESTROY:
		::PostQuitMessage(0);
		return 0;
	}
	return ::DefWindowProcW(hWindow, uInt, wParam, lParam);
}

#endif // !WNDPROC_H
