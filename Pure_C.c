﻿// Pure_C.c : アプリケーションのエントリ ポイントを定義します。
//

#include "framework.h"
#include "Pure_C.h"

#define MAX_LOADSTRING 100



// グローバル変数:
HINSTANCE hInst;                                // 現在のインターフェイス
WCHAR szTitle[MAX_LOADSTRING];                  // タイトル バーのテキスト
WCHAR szWindowClass[MAX_LOADSTRING];            // メイン ウィンドウ クラス名
GRAPH graph = { 0, 0, 4, 200 };


// このコード モジュールに含まれる関数の宣言を転送します:

ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    MenuImport(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    MenuExport(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
LPDWORD             ColorAt(UINT, UINT, UINT, UINT);
int                 Calc(UINT, UINT, UINT, UINT);

int APIENTRY wWinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow
)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: ここにコードを挿入してください。

    // グローバル文字列を初期化する
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_PUREC, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // アプリケーション初期化の実行:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    // アクセラレータテーブルのハンドル
    // アクセラレータテーブル :
    //     特別なキーの組み合わせと発生させるIDを組み合わせたもの
    //     メニューの操作に用いる
    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_PUREC));

    MSG msg;

    // メイン メッセージ ループ:
    // メッセージの取得(メッセージが取得できるまで待機する)
    while (GetMessage(&msg, NULL, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);// 仮想キーコードを文字に変換する
            DispatchMessage(&msg);// ウィンドウプロシージャにメッセージを渡す
        }
    }

    return (int) msg.wParam;
}



//
//  関数: MyRegisterClass()
//
//  目的: ウィンドウ クラスを登録します。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    // ウィンドウクラスのパラメータ
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);// 構造体のサイズ

    wcex.style          = CS_HREDRAW | CS_VREDRAW;// ウィンドウスタイル
    wcex.lpfnWndProc    = WndProc;// ウィンドウプロシージャ
    wcex.cbClsExtra     = 0;// ウィンドウクラス構造体の後ろに割り当てる補足バイト数
    wcex.cbWndExtra     = 0;// ウィンドウインスタンスの後ろに割り当てる補足バイト数
    wcex.hInstance      = hInstance;// このクラスのためのウィンドウプロシージャがあるインスタンスハンドル
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_PUREC));// アイコンのハンドル
    wcex.hCursor        = LoadCursor(NULL, IDC_ARROW);// マウスカーソルのハンドル
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);// 背景描画用ブラシのハンドル
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_PUREC);// デフォルトメニュー名
    wcex.lpszClassName  = szWindowClass;// このウィンドウクラスにつける名前
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));// 16x16の小さいサイズのアイコン

    // ウィンドウクラスの登録
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

   HWND hWnd = CreateWindowW(
       szWindowClass,       // lpWindowName : ウィンドウクラスの名前
       szTitle,             // lpWindowName : ウィンドウの名前
       WS_OVERLAPPEDWINDOW, // dwStyle      : ウィンドウのスタイル
       CW_USEDEFAULT,       // x            : 初期位置
       0,                   // y
       CW_USEDEFAULT,       // nWidth       : ウィンドウ幅
       0,                   // nHeight
       NULL,                // hWndParent   : 親ウィンドウ
       NULL,                // hMenu        : メニューのハンドル
       hInstance,           // hInstance    : インスタンスハンドル
       NULL                 // lpParam      : メッセージに渡されるパラメータ
   );

   if (!hWnd)
   {
       // ウィンドウ生成に失敗したとき，FALSEを返す
       return FALSE;
   }

   // ウィンドウの表示
   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//d
//  関数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的: メイン ウィンドウのメッセージを処理します。
//
//  WM_COMMAND  - アプリケーション メニューの処理
//  WM_SIZE     - リサイズ時の処理
//  WM_PAINT    - メイン ウィンドウを描画する
//  WM_DESTROY  - 中止メッセージを表示して戻る
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static UINT width = 0, height = 0;

    HDC hdc;// デバイスコンテキストのハンドル
    PAINTSTRUCT ps;
    static LPDWORD lpPixel;
    static BITMAPINFO bmpInfo;
    UINT x, y;
    static int mx = -100, my = -100;
    BOOL m_in = FALSE;
    UINT mlen = min(width, height);


    switch (message)
    {
    case WM_CREATE:
    {
        width = GetSystemMetrics(SM_CXSCREEN);
        height = GetSystemMetrics(SM_CYSCREEN);
        mlen = min(width, height);
        lpPixel = (LPDWORD)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, width * height * 4);
        // DIBの情報を設定する
        bmpInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        bmpInfo.bmiHeader.biPlanes = 1;
        bmpInfo.bmiHeader.biBitCount = 32;
        bmpInfo.bmiHeader.biCompression = BI_RGB;
    }
    case WM_SIZE:
    {
        if (message == WM_SIZE) {
            mlen = min(width, height);
            width = LOWORD(lParam);
            height = HIWORD(lParam);
        }
    }
    case WM_LBUTTONDOWN:
    {
        if (message == WM_LBUTTONDOWN) {
            graph.x0 += (mx - (long double)width / 2) / mlen * graph.size;
            graph.y0 -= (my - (long double)height / 2) / mlen * graph.size;
            graph.size *= 0.5;
        }
        bmpInfo.bmiHeader.biWidth = width;
        bmpInfo.bmiHeader.biHeight = height;
        // 裏画面への描画
        for (y = 0; y < height; y++) {
            for (x = 0; x < width; x++) {
                lpPixel[x + y * width] = ColorAt(x, y, width, height);
            }
        }
        InvalidateRect(hWnd, NULL, FALSE);
        break;
        /*
        PAINTSTRUCT ps;//
        HDC hdc = BeginPaint(hWnd, &ps);// デバイスコンテキストのハンドル
        // TODO: HDC を使用する描画コードをここに追加してください...
        //TextOut(hdc, 10, 10, L"sample text", lstrlen(L"sample text"));
        UINT x, y;
        InvalidateRect(hWnd, NULL, TRUE);
        for (y = 0; y < height; y++) {
            for (x = 0; x < width; x++) {
                //cells[y][x] = ColorAt(x, y, width, height);
                SetPixel(hdc, x, y, ColorAt(x, y, width, height));
            }
        }


        wsprintf(strSize, L"width = %d : height = %d", width, height);
        TextOut(hdc, 10, 10, strSize, lstrlen(strSize));
        */
        /*
        RECT rect;
        GetClientRect(hWnd, &rect);
        SelectObject(hdc, GetStockObject(GRAY_BRUSH));
        Rectangle(hdc, 10, 10, 100, 100);
        SelectObject(hdc, GetStockObject(BLACK_BRUSH));
        SetTextColor(hdc, RGB(255, 0, 0));
        DrawText(
            hdc,
            TEXT("画面幅に合わせて改行を入れるテキスト描画のテスト"),
            -1,
            &rect,
            DT_CENTER | DT_VCENTER | DT_SINGLELINE // | DT_WORDBREAK //
        );
        */
        /*EndPaint(hWnd, &ps);
        break;*/
    }
    case WM_PAINT:
    {
        hdc = BeginPaint(hWnd, &ps);
        // 表画面へ転送
        SetDIBitsToDevice(hdc, 0, 0, width, height, 0, 0, 0, height, lpPixel, &bmpInfo, DIB_RGB_COLORS);
        StretchDIBits(hdc, 0, 0, width, height, 0, 0, width, height, lpPixel, &bmpInfo, DIB_RGB_COLORS, SRCCOPY);
        //TCHAR str[128];
        //wsprintf(str, L"width: %d, height: %d", width, height);
        //TextOut(hdc, 10, 10, str, lstrlen(str));
        //wsprintf(str, L"mouseX: %d, mouseY: %d", mx, my);
        //TextOut(hdc, 10, 30, str, lstrlen(str));
        if (m_in) {
            SelectObject(hdc, GetStockObject(WHITE_BRUSH));
            Rectangle(hdc, mx - 10, my - 10, mx + 10, my + 10);
        }
        EndPaint(hWnd, &ps);
        break;
    }
    case WM_MOUSEHOVER:
    {
        m_in = TRUE;
        break;
    }
    case WM_MOUSELEAVE:
    {
        mx = -100;
        my = -100;
        m_in = FALSE;
        InvalidateRect(hWnd, NULL, FALSE);
        break;
    }
    case WM_MOUSEMOVE:
    {
        // WM_MOUSELEAVEの有効化
        TRACKMOUSEEVENT tme;
        tme.cbSize = sizeof(tme);
        tme.dwFlags = TME_LEAVE;
        tme.hwndTrack = hWnd;
        TrackMouseEvent(&tme);
        
        // マウスカーソル位置の取得
        POINTS mp = MAKEPOINTS(lParam);
        mx = mp.x;
        my = mp.y;
        InvalidateRect(hWnd, NULL, FALSE);
        break;
    }
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // 選択されたメニューの解析:
        switch (wmId)
        {
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        case IDM_IMPORT:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_IMBOX), hWnd, MenuImport);
            break;
        case IDM_EXPORT:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_EXBOX), hWnd, MenuExport);
            break;
        case IDM_ABOUT:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;
    case WM_DESTROY:
        HeapFree(GetProcessHeap(), 0, lpPixel);
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// メニュー 描画内容入力
INT_PTR CALLBACK MenuImport(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    TCHAR input[1024];// 入力内容保存用の変数

    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case IDCANCEL:
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        case IDC_IMBTN:
            // 入力の決定
            GetDlgItemText(hDlg, IDC_IMIPT, (LPTSTR)input, (int)sizeof(input));
            SetDlgItemText(hDlg, IDC_IMTXT, (LPCTSTR)input);
            //EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}


// メニュー 描画内容出力
INT_PTR CALLBACK MenuExport(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    HGLOBAL hg;
    CHAR *strMem;
    static LPCWSTR output[256] = {0x00};// 入力内容保存用の変数
    static UINT dataSize = 0;

    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        // 出力文字列を設定する
        GetGraphData(&graph, output, 256);

        dataSize = lstrlen(output);

        SetDlgItemText(hDlg, IDC_EXTXT1, output);
        dataSize = lstrlen(output);
        return (INT_PTR)TRUE;
    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case IDCANCEL:
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        case IDC_EXBTN:
        {
            // 文字列のコピー
            if (!OpenClipboard(hDlg)) {
                SetDlgItemText(hDlg, IDC_EXTXT2, TEXT("クリップボードを開くことができません"));
                return (INT_PTR)FALSE;
            }

            EmptyClipboard();
            hg = GlobalAlloc(GHND | GMEM_SHARE, dataSize * 2 + 1);
            if (!hg)return (INT_PTR)FALSE;
            strMem = (CHAR *)GlobalLock(hg);
            lstrcpy(strMem, output);
            GlobalUnlock(hg);
            SetClipboardData(CF_UNICODETEXT, hg);
            CloseClipboard();
            SetDlgItemText(hDlg, IDC_EXTXT2, TEXT("クリップボードにコピーしました"));
            //SetDlgItemText(hDlg, IDC_EXTXT2, strMem);
            //EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        }
        break;
    }
    return (INT_PTR)FALSE;
}

// バージョン情報ボックスのメッセージ ハンドラーです。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);// 未使用時エラーの抑制
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


// 色を返す
LPDWORD ColorAt(UINT x, UINT y, UINT width, UINT height)
{
    //if ((x - width / 2) * (x - width / 2) + (y - height / 2) * (y - height / 2) < 50)return 0x00ff0000;
    int t = Calc(x, y, width, height);
    if (t < 0) return (LPDWORD)0x00ffffff;
    return (LPDWORD)((t * 255 / graph.limit) * 0x00000100);
    //return (x + y) % 0x01000000;
    //return (0x01000000 - 1) * (x + y) / (width + height);
}

// 計算回数を返す
int Calc(UINT x, UINT y, UINT width, UINT height)
{
    UINT m = min(width, height);
    int i;
    long double zr = 0, zi = 0, tmp;
    long double cr = graph.x0 + (x - (long double)width / 2) / m * graph.size;
    long double ci = graph.y0 + (y - (long double)height / 2) / m * graph.size;
    for (i = 0; i < graph.limit; i++) {
        if (zr * zr + zi * zi > 4) return i;
        tmp = zr * zr - zi * zi + cr;
        zi = 2 * zr * zi + ci;
        zr = tmp;
    }
    return -1;
}