#pragma once

#include <io.h>
#include <fcntl.h>

#include "Engine/Managers/GraphicsManager.h"
#include "Engine/Graphics/Mesh.h"

#include "Engine/GameObjects/CameraGameObject.h"
#include "Engine/Graphics/ConstantBuffer.h"

#include "Engine/Managers/Events/EventManager.h"
#include "Engine/Managers/Events/EventObserver.h"

#include "Engine/Helpers/Inputs.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HRESULT	InitWindow(HINSTANCE hInstance, int nCmdShow);
HRESULT	InitDevice();
void CleanupDevice();

void Render();

void BindQuadBuffers();

HINSTANCE g_hInstance;

HWND g_hWnd;

D3D_DRIVER_TYPE g_driverType = D3D_DRIVER_TYPE_NULL;
D3D_FEATURE_LEVEL g_featureLevel = D3D_FEATURE_LEVEL_11_0;
ID3D11Device* g_pd3dDevice = nullptr;
ID3D11DeviceContext* g_pImmediateContext = nullptr;
IDXGISwapChain* g_pSwapChain = nullptr;
ID3D11RenderTargetView* g_pRenderTargetView = nullptr;
ID3D11Texture2D* g_pDepthStencil = nullptr;
ID3D11DepthStencilView* g_pDepthStencilView = nullptr;
ID3D11RasterizerState* g_prasterState = nullptr;

CameraGameObject* g_pcamera = nullptr;

GameObject* g_ptestObject;

ConstantBuffer<PerFrameCB>* g_pperFrameCB;
ConstantBuffer<PerInstanceCB>* g_pperInstanceCB;

int g_Width = 1920;
int g_Height = 1080;

Inputs* g_inputController;

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

#if _DEBUG
	AllocConsole();

	HANDLE handle_out = GetStdHandle(STD_OUTPUT_HANDLE);
	int hCrt = _open_osfhandle((long)handle_out, _O_TEXT);
	FILE* hf_out = _fdopen(hCrt, "w");
	setvbuf(hf_out, NULL, _IONBF, 1);
	*stdout = *hf_out;

	freopen_s(&hf_out, "CONOUT$", "w", stdout);
#endif //_DEBUG

	if (FAILED(InitWindow(hInstance, nCmdShow)))
		return 0;

	if (FAILED(InitDevice()))
	{
		CleanupDevice();
		return 0;
	}

  ExampleObserver observer(new int(10));
	EventManager::Instance()->AddClient(EventType::KeyPressed,&observer);
	EventManager::Instance()->AddClient(EventType::KeyReleased,&observer);
	EventManager::Instance()->AddClient(EventType::MouseButtonPressed, &observer);
	EventManager::Instance()->AddClient(EventType::MouseButtonReleased, &observer);
	EventManager::Instance()->AddClient(EventType::MouseMoved, &observer);

	GraphicsManager::GetInstance()->Init(g_pd3dDevice);

	GraphicsManager::GetInstance()->LoadTextureFromFile(L"Resources/Sprites/Brick.dds", g_pd3dDevice);

	//Setup input classes
	g_inputController = new Inputs();


	//Create camera
	XMFLOAT3 cameraPos = XMFLOAT3(0, 0, 0);
	XMFLOAT3 cameraForward = XMFLOAT3(0, -1, 0);
	XMFLOAT3 cameraUp = XMFLOAT3(0, 0, 1);

	float windowWidth = g_Width;
	float windowHeight = g_Height;

	float nearDepth = 0.01f;
	float farDepth = 1000.0f;

	g_pcamera = new CameraGameObject("Camera");
	g_pcamera->Initialize(cameraPos, cameraForward, cameraUp, windowWidth, windowHeight, nearDepth, farDepth);

	//Create constant buffers
	g_pperFrameCB = new ConstantBuffer<PerFrameCB>(g_pd3dDevice);
	g_pperInstanceCB = new ConstantBuffer<PerInstanceCB>(g_pd3dDevice);

	//Create test gameobject
	XMFLOAT3 objectPos = XMFLOAT3(0, -5.0f, 0);
	XMFLOAT3 objectScale = XMFLOAT3(100, 100, 100);

	g_ptestObject = new GameObject("tempTile");
	g_ptestObject->SetMesh(QUAD_MESH_NAME);
	g_ptestObject->SetVertexShader(DEFAULT_VERTEX_SHADER_NAME);
	g_ptestObject->SetPixelShader(DEFAULT_PIXEL_SHADER_NAME);
	g_ptestObject->SetAlbedo(L"Resources/Sprites/Brick.dds");
	g_ptestObject->GetTransform()->SetPosition(objectPos);
	g_ptestObject->GetTransform()->SetScale(objectScale);


	// Main message loop
	MSG msg = { 0 };
	while (WM_QUIT != msg.message)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);




		}
		else
		{
			Render();
			//LOG("Console output test");

			EventManager::Instance()->ProcessEvents();
		}
	}

	CleanupDevice();

	delete g_pperFrameCB;
	g_pperFrameCB = nullptr;

	delete g_pperInstanceCB;
	g_pperInstanceCB = nullptr;

	return (int)msg.wParam;
}

//--------------------------------------------------------------------------------------
// Called every time the application receives a message
//--------------------------------------------------------------------------------------
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	

	//Handing the keyboard inputs to a keyboard class & mouse inputs to a mouse class
	g_inputController->Update(&hWnd, &message, &wParam, &lParam);




	switch (message)
	{
	case WM_LBUTTONDOWN:
	{
		break;
	}
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
		{
			PostQuitMessage(0);
		}
		break;
	case WM_KEYUP:
		break;
	case WM_MOUSEMOVE:
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	

	return 0;
}

inline HRESULT InitWindow(HINSTANCE hInstance, int nCmdShow)
{
	// Register class
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, (LPCTSTR)107);
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = nullptr;
	wcex.lpszClassName = L"TutorialWindowClass";
	wcex.hIconSm = LoadIcon(wcex.hInstance, (LPCTSTR)107);

	if (!RegisterClassEx(&wcex))
		return E_FAIL;

	// Create window
	g_hInstance = hInstance;
	RECT rc = { 0, 0, g_Width, g_Height };

	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
	g_hWnd = CreateWindow(L"TutorialWindowClass", L"Cool Engine",
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
		CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, nullptr, nullptr, hInstance,
		nullptr);
	if (!g_hWnd)
		return E_FAIL;

	ShowWindow(g_hWnd, nCmdShow);

	return S_OK;
}

inline HRESULT InitDevice()
{
	HRESULT hr = S_OK;

	RECT rc;
	GetClientRect(g_hWnd, &rc);
	UINT width = rc.right - rc.left;
	UINT height = rc.bottom - rc.top;

	UINT createDeviceFlags = 0;
#ifdef _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_DRIVER_TYPE driverTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE,
	};
	UINT numDriverTypes = ARRAYSIZE(driverTypes);

	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
	};
	UINT numFeatureLevels = ARRAYSIZE(featureLevels);

	for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
	{
		g_driverType = driverTypes[driverTypeIndex];
		hr = D3D11CreateDevice(nullptr, g_driverType, nullptr, createDeviceFlags, featureLevels, numFeatureLevels,
			D3D11_SDK_VERSION, &g_pd3dDevice, &g_featureLevel, &g_pImmediateContext);

		if (hr == E_INVALIDARG)
		{
			// DirectX 11.0 platforms will not recognize D3D_FEATURE_LEVEL_11_1 so we need to retry without it
			hr = D3D11CreateDevice(nullptr, g_driverType, nullptr, createDeviceFlags, &featureLevels[1], numFeatureLevels - 1,
				D3D11_SDK_VERSION, &g_pd3dDevice, &g_featureLevel, &g_pImmediateContext);
		}

		if (SUCCEEDED(hr))
			break;
	}
	if (FAILED(hr))
		return hr;

	// Obtain DXGI factory from device (since we used nullptr for pAdapter above)
	IDXGIFactory* dxgiFactory = nullptr;
	{
		IDXGIDevice* dxgiDevice = nullptr;
		hr = g_pd3dDevice->QueryInterface(__uuidof(IDXGIDevice), reinterpret_cast<void**>(&dxgiDevice));
		if (SUCCEEDED(hr))
		{
			IDXGIAdapter* adapter = nullptr;
			hr = dxgiDevice->GetAdapter(&adapter);
			if (SUCCEEDED(hr))
			{
				hr = adapter->GetParent(__uuidof(IDXGIFactory), reinterpret_cast<void**>(&dxgiFactory));
				adapter->Release();
			}
			dxgiDevice->Release();
		}
	}

	if (FAILED(hr))
	{
		return hr;
	}

	// Create swap chain
	DXGI_SWAP_CHAIN_DESC sd = {};
	sd.BufferCount = 1;
	sd.BufferDesc.Width = width;
	sd.BufferDesc.Height = height;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = g_hWnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;

	hr = dxgiFactory->CreateSwapChain(g_pd3dDevice, &sd, &g_pSwapChain);

	// Note this tutorial doesn't handle full-screen swapchains so we block the ALT+ENTER shortcut
	dxgiFactory->MakeWindowAssociation(g_hWnd, DXGI_MWA_NO_ALT_ENTER);

	dxgiFactory->Release();

	if (FAILED(hr))
		return hr;

	// Create a render target view
	ID3D11Texture2D* pBackBuffer = nullptr;
	hr = g_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&pBackBuffer));
	if (FAILED(hr))
		return hr;

	hr = g_pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &g_pRenderTargetView);
	pBackBuffer->Release();
	if (FAILED(hr))
		return hr;

	// Create depth stencil texture
	D3D11_TEXTURE2D_DESC descDepth = {};
	descDepth.Width = width;
	descDepth.Height = height;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;
	hr = g_pd3dDevice->CreateTexture2D(&descDepth, nullptr, &g_pDepthStencil);
	if (FAILED(hr))
		return hr;

	// Create the depth stencil view
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV = {};
	descDSV.Format = descDepth.Format;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;
	hr = g_pd3dDevice->CreateDepthStencilView(g_pDepthStencil, &descDSV, &g_pDepthStencilView);
	if (FAILED(hr))
		return hr;

	g_pImmediateContext->OMSetRenderTargets(1, &g_pRenderTargetView, g_pDepthStencilView);

	// Setup the viewport
	D3D11_VIEWPORT vp;
	vp.Width = (FLOAT)width;
	vp.Height = (FLOAT)height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	g_pImmediateContext->RSSetViewports(1, &vp);

	if (FAILED(hr))
	{
		return hr;
	}

	D3D11_RASTERIZER_DESC rasterDesc;
	ZeroMemory(&rasterDesc, sizeof(D3D11_RASTERIZER_DESC));
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.CullMode = D3D11_CULL_NONE;
	hr = g_pd3dDevice->CreateRasterizerState(&rasterDesc, &g_prasterState);

	if (FAILED(hr))
	{
		return hr;
	}

	g_pImmediateContext->RSSetState(g_prasterState);

	g_pImmediateContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return S_OK;
}

void CleanupDevice()
{
	// Remove any bound render target or depth/stencil buffer
	ID3D11RenderTargetView* nullViews[] = { nullptr };
	g_pImmediateContext->OMSetRenderTargets(_countof(nullViews), nullViews, nullptr);

	if (g_pImmediateContext)
	{
		g_pImmediateContext->ClearState();
	}

	// Flush the immediate context to force cleanup
	g_pImmediateContext->Flush();

	if (g_pDepthStencil)
	{
		g_pDepthStencil->Release();
	}

	if (g_pDepthStencilView)
	{
		g_pDepthStencilView->Release();
	}

	if (g_pRenderTargetView)
	{
		g_pRenderTargetView->Release();
	}

	if (g_pSwapChain)
	{
		g_pSwapChain->Release();
	}

	if (g_pImmediateContext)
	{
		g_pImmediateContext->Release();
	}

#if _DEBUG
	ID3D11Debug* debugDevice = nullptr;
	g_pd3dDevice->QueryInterface(__uuidof(ID3D11Debug), reinterpret_cast<void**>(&debugDevice));

	if (g_pd3dDevice)
	{
		g_pd3dDevice->Release();
	}

	// handy for finding dx memory leaks
	debugDevice->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);

	if (debugDevice)
	{
		debugDevice->Release();
	}
#endif //_DEBUG
}

void Render()
{
	// Clear the back buffer
	g_pImmediateContext->ClearRenderTargetView(g_pRenderTargetView, DirectX::Colors::MidnightBlue);

	// Clear the depth buffer to 1.0 (max depth)
	g_pImmediateContext->ClearDepthStencilView(g_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

	g_pImmediateContext->IASetInputLayout(GraphicsManager::GetInstance()->GetInputLayout(GraphicsManager::InputLayouts::POS_TEX));

	BindQuadBuffers();

	//Bind sampler
	ID3D11SamplerState* psampler = GraphicsManager::GetInstance()->GetSampler(GraphicsManager::Samplers::LINEAR_WRAP);

	g_pImmediateContext->PSSetSamplers(0, 1, &psampler);

	//Update per frame CB
	PerFrameCB perFrameCB;
	XMStoreFloat4x4(&perFrameCB.viewProjection, XMMatrixTranspose(XMLoadFloat4x4(&g_pcamera->GetViewProjection())));

	g_pperFrameCB->Update(perFrameCB, g_pImmediateContext);

	//Bind per frame CB
	ID3D11Buffer* pbuffer = g_pperFrameCB->GetBuffer();

	g_pImmediateContext->VSSetConstantBuffers((int)GraphicsManager::CBOrders::PER_FRAME, 1, &pbuffer);
	g_pImmediateContext->PSSetConstantBuffers((int)GraphicsManager::CBOrders::PER_FRAME, 1, &pbuffer);

	g_ptestObject->Render(g_pImmediateContext, g_pperInstanceCB);

	// Present our back buffer to our front buffer
	g_pSwapChain->Present(0, 0);
}

void BindQuadBuffers()
{
	Mesh* pmesh = GraphicsManager::GetInstance()->GetMesh(QUAD_MESH_NAME);

	ID3D11Buffer* pindexBuffer = pmesh->GetIndexBuffer();
	ID3D11Buffer* pvertexBuffer = pmesh->GetVertexBuffer();

	UINT stride = pmesh->GetVBStride();
	UINT offset = pmesh->GetVBOffset();

	g_pImmediateContext->IASetVertexBuffers(0, 1, &pvertexBuffer, &stride, &offset);
	g_pImmediateContext->IASetIndexBuffer(pmesh->GetIndexBuffer(), DXGI_FORMAT_R16_UINT, 0);
}
