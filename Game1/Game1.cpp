﻿// Game1.cpp : アプリケーションのエントリ ポイントを定義します。
//

#include "framework.h"
#include "Game1.h"
#include <atlstr.h>
#include <strstream>
#include <graphics/DirectX.h>
#include <scene/Scene.h>
#include <graphics/TextureManager.h>
#include <graphics/SpriteRenderer.h>
#include <graphics/UIRenderer.h>
#include <hid/Input.h>
#include <common/Singleton.h>
#include <audio/dx/Audio.h>
#include <common/GameObject.h>
#include "scene/Title.h"
#include "scene/CommonData.h"

// for debug
#include "scene/Game.h"
#include "scene/Stage2.h"
#include "scene/Stage3.h"
#include "scene/Stage4.h"
#include "scene/Stage5.h"
#include "scene/Clear.h"

#define MAX_LOADSTRING 100

// グローバル変数:
HINSTANCE hInst;                                // 現在のインターフェイス
WCHAR szTitle[MAX_LOADSTRING];                  // タイトル バーのテキスト
WCHAR szWindowClass[MAX_LOADSTRING];            // メイン ウィンドウ クラス名

// このコード モジュールに含まれる関数の宣言を転送します:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

// 以下自作関数
void AppInitialize();
void AppProcess();
void Update();
void Draw();
void Finalize();

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: ここにコードを挿入してください。

    // グローバル文字列を初期化する
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_GAME1, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // アプリケーション初期化の実行:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }
    AppInitialize();

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_GAME1));

    // メインのメッセージループ:
    MSG msg = {};

    // メイン メッセージ ループ:
    while (msg.message != WM_QUIT)
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else {
            AppProcess();
        }
    }

    Finalize();

    return (int) msg.wParam;
}



//
//  関数: MyRegisterClass()
//
//  目的: ウィンドウ クラスを登録します。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_GAME1));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_GAME1);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   関数: InitInstance(HINSTANCE, int)
//
//   目的: インスタンス ハンドルを保存して、メイン ウィンドウを作成します
//
//   コメント:
//
//        この関数で、グローバル変数でインスタンス ハンドルを保存し、
//        メイン プログラム ウィンドウを作成および表示します。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // グローバル変数にインスタンス ハンドルを格納する

   HWND hWnd = CreateWindow(
       szWindowClass,
       szTitle,
       WS_OVERLAPPEDWINDOW,
       CW_USEDEFAULT, CW_USEDEFAULT,
       DISPLAY_WIDTH, DISPLAY_HEIGHT,
       NULL,
       NULL,
       hInstance,
       NULL
   );

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   using Hirai::DirectX;
   using Hirai::Input;
   if (!DirectX::Initialize(hWnd)) {
       MessageBox(NULL,
           _T("DirectX関連の初期化に失敗しました。"),
           _T("Error"),
           NULL);
       return 1;
   }
   if (!Input::InitializeForMouse(hWnd)) {
       MessageBox(NULL,
           _T("インプットデバイスの初期化に失敗しました。"),
           _T("Error"),
           NULL);
       return 1;
   }

   // サウンド
   DirectAudio& dAudio = singleton<DirectAudio>::GetInstance();
   dAudio.Initialize(hWnd);

   return TRUE;
}

//
//  関数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的: メイン ウィンドウのメッセージを処理します。
//
//  WM_COMMAND  - アプリケーション メニューの処理
//  WM_PAINT    - メイン ウィンドウを描画する
//  WM_DESTROY  - 中止メッセージを表示して戻る
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    std::ostrstream oss;

    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 選択されたメニューの解析:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            case IDM_HOWTOPLAY:
                oss << "（操作方法）\n";
                oss << "左クリック：決定\n";
                oss << "マウス：カプセルを動かす\n\n";

                oss << "（ルール）\n";
                oss << "ウイルスが増殖してくるので、人に感染しないように\n";
                oss << "マウスでカプセルを動かして、ウイルスを消していきます。\n";
                oss << "人が家にたどり着くまでウイルスに感染しなかったらクリアです。\n";
                oss << std::ends;
                if (MessageBox(hWnd, CString(oss.str()), CString("遊び方"), MB_OK) == IDOK) {
                    return 0;
                }
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: HDC を使用する描画コードをここに追加してください...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_KEYDOWN:
        switch (wParam)
        {
        case VK_ESCAPE:
            //終了メッセージを発生させる
            PostMessage(hWnd, WM_CLOSE, 0, 0);
            //PostQuitMessage(0);
            break;
        case VK_RETURN:
            //PostMessage(hWnd, WM_ACTIVATE, 0, 0);
            break;
        default:
            break;
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// バージョン情報ボックスのメッセージ ハンドラーです。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

void AppInitialize()
{
    // 音源読み込み
    DirectAudio& dAudio = singleton<DirectAudio>::GetInstance();
    dAudio.LoadAudio("sound/bgm.wav", "BGM");
    //dAudio.LoadAudio("sound/levelDisp.wav", "LevelDisp");
    dAudio.LoadAudio("sound/decision.wav", "Decision");
    dAudio.LoadAudio("sound/gameClear.wav", "GameClear");
    dAudio.LoadAudio("sound/miss.wav", "Miss");
    dAudio.LoadAudio("sound/stageClear.wav", "StageClear");

    Scene::InitializeScene();
    Scene::SetScene(new Title);
    //Scene::SetScene(new Clear);
    CommonData::SetCurrentStage(1);
    
    SpriteRenderer::Initialize();
    SpriteRenderer::SetScreenSize(DISPLAY_WIDTH, DISPLAY_HEIGHT);
    UIRenderer::Initialize();
    UIRenderer::SetScreenSize(DISPLAY_WIDTH, DISPLAY_HEIGHT);

}

void AppProcess()
{
    Update();
    Draw();
}

void Update()
{
    using Hirai::DirectX;
    Scene::UpdateScene();
}

void Draw() {
    using Hirai::DirectX;
    using Hirai::Input;
    Input::Update();
    DirectX::SetPresentDevice();
    Scene::DrawScene();
}

void Finalize() {
    TextureManager::ReleaseAll();
    UIRenderer::Finalize();
    SpriteRenderer::Finalize();

    using Hirai::DirectX;
    using Hirai::Input;
    Input::Finalize();
    DirectX::FreeDx();
}
