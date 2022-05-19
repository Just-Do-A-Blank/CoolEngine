#pragma once

#include <io.h>
#include <fcntl.h>

#include "Engine/Managers/GraphicsManager.h"
#include "Engine/Managers/AudioManager.h"
#include "Engine/Graphics/Mesh.h"
#include "Engine/Graphics/ConstantBuffer.h"
#include "Engine/Graphics/SpriteAnimation.h"
#include "Engine/GameObjects/CameraGameObject.h"
#include "Engine/GameObjects/EnemyGameObject.h"
#include "Engine/GameObjects/WeaponGameObject.h"

#include "Engine/Managers/Events/EventManager.h"
#include "Engine/Managers/Events/EventObserver.h"
#include "Engine/Helpers/Inputs.h"
#include "Engine/Includes/DirectXTK/SpriteBatch.h"

#include "Engine/EditorUI/EditorUI.h"

#include "FileIO/FileIO.h"

#include "Engine/TileMap/TileMap/TileMap.h"
#include "Engine/AI/Pathfinding.h"
#include "Engine/ResourceDefines.h"
#include "Managers/DebugDrawManager.h"
#include "Scene/Scene.h"
#include "Engine/Managers/GameManager.h"
#include <Engine/Physics/Box.h>
#include "Engine/GameUI/GameUIComponent.h"
#include "Engine/GameUI/ImageComponent.h"
#include "Engine/GameUI/TextComponent.h"

#include "Physics/ParticleManager.h"
#include "Engine/Managers/FontManager.h"

#include "Engine/Tools/ToolBase.h"
#include "Engine/Tools/TileMapTool.h"
#include "Engine/Tools/AnimationTool.h"
#include "Engine/GameObjects/EditorCameraGameObject.h"

#include "Engine/Managers/Events/EventObserverExamples.h"
#include "Engine/Managers/Events/DamageCalculation.h"
#include "Engine/Managers/Events/BulletCreator.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HRESULT	InitWindow(HINSTANCE hInstance, int nCmdShow);
HRESULT	InitDevice();
void CleanupDevice();

void Render();
void Update();

void BindQuadBuffers();


HINSTANCE g_hInstance;

HWND g_hWnd;

D3D_DRIVER_TYPE g_driverType = D3D_DRIVER_TYPE_NULL;
D3D_FEATURE_LEVEL g_featureLevel = D3D_FEATURE_LEVEL_11_0;
ID3D11Device* g_pd3dDevice = nullptr;
ID3D11DeviceContext* g_pImmediateContext = nullptr;
IDXGISwapChain* g_pSwapChain = nullptr;
ID3D11RenderTargetView* g_pRenderTargetView = nullptr;
ID3D11ShaderResourceView* g_pRenderToTexture = nullptr;
ID3D11RasterizerState* g_prasterState = nullptr;


ID3D11Texture2D* g_pRTTRrenderTargetTexture;
ID3D11RenderTargetView* g_pRTTRenderTargetView;
ID3D11ShaderResourceView* g_pRTTShaderResourceView;



EditorCameraGameObject* g_pcamera = nullptr;
PlayerGameObject* g_pplayer = nullptr;

TileMap* g_testMap1;

TileMap* g_testMap2;

#if EDITOR
EditorUI* g_peditorUI;
ToolBase* g_ptoolBase = nullptr;
#endif



using namespace DirectX;

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

	GameManager::GetInstance()->Init();


	//Setup audio stuff
	AudioManager::GetInstance()->Init();

	AudioManager::GetInstance()->SetListenerPosition(XMFLOAT3(0, 0, 0));

	GraphicsManager::GetInstance()->Init(g_pd3dDevice, g_pImmediateContext);
	GraphicsManager::GetInstance()->SetHWND(&g_hWnd);

#if EDITOR
	g_peditorUI = new EditorUI(g_pd3dDevice);
	g_peditorUI->InitIMGUI(g_pImmediateContext, g_pd3dDevice, &g_hWnd);
#endif

	Inputs::GetInstance();

	//Debug Manager
#if _DEBUG
	DebugDrawManager::GetInstance()->Init(g_pd3dDevice);


#endif

	srand(time(0));

	//Create camera
	XMFLOAT3 cameraPos = XMFLOAT3(0, 0, -5);
	XMFLOAT3 cameraForward = XMFLOAT3(0, 0, 1);
	XMFLOAT3 cameraUp = XMFLOAT3(0, 1, 0);

	float windowWidth = GraphicsManager::GetInstance()->GetWindowDimensions().x;
	float windowHeight = GraphicsManager::GetInstance()->GetWindowDimensions().y;

	float nearDepth = 0.01f;
	float farDepth = 1000.0f;

	CoolUUID uuid;
	g_pcamera = new EditorCameraGameObject(std::string("Camera"), uuid);
	g_pcamera->Initialize(cameraPos, cameraForward, cameraUp, windowWidth, windowHeight, nearDepth, farDepth);

	GameManager::GetInstance()->SetCamera(g_pcamera);
	FontManager::GetInstance()->LoadFont(L"Resources\\Fonts\\ComicSans", "comicSans");

	//Create scene
	GameManager* pgameManager = GameManager::GetInstance();
	pgameManager->CreateScene("TestScene");
	pgameManager->SelectSceneUsingIdentifier("TestScene");
	pgameManager->SelectSceneUsingIdentifier("TestScene");

	//Music
	AudioManager::GetInstance()->LoadMusic(TEST_MUSIC);

	AudioManager::GetInstance()->PlayMusic(TEST_MUSIC, 0.001f, true);

	//Sound
	AudioManager::GetInstance()->Load(TEST_SOUND);

	AudioManager::GetInstance()->Play(TEST_SOUND, 0.01f);

	GraphicsManager::GetInstance()->LoadTextureFromFile(DEFAULT_IMGUI_IMAGE);
	GraphicsManager::GetInstance()->LoadTextureFromFile(TEST2);

	//Load animations

	//Create test gameobject
	string obj0Name = "TestObject0";
	string obj1Name = "TestObject1";
	string playerName = "Player";
	string enemyName = "Enemy";
	string weaponName = "Weapon";

	pgameManager->CreateGameObject<RenderableCollidableGameObject>(obj0Name);
	pgameManager->CreateGameObject<RenderableCollidableGameObject>(obj1Name);
	pgameManager->CreateGameObject<PlayerGameObject>(playerName);
	pgameManager->CreateGameObject<EnemyGameObject>(enemyName);
	pgameManager->CreateGameObject<RangedWeaponGameObject>(weaponName);

	RenderableCollidableGameObject* pgameObject = pgameManager->GetGameObjectUsingIdentifier<RenderableCollidableGameObject>(obj0Name);

	XMFLOAT3 objectPos = XMFLOAT3(0, -200.0f, 0.0f);
	XMFLOAT3 objectScale = XMFLOAT3(2, 2, 2);
	XMFLOAT3 objectRot = XMFLOAT3(0, 0, 0);
	bool isCollision = true;

	pgameObject->SetMesh(QUAD_MESH_NAME);
	pgameObject->SetVertexShader(DEFAULT_VERTEX_SHADER_NAME);
	pgameObject->SetPixelShader(DEFAULT_PIXEL_SHADER_NAME);
	pgameObject->SetAlbedo(DEFAULT_IMGUI_IMAGE);
	pgameObject->GetTransform()->SetWorldPosition(objectPos);
	pgameObject->GetTransform()->SetWorldScale(objectScale);
	Box* pbox = new Box(pgameObject->GetTransform());
	pbox->SetIsCollidable(isCollision);
	pbox->SetIsTrigger(isCollision);
	pgameObject->SetShape(pbox);

	////Init second gameObject
	pgameObject = pgameManager->GetGameObjectUsingIdentifier<RenderableCollidableGameObject>(obj1Name);

	objectPos = XMFLOAT3(10.0f, -200.0f, 0.0f);
	objectScale = XMFLOAT3(2, 2, 2);

	pgameObject->SetMesh(QUAD_MESH_NAME);
	pgameObject->SetVertexShader(DEFAULT_VERTEX_SHADER_NAME);
	pgameObject->SetPixelShader(DEFAULT_PIXEL_SHADER_NAME);
	pgameObject->SetAlbedo(TEST2);
	pgameObject->GetTransform()->SetWorldPosition(objectPos);
	pgameObject->GetTransform()->SetWorldScale(objectScale);
	pgameObject->GetTransform()->SetWorldRotation(objectRot);
	pbox = new Box(pgameObject->GetTransform());
	pbox->SetIsCollidable(isCollision);
	pbox->SetIsTrigger(isCollision);
	pgameObject->SetShape(pbox);

	//Init enemy object
	pgameObject = pgameManager->GetGameObjectUsingIdentifier<EnemyGameObject>(enemyName);
	objectPos = XMFLOAT3(-570, -25.0f, 0);
	objectScale = XMFLOAT3(0.8f, 0.8f, 0.8f);

	pgameObject->SetMesh(QUAD_MESH_NAME);
	pgameObject->SetVertexShader(DEFAULT_VERTEX_SHADER_NAME);
	pgameObject->SetPixelShader(DEFAULT_PIXEL_SHADER_NAME);
	pgameObject->SetAlbedo(DEFAULT_IMGUI_IMAGE);
	pgameObject->GetTransform()->SetWorldPosition(objectPos);
	pgameObject->GetTransform()->SetWorldScale(objectScale);
	pbox = new Box(pgameObject->GetTransform());
	pbox->SetIsCollidable(isCollision);
	pbox->SetIsTrigger(isCollision);
	pgameObject->SetShape(pbox);

	// Init player object
	pgameObject = pgameManager->GetGameObjectUsingIdentifier<PlayerGameObject>(playerName);

	objectPos = XMFLOAT3(200.0f, -200.0f, 5.0f);
	objectScale = XMFLOAT3(50, 50, 50);

	pgameObject->SetMesh(QUAD_MESH_NAME);
	pgameObject->SetVertexShader(DEFAULT_VERTEX_SHADER_NAME);
	pgameObject->SetPixelShader(DEFAULT_PIXEL_SHADER_NAME);
	pgameObject->SetAlbedo(DEFAULT_IMGUI_IMAGE);
	pgameObject->GetTransform()->SetWorldPosition(objectPos);
	pgameObject->GetTransform()->SetWorldScale(objectScale);
	pbox = new Box(pgameObject->GetTransform());
	pbox->SetIsCollidable(isCollision);
	pbox->SetIsTrigger(isCollision);
	pgameObject->SetShape(pbox);


	// Weapon test
	pgameObject = pgameManager->GetGameObjectUsingIdentifier<RangedWeaponGameObject>(weaponName);
	pgameObject->SetMesh(QUAD_MESH_NAME);
	pgameObject->SetVertexShader(DEFAULT_VERTEX_SHADER_NAME);
	pgameObject->SetPixelShader(DEFAULT_PIXEL_SHADER_NAME);
	pgameObject->SetAlbedo(DEFAULT_IMGUI_IMAGE);
	objectPos = XMFLOAT3(200.0f, 0.0f, 5.0f);
	objectScale = XMFLOAT3(25, 25, 25);
	pgameObject->GetTransform()->SetWorldPosition(objectPos);
	pgameObject->GetTransform()->SetWorldScale(objectScale);
	pbox = new Box(pgameObject->GetTransform());
	pbox->SetIsTrigger(isCollision);
	isCollision = false;
	pbox->SetIsCollidable(isCollision);
	pgameObject->SetShape(pbox);

	//g_testMap1 = new TileMap(TEST_MAP, XMFLOAT3(-500, -200, 0), "TestMap");

	//Pathfinding::GetInstance()->Initialize(g_testMap1);

	// Observer for collision detection
	CollisionObserver collisionObserver = CollisionObserver();

	// Observer for taking damage
	DamageCalculation damageObserver = DamageCalculation();

	// Observer for making attacks
	BulletCreator bulletCreator = BulletCreator();
	

	XMFLOAT3 pos = XMFLOAT3(-400, 250, 5);
	XMFLOAT3 rot = XMFLOAT3(0, 0, 0);
	XMFLOAT3 scale = XMFLOAT3(0.5f, 0.5f, 0.5f);
	Transform trans = Transform();
	trans.SetWorldPosition(pos);
	trans.SetWorldRotation(rot);
	trans.SetWorldScale(scale);
	ParticleManager::GetInstance()->AddSystem(trans, 1000.0f, DEFAULT_IMGUI_IMAGE, { 0,0 }, { 0,0 }, 1.0f, 0.2f, 3, 20, 90.0f, 0.0f, 0.2f, 0);

	pos = XMFLOAT3(0, 250, 5);
	rot = XMFLOAT3(0, 0, 0);
	scale = XMFLOAT3(0.5f, 0.5f, 0.5f);
	trans.SetWorldPosition(pos);
	trans.SetWorldRotation(rot);
	trans.SetWorldScale(scale);
	ParticleManager::GetInstance()->AddSystem(trans, 1000.0f, DEFAULT_IMGUI_IMAGE, { 0,0 }, { 0,0 }, 0.5f, 1.0f, 16, 100.0f, 0.0f, 0.0f, 0.2f, 2);

	pos = XMFLOAT3(400, 250, 5);
	rot = XMFLOAT3(0, 0, 0);
	scale = XMFLOAT3(0.5f, 0.5f, 0.5f);
	trans.SetWorldPosition(pos);
	trans.SetWorldRotation(rot);
	trans.SetWorldScale(scale);
	ParticleManager::GetInstance()->AddSystem(trans, 1000.0f, DEFAULT_IMGUI_IMAGE, { -100,150 }, { 300,-75 }, 2.0f, 0.25f, 3, 100.0f, 25.0f, 25.0f, 0.1f, 1);

	GameManager::GetInstance()->GetTimer()->Tick();
	GameManager::GetInstance()->GetTimer()->Tick();

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
			Update();
			Render();
		}
	}

#if EDITOR
	g_peditorUI->ShutdownIMGUI();
#endif

	CleanupDevice();

	return (int)msg.wParam;
}

#if EDITOR
// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
#endif

//--------------------------------------------------------------------------------------
// Called every time the application receives a message
//--------------------------------------------------------------------------------------
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
#if EDITOR
	if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
	{
		return true;
	}
#endif

	Inputs::GetInstance()->Update(&hWnd, &message, &wParam, &lParam);

	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
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
	RECT rc = { 0, 0, GraphicsManager::GetInstance()->GetWindowDimensions().x, GraphicsManager::GetInstance()->GetWindowDimensions().y };

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

	GraphicsManager::GetInstance()->SetWindowDimensions(XMFLOAT2(width, height));

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
		{
			break;
		}
	}

	if (FAILED(hr))
	{
		return hr;
	}

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

	D3D11_TEXTURE2D_DESC textureDesc;
	ZeroMemory(&textureDesc, sizeof(textureDesc));
	textureDesc.Width = width;
	textureDesc.Height = height;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;
	hr = g_pd3dDevice->CreateTexture2D(&textureDesc, NULL, &g_pRTTRrenderTargetTexture);

	if (FAILED(hr))
		return hr;

	// Setup the description of the render target view.
	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
	renderTargetViewDesc.Format = textureDesc.Format;
	renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	renderTargetViewDesc.Texture2D.MipSlice = 0;

	hr = g_pd3dDevice->CreateRenderTargetView(g_pRTTRrenderTargetTexture, &renderTargetViewDesc, &g_pRTTRenderTargetView);

	if (FAILED(hr))
		return hr;

	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
	shaderResourceViewDesc.Format = textureDesc.Format;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	shaderResourceViewDesc.Texture2D.MipLevels = 1;

	// Create the shader resource view.
	hr = g_pd3dDevice->CreateShaderResourceView(g_pRTTRrenderTargetTexture, &shaderResourceViewDesc, &g_pRTTShaderResourceView);

	if (FAILED(hr))
		return hr;

	g_pImmediateContext->OMSetRenderTargets(1, &g_pRenderTargetView, nullptr);

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

void RecreateFrameResources(XMFLOAT2 dimensions)
{
	g_pRTTRrenderTargetTexture->Release();
	g_pRTTRenderTargetView->Release();
	g_pRTTShaderResourceView->Release();

	D3D11_TEXTURE2D_DESC textureDesc;
	ZeroMemory(&textureDesc, sizeof(textureDesc));
	textureDesc.Width = dimensions.x;
	textureDesc.Height = dimensions.y;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;
	HRESULT hr = g_pd3dDevice->CreateTexture2D(&textureDesc, NULL, &g_pRTTRrenderTargetTexture);

	if (FAILED(hr))
	{
		LOG("Failed to create viewport texture!");

		return;
	}

	// Setup the description of the render target view.
	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
	renderTargetViewDesc.Format = textureDesc.Format;
	renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	renderTargetViewDesc.Texture2D.MipSlice = 0;

	hr = g_pd3dDevice->CreateRenderTargetView(g_pRTTRrenderTargetTexture, &renderTargetViewDesc, &g_pRTTRenderTargetView);

	if (FAILED(hr))
	{
		LOG("Failed to create view to viewport texture!");

		return;
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
	shaderResourceViewDesc.Format = textureDesc.Format;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	shaderResourceViewDesc.Texture2D.MipLevels = 1;

	// Create the shader resource view.
	hr = g_pd3dDevice->CreateShaderResourceView(g_pRTTRrenderTargetTexture, &shaderResourceViewDesc, &g_pRTTShaderResourceView);

	if (FAILED(hr))
	{
		LOG("Failed to create shader resource view to viewport texture!");

		return;
	}
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
#if EDITOR
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	//Create Dockspace
	ImGui::DockSpaceOverViewport();
#endif

	// Clear the back buffer
	g_pImmediateContext->ClearRenderTargetView(g_pRenderTargetView, DirectX::Colors::MidnightBlue);

	//Set current render target to render to texture target.
	g_pImmediateContext->OMSetRenderTargets(1, &g_pRTTRenderTargetView, nullptr);
	g_pImmediateContext->ClearRenderTargetView(g_pRTTRenderTargetView, DirectX::Colors::MidnightBlue);

	g_pImmediateContext->IASetInputLayout(GraphicsManager::GetInstance()->GetInputLayout(GraphicsManager::InputLayouts::POS_TEX));

	BindQuadBuffers();

	//Bind sampler
	ID3D11SamplerState* psampler = GraphicsManager::GetInstance()->GetSampler(GraphicsManager::Samplers::LINEAR_WRAP);

	g_pImmediateContext->PSSetSamplers(0, 1, &psampler);

	//Update per frame CB
	PerFrameCB perFrameCB;
	XMStoreFloat4x4(&perFrameCB.viewProjection, XMMatrixTranspose(XMLoadFloat4x4(&g_pcamera->GetViewProjection())));

	GraphicsManager::GetInstance()->m_pperFrameCB->Update(perFrameCB, g_pImmediateContext);

	//Bind per frame CB
	ID3D11Buffer* pbuffer = GraphicsManager::GetInstance()->m_pperFrameCB->GetBuffer();

	g_pImmediateContext->VSSetConstantBuffers((int)GraphicsManager::CBOrders::PER_FRAME, 1, &pbuffer);
	g_pImmediateContext->PSSetConstantBuffers((int)GraphicsManager::CBOrders::PER_FRAME, 1, &pbuffer);

	RenderStruct renderStruct;
	renderStruct.m_pcontext = g_pImmediateContext;

	for (int i = 0; i < GraphicsManager::GetInstance()->GetNumLayers(); ++i)
	{
		GraphicsManager::GetInstance()->GetSpriteBatches()[i]->Begin(SpriteSortMode_Deferred, nullptr, nullptr, nullptr, nullptr, [=]
		{
				ID3D11Buffer* pbuffer = GraphicsManager::GetInstance()->m_pperFrameCB->GetBuffer();

				D3D11_VIEWPORT viewport;
				viewport.TopLeftX = 0;
				viewport.TopLeftY = 0;
				viewport.Width = EditorUI::GetViewportSize().x;
				viewport.Height = EditorUI::GetViewportSize().y;
				viewport.MinDepth = 0.01f;
				viewport.MaxDepth = 1.0f;

				g_pImmediateContext->RSSetViewports(1, &viewport);
				g_pImmediateContext->RSSetState(g_prasterState);
				g_pImmediateContext->IASetInputLayout(GraphicsManager::GetInstance()->GetInputLayout(GraphicsManager::InputLayouts::POS_TEX_COLOR));

				g_pImmediateContext->VSSetShader(GraphicsManager::GetInstance()->GetVertexShader(SPRITE_BATCH_VERTEX_SHADER_NAME), nullptr, 0);
				g_pImmediateContext->PSSetShader(GraphicsManager::GetInstance()->GetPixelShader(SPRITE_BATCH_PIXEL_SHADER_NAME), nullptr, 0);

				g_pImmediateContext->VSSetConstantBuffers((int)GraphicsManager::CBOrders::PER_FRAME, 1, &pbuffer);
				g_pImmediateContext->PSSetConstantBuffers((int)GraphicsManager::CBOrders::PER_FRAME, 1, &pbuffer);
		});
	}

	GameManager* pgamemanager = GameManager::GetInstance();
	pgamemanager->Render(renderStruct);

	ParticleManager::GetInstance()->Render(renderStruct.m_pcontext);

#if _DEBUG
	DebugDrawManager::GetInstance()->Render(renderStruct);
#endif

	for (int i = 0; i < GraphicsManager::GetInstance()->GetNumLayers(); ++i)
	{
		GraphicsManager::GetInstance()->GetSpriteBatches()[i]->End();
	}

	if (g_ptoolBase != nullptr)
	{
		g_ptoolBase->Render();
	}
	else
	{
#if EDITOR
		g_peditorUI->DrawEditorUI(g_pd3dDevice, g_ptoolBase);
#endif
	}

#if EDITOR
	ImGuiWindowFlags viewportWindowFlags = 0;
	viewportWindowFlags |= ImGuiWindowFlags_HorizontalScrollbar;
	ImGui::Begin("Viewport", nullptr, viewportWindowFlags);

	ImVec2 viewportPos = ImGui::GetCursorScreenPos();
	EditorUI::SetViewportPosition(DirectX::XMFLOAT2(viewportPos.x, viewportPos.y));

	ImVec2 newViewportSize = ImGui::GetContentRegionAvail();
	XMFLOAT2 oldViewportSize = EditorUI::GetViewportSize();

	if ((oldViewportSize.x != newViewportSize.x || oldViewportSize.y != newViewportSize.y) && (newViewportSize.x > 0 && newViewportSize.y > 0))
	{
		XMFLOAT2 viewportSize = XMFLOAT2(newViewportSize.x, newViewportSize.y);

		RecreateFrameResources(viewportSize);

		EditorUI::SetViewportSize(viewportSize);
	}

	//Pass everything rendered till this point into ImGuiImage

	XMFLOAT2 dimension = GraphicsManager::GetInstance()->GetWindowDimensions();
	ImGui::Image(g_pRTTShaderResourceView, newViewportSize);

	EditorUI::SetIsViewportHovered(ImGui::IsItemHovered());

	ImGui::End();



	//Swap render target to back buffer
	g_pImmediateContext->OMSetRenderTargets(1, &g_pRenderTargetView, nullptr);
	g_pImmediateContext->ClearRenderTargetView(g_pRenderTargetView, DirectX::Colors::MidnightBlue);
	ImGui::Render();

	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	// Update and Render additional Platform Windows
	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
	}

	ImGui::EndFrame();
#endif

	// Present our back buffer to our front buffer
	g_pSwapChain->Present(0, 0);
}

float temp;

void Update()
{
#if _DEBUG
	DebugDrawManager::GetInstance()->Update();
#endif

	ParticleManager::GetInstance()->Update(GameManager::GetInstance()->GetTimer()->DeltaTime());

	AudioManager::GetInstance()->Update();

	Inputs::GetInstance()->Update();
	EventManager::Instance()->ProcessEvents();

	GameManager* pgamemanager = GameManager::GetInstance();

	pgamemanager->GetTimer()->Tick();
	pgamemanager->Update();

#if EDITOR
	g_peditorUI->Update();
	g_pcamera->Update();
#endif

	if (g_ptoolBase != nullptr)
	{
		if (g_ptoolBase->HasToolExited() == true)
		{
			delete g_ptoolBase;
			g_ptoolBase = nullptr;
		}
		else
		{
			g_ptoolBase->Update();
		}
	}
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
