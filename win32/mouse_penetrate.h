#pragma once

#include <Windows.h>
#include <windowsx.h>


HWND hWnd;


void Init();
void Final();
void OnPaint();
void OnSize(int, int);
void OnWheel(int, bool ctrl, int x, int y);

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);


int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow) {
    const wchar_t className[] = L"D2DTestWndClass";
    const wchar_t titleName[] = L"D2DTest";

    WNDCLASSEX wc = {};
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.hInstance = hInstance;
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpfnWndProc = WndProc;
    wc.lpszClassName = className;

    if (!RegisterClassExW(&wc)) { throw; }

    hWnd = CreateWindowExW(WS_EX_NOREDIRECTIONBITMAP, // WS_EX_LAYERED | WS_EX_TRANSPARENT, // , //|  
                           className, titleName,
                           WS_POPUP | WS_THICKFRAME | WS_MAXIMIZEBOX, // WS_OVERLAPPEDWINDOW, //
                           200, 200, 800, 500,
                           NULL, NULL, hInstance, NULL);

    if (hWnd == NULL) { throw; }

    //SetLayeredWindowAttributes(hWnd, 0, 255, LWA_ALPHA);
    //UpdateLayeredWindow(hWnd, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);

    Init();

    ShowWindow(hWnd, SW_SHOW);
    OnPaint();  // WM_PAINT will never be sent when WS_EX_LAYERED is used.

    MSG msg;

    while (GetMessageW(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }

    Final();

    return (int)msg.wParam;
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_PAINT:
        PAINTSTRUCT ps;
        BeginPaint(hWnd, &ps);
        OnPaint();
        EndPaint(hWnd, &ps);
        break;
    case WM_SIZE:
        if (::hWnd == nullptr) { break; }  // If use WS_POPUP rather than WS_OVERLAPPEDWINDOW, 
                                           //   WM_SIZE will be sent before window is created.
        OnSize(LOWORD(lParam), HIWORD(lParam));
        OnPaint();
        break;

    case WM_NCCALCSIZE:  // Process the message to set client region the same as the window region.
        break;
    case WM_NCACTIVATE:  // Do not draw the nonclient area.
        break;

    case WM_ERASEBKGND: return false;
    case WM_LBUTTONDOWN:  // Send the fake mouse messages hit on caption to fool DefWindowProc().
        SendMessage(hWnd, WM_NCLBUTTONDOWN, HTCAPTION, lParam);
        break;
    case WM_MOUSEWHEEL: {
            RECT rect;
            GetWindowRect(hWnd, &rect);
            OnWheel(
                GET_WHEEL_DELTA_WPARAM(wParam),
                (GET_KEYSTATE_WPARAM(wParam) & MK_CONTROL) != 0,
                GET_X_LPARAM(lParam) - rect.left, GET_Y_LPARAM(lParam) - rect.top
            );
        }break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, msg, wParam, lParam);
    }
    return 0;
}



#include <string>

#include <d2d1_1.h>
#include <dwrite.h>
#include <dxgi1_4.h>
#include <d3d11.h>

#include <dcomp.h>


#pragma comment(lib, "d2d1.lib" )
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dwrite.lib")
#pragma comment(lib, "dxguid.lib")  // For effects.

#pragma comment(lib, "dcomp.lib")


ID2D1Factory1* pD2DFactory = nullptr;
ID2D1DeviceContext* pD2DDeviceContext = nullptr;

ID2D1Bitmap1* pD2DTargetBitmap = nullptr;
ID2D1SolidColorBrush* pBrush = nullptr;

IDWriteFactory* pDWriteFactory;
IDWriteTextFormat* pDWriteTextFormat;
IDWriteTextLayout* pDWriteTextLayout;

ID3D11Device* pD3DDevice;
IDXGISwapChain1* pSwapChain = nullptr;

HRESULT hr = S_OK;

template<class Interface>
inline void SafeRelease(Interface*& pInterfaceToRelease) {
    if (pInterfaceToRelease != nullptr) {
        pInterfaceToRelease->Release();
        pInterfaceToRelease = nullptr;
    }
}


void Init();
void Final();

void CreateDeviceIndependentResources();
void DiscardDeviceIndependentResources();

void CreateDeviceDependentResources();
void DiscardDeviceDependentResources();

void CreateWindowSizeDependentResources();
void DiscardWindowSizeDependentResources();



void Init() {
    hr = CoInitialize(nullptr);
    if (FAILED(hr)) { throw; }

    CreateDeviceIndependentResources();
    CreateDeviceDependentResources();
    CreateWindowSizeDependentResources();
}

void Final() {
    DiscardWindowSizeDependentResources();
    DiscardDeviceDependentResources();
    DiscardDeviceIndependentResources();

    CoUninitialize();
}

void CreateDeviceIndependentResources() {
    hr = D2D1CreateFactory(
        D2D1_FACTORY_TYPE_SINGLE_THREADED,
        &pD2DFactory
    );
    if (FAILED(hr)) { throw; }


    hr = DWriteCreateFactory(
        DWRITE_FACTORY_TYPE_SHARED,
        __uuidof(IDWriteFactory),
        reinterpret_cast<IUnknown**>(&pDWriteFactory)
    );
    if (FAILED(hr)) { throw; }

    hr = pDWriteFactory->CreateTextFormat(
        L"Arial",
        NULL,
        DWRITE_FONT_WEIGHT_NORMAL,
        DWRITE_FONT_STYLE_NORMAL,
        DWRITE_FONT_STRETCH_NORMAL,
        50.0f,
        L"en-US",
        &pDWriteTextFormat
    );
    if (FAILED(hr)) { throw; }

    static std::wstring text = L"Hello world!";

    hr = pDWriteFactory->CreateTextLayout(
        text.c_str(),
        text.length(),
        pDWriteTextFormat,
        200.0f, 100.0f,
        &pDWriteTextLayout
    );
    if (FAILED(hr)) { throw; }

}

void DiscardDeviceIndependentResources() {
    SafeRelease(pDWriteTextLayout);
    SafeRelease(pDWriteTextFormat);
    SafeRelease(pDWriteFactory);

    SafeRelease(pD2DFactory);
}

void CreateDeviceDependentResources() {
    D3D_FEATURE_LEVEL featureLevels[] = {
        D3D_FEATURE_LEVEL_11_1,
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0,
        D3D_FEATURE_LEVEL_9_3,
        D3D_FEATURE_LEVEL_9_2,
        D3D_FEATURE_LEVEL_9_1
    };

    D3D_FEATURE_LEVEL featureLevel;

    hr = D3D11CreateDevice(
        nullptr,
        D3D_DRIVER_TYPE_HARDWARE, nullptr,
        D3D11_CREATE_DEVICE_BGRA_SUPPORT | D3D11_CREATE_DEVICE_DEBUG,   // debug layer
        featureLevels, ARRAYSIZE(featureLevels),
        D3D11_SDK_VERSION,
        &pD3DDevice,
        &featureLevel,
        nullptr
    );
    if (FAILED(hr)) { throw; }

    IDXGIDevice1* pDxgiDevice = nullptr;
    hr = pD3DDevice->QueryInterface(IID_PPV_ARGS(&pDxgiDevice));
    if (FAILED(hr)) { throw; }

    ID2D1Device* pD2DDevice = nullptr;
    hr = pD2DFactory->CreateDevice(pDxgiDevice, &pD2DDevice);
    if (FAILED(hr)) { throw; }
    SafeRelease(pDxgiDevice);

    hr = pD2DDevice->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE, &pD2DDeviceContext);
    if (FAILED(hr)) { throw; }
    SafeRelease(pD2DDevice);

    hr = pD2DDeviceContext->CreateSolidColorBrush(
        D2D1::ColorF(D2D1::ColorF::DarkBlue), &pBrush);
    if (FAILED(hr)) { throw; }
}

void DiscardDeviceDependentResources() {
    SafeRelease(pBrush);

    SafeRelease(pD2DTargetBitmap);
    SafeRelease(pSwapChain);
    SafeRelease(pD2DDeviceContext);

#ifdef _DEBUG
    ID3D11Debug* d3d_debug;
    hr = pD3DDevice->QueryInterface(__uuidof(ID3D11Debug), reinterpret_cast<void**>(&d3d_debug));
    d3d_debug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
    SafeRelease(d3d_debug);
#endif
    SafeRelease(pD3DDevice);
}

void CreateWindowSizeDependentResources() {
    DiscardWindowSizeDependentResources();

    RECT rect = { 0 }; GetClientRect(hWnd, &rect);

    if (pSwapChain == nullptr) {
        IDXGIDevice1* pDxgiDevice = nullptr;
        IDXGIAdapter* pDxgiAdapter = nullptr;
        IDXGIFactory2* pDxgiFactory = nullptr;

        hr = pD3DDevice->QueryInterface(IID_PPV_ARGS(&pDxgiDevice));
        if (FAILED(hr)) { throw; }

        hr = pDxgiDevice->GetAdapter(&pDxgiAdapter);
        if (FAILED(hr)) { throw; }

        hr = pDxgiAdapter->GetParent(IID_PPV_ARGS(&pDxgiFactory));
        if (FAILED(hr)) { throw; }

        DXGI_SWAP_CHAIN_DESC1 swapChainDesc = { 0 };
        swapChainDesc.Width = rect.right - rect.left;
        swapChainDesc.Height = rect.bottom - rect.top;
        swapChainDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
        swapChainDesc.Stereo = false;
        swapChainDesc.SampleDesc.Count = 1;
        swapChainDesc.SampleDesc.Quality = 0;
        swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        swapChainDesc.BufferCount = 2;
        swapChainDesc.Scaling = DXGI_SCALING_STRETCH;
        swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
        swapChainDesc.Flags = 0;
        swapChainDesc.AlphaMode = DXGI_ALPHA_MODE_PREMULTIPLIED;
        hr = pDxgiFactory->CreateSwapChainForComposition(pD3DDevice, &swapChainDesc, nullptr, &pSwapChain);
        if (FAILED(hr)) { throw; }

        IDCompositionDevice* dcompDevice;
        DCompositionCreateDevice(
            pDxgiDevice,
            __uuidof(dcompDevice),
            reinterpret_cast<void**>(&dcompDevice));

        IDCompositionTarget* target;
        dcompDevice->CreateTargetForHwnd(hWnd, true, &target);

        IDCompositionVisual* visual;
        dcompDevice->CreateVisual(&visual);

        visual->SetContent(pSwapChain);

        target->SetRoot(visual);

        dcompDevice->Commit();

        hr = pDxgiDevice->SetMaximumFrameLatency(1);
        if (FAILED(hr)) { throw; }

        SafeRelease(pDxgiFactory);
        SafeRelease(pDxgiAdapter);
        SafeRelease(pDxgiDevice);

    } else {
        hr = pSwapChain->ResizeBuffers(
            2,
            rect.right - rect.left,
            rect.bottom - rect.top,
            DXGI_FORMAT_B8G8R8A8_UNORM,
            0);
        if (FAILED(hr)) { throw; }
    }

    IDXGISurface* pDxgiBackBuffer = nullptr;
    hr = pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pDxgiBackBuffer));
    if (FAILED(hr)) { throw; }

    D2D1_BITMAP_PROPERTIES1 bitmapProperties = D2D1::BitmapProperties1(
        D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
        D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED)
    );

    hr = pD2DDeviceContext->CreateBitmapFromDxgiSurface(
        pDxgiBackBuffer,
        &bitmapProperties,
        &pD2DTargetBitmap
    );
    if (FAILED(hr)) { throw; }

    SafeRelease(pDxgiBackBuffer);
}

void DiscardWindowSizeDependentResources() {
    SafeRelease(pD2DTargetBitmap);
}

void OnSize(int width, int height) {
    CreateWindowSizeDependentResources();
}


struct Shape {
    unsigned width = 200, height = 100;
    int center_x = 100, center_y = 50;   // relative to shape
    int offset_x = 400, offset_y = 100;   // relative to target
    int rotation = 30;
}shape;


void OnPaint() {
    pD2DDeviceContext->BeginDraw();
    pD2DDeviceContext->SetTarget(pD2DTargetBitmap);
    pD2DDeviceContext->Clear(D2D1::ColorF(D2D1::ColorF::White, 0.0));

    pBrush->SetColor(D2D1::ColorF(D2D1::ColorF::DodgerBlue));
    pD2DDeviceContext->FillRectangle(D2D1::RectF(200, 100, 600, 400), pBrush);


    D2D1::Matrix3x2F transform;
    pD2DDeviceContext->GetTransform(&transform);

    pD2DDeviceContext->SetTransform(
        D2D1::Matrix3x2F::Rotation(shape.rotation, D2D1::Point2F(shape.offset_x, shape.offset_y)) * transform
    );

    D2D1_RECT_F rect = D2D1::RectF(
        shape.offset_x - shape.center_x,
        shape.offset_y - shape.center_y,
        shape.offset_x - shape.center_x + shape.width,
        shape.offset_y - shape.center_y + shape.height
    );

    D2D1_LAYER_PARAMETERS layer = {};
    layer.contentBounds = rect;
    layer.opacity = 1.0;

    pD2DDeviceContext->PushLayer(layer, NULL);

    pBrush->SetColor(D2D1::ColorF(D2D1::ColorF::Firebrick));
    pD2DDeviceContext->DrawRectangle(
        rect,
        pBrush
    );

    pBrush->SetColor(D2D1::ColorF(D2D1::ColorF::OliveDrab));
    pD2DDeviceContext->DrawTextLayout(
        D2D1::Point2F(rect.left, rect.top),
        pDWriteTextLayout,
        pBrush
    );

    pD2DDeviceContext->PopLayer();

    pD2DDeviceContext->SetTransform(
        transform
    );

    pD2DDeviceContext->SetTarget(nullptr);

    hr = pD2DDeviceContext->EndDraw();
    if (FAILED(hr)) { throw; }

    DXGI_PRESENT_PARAMETERS present_parameters = {};
    pSwapChain->Present1(0, 0, &present_parameters);
}

void OnWheel(int delta, bool ctrl, int x, int y) {
    if (ctrl == false) {
        shape.rotation += delta * 10 / 120;
        shape.rotation %= 360;
    } else {
        float scale = delta > 0 ? 1.2 : 0.8;

        pD2DDeviceContext->SetTarget(pD2DTargetBitmap);
        D2D1::Matrix3x2F transform;
        pD2DDeviceContext->GetTransform(&transform);
        pD2DDeviceContext->SetTransform(
            transform * D2D1::Matrix3x2F::Scale(scale, scale, D2D1::Point2F(x, y))
        );
        pD2DDeviceContext->SetTarget(nullptr);
    }
    OnPaint();
}