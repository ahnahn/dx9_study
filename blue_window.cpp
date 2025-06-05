// main.cpp
#include <windows.h>
#include <d3d9.h>

#pragma comment (lib, "d3d9.lib")

LPDIRECT3D9 d3d;
LPDIRECT3DDEVICE9 d3ddev;

// 윈도우 프로시저 선언
LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Direct3D 초기화
void InitD3D(HWND hwnd);

// 그리기
void RenderFrame();

// Direct3D 해제
void CleanD3D();

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, WindowProc, 0L, 0L,
                      hInstance, NULL, NULL, NULL, NULL,
                      L"DX9Window", NULL };
    RegisterClassEx(&wc);

    HWND hwnd = CreateWindow(L"DX9Window", L"DirectX 9 Demo",
        WS_OVERLAPPEDWINDOW, 100, 100, 800, 600,
        NULL, NULL, hInstance, NULL);

    ShowWindow(hwnd, nCmdShow);

    InitD3D(hwnd);

    MSG msg;
    while (true) {
        if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE)) {
            if (msg.message == WM_QUIT) break;
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        RenderFrame();
    }

    CleanD3D();
    UnregisterClass(L"DX9Window", hInstance);
    return 0;
}

void InitD3D(HWND hwnd) {
    d3d = Direct3DCreate9(D3D_SDK_VERSION);

    D3DPRESENT_PARAMETERS d3dpp = {};
    d3dpp.Windowed = TRUE;
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dpp.hDeviceWindow = hwnd;

    d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd,
        D3DCREATE_SOFTWARE_VERTEXPROCESSING,
        &d3dpp, &d3ddev);
}

void RenderFrame() {
    d3ddev->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 40, 100), 1.0f, 0);
    d3ddev->BeginScene();

    // 여기에 그리기 코드가 들어감

    d3ddev->EndScene();
    d3ddev->Present(NULL, NULL, NULL, NULL);
}

void CleanD3D() {
    d3ddev->Release();
    d3d->Release();
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}
