#include <windows.h>
#include <d3d9.h>

#pragma comment(lib, "d3d9.lib")

LPDIRECT3D9 d3d; // Direct3D 인터페이스
LPDIRECT3DDEVICE9 d3ddev; // Direct3D 디바이스
LPDIRECT3DVERTEXBUFFER9 v_buffer = NULL; // 정점 버퍼

struct CUSTOMVERTEX {
    FLOAT x, y, z; // 위치
    DWORD color;   // 색상
};
#define CUSTOMFVF (D3DFVF_XYZ | D3DFVF_DIFFUSE)

void InitD3D(HWND hwnd);
void RenderFrame(void);
void CleanD3D(void);
void InitGraphics(void);

LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    WNDCLASS wc = { 0 };
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = L"WindowClass";

    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(
        0, L"WindowClass", L"DirectX Triangle",
        WS_OVERLAPPEDWINDOW, 100, 100, 800, 600,
        NULL, NULL, hInstance, NULL);

    ShowWindow(hwnd, nCmdShow);

    InitD3D(hwnd);
    InitGraphics();

    MSG msg;
    while (TRUE) {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT) break;
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        RenderFrame();
    }

    CleanD3D();
    return msg.wParam;
}

void InitD3D(HWND hwnd) {
    d3d = Direct3DCreate9(D3D_SDK_VERSION);

    D3DPRESENT_PARAMETERS d3dpp = { 0 };
    d3dpp.Windowed = TRUE;
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dpp.hDeviceWindow = hwnd;

    d3d->CreateDevice(D3DADAPTER_DEFAULT,
        D3DDEVTYPE_HAL,
        hwnd,
        D3DCREATE_SOFTWARE_VERTEXPROCESSING,
        &d3dpp,
        &d3ddev);
}

void InitGraphics() {
    CUSTOMVERTEX vertices[] = {
        { 0.0f, 0.5f, 0.0f, D3DCOLOR_XRGB(255, 0, 0)},
        { 0.5f, -0.5f, 0.0f, D3DCOLOR_XRGB(0, 255, 0)},
        { -0.5f, -0.5f, 0.0f, D3DCOLOR_XRGB(0, 0, 255)},
    };

    d3ddev->CreateVertexBuffer(3 * sizeof(CUSTOMVERTEX),
        0,
        CUSTOMFVF,
        D3DPOOL_MANAGED,
        &v_buffer,
        NULL);

    VOID* pVoid;
    v_buffer->Lock(0, 0, (void**)&pVoid, 0);
    memcpy(pVoid, vertices, sizeof(vertices));
    v_buffer->Unlock();
}

void RenderFrame(void) {
    d3ddev->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(30, 30, 60), 1.0f, 0);
    d3ddev->BeginScene();

    d3ddev->SetFVF(CUSTOMFVF);
    d3ddev->SetStreamSource(0, v_buffer, 0, sizeof(CUSTOMVERTEX));
    d3ddev->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 1);

    d3ddev->EndScene();
    d3ddev->Present(NULL, NULL, NULL, NULL);
}

void CleanD3D(void) {
    v_buffer->Release();
    d3ddev->Release();
    d3d->Release();
}
