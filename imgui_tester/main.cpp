#include"dx11.h"
#include"wndproc.h"

float window_bg[4] = { 255,255,255,255 };
float window_border[4] = { 0,0,0,0 };

void draw_menu(const char* title, int width, int height)
{
	ImGui::Begin(title, nullptr, 0);
	ImGui::SetWindowSize(ImVec2(width, height));
	ImGui::End();
}

int main()
{
	WNDCLASSEX Windowclass = { sizeof(Windowclass), CS_CLASSDC, wndproc::WndProc, 0L, 0L, GetModuleHandle(nullptr), nullptr, nullptr, nullptr,
		nullptr, L"ImGui Window", nullptr};
	::RegisterClassExW(&Windowclass);
	HWND hWindow = ::CreateWindowW(Windowclass.lpszClassName, L"Window For Dev ImGui Menus", WS_OVERLAPPEDWINDOW, 100, 100, 1280, 800, nullptr,
		nullptr, Windowclass.hInstance, nullptr);

	if (hWindow == 0)
	{
		_tprintf(L"failed to create window\n");
		Sleep(5000);
		return -1;
	}

	if (dx11::CreateDevice(hWindow) == false)
	{
		_tprintf(L"failed to create device\n");
		::UnregisterClassW(Windowclass.lpszClassName, Windowclass.hInstance);
		Sleep(5000);
		return -1;
	}

	::ShowWindow(hWindow, SW_SHOWDEFAULT);
	::UpdateWindow(hWindow);

	ImGui::CreateContext(); 
	//ImGui::StyleColorsDark();

	ImGui_ImplWin32_Init(hWindow);
	ImGui_ImplDX11_Init(pDevice, pContext);

	bool finish = false;
	while (!finish)
	{
		MSG msg;
		while (::PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
		{
			::TranslateMessage(&msg);
			::DispatchMessageW(&msg);
			if (msg.message == WM_QUIT)
				finish = true;
		}

		if (finish)
			break;

		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();	
		{
			ImGui::Begin("ImGui Menu");
			ImGui::SetWindowSize(ImVec2(450, 450));
			ImGui::End();
		}

		ImGui::EndFrame();
		ImGui::Render();
		pContext->OMSetRenderTargets(1, &pRenderTargetView, nullptr);
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
		pSwapChain->Present(1, 0);
	}

	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	dx11::CleanupDevice();
	::DestroyWindow(hWindow);
	::UnregisterClassW(Windowclass.lpszClassName, Windowclass.hInstance);
}