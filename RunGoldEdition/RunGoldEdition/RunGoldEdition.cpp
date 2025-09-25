#include <windows.h>
#include <string>
#include <filesystem>
#include <fstream>

#include "resource.h"

using namespace std;

namespace fs = filesystem;

// .Global
HBITMAP hBitmap = NULL; // loading picture
LPCWSTR szClassName = L"WindowClass"; // window class name

const char wd_files_dir_name[] = "WDFiles";
const char video_dir_name[] = "Video";
const char spg_file_name[] = "SPG.wd1";
const char run_gold_edition_cfg_file_name[] = "RunGoldEdition.cfg";

// Window Function
static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            // Create a DC for drawing an image
            HDC hdcMem = CreateCompatibleDC(hdc);
            SelectObject(hdcMem, ::hBitmap);

            // Get bitmap information
            BITMAP bitmap = {};
            GetObject(::hBitmap, sizeof(BITMAP), &bitmap);

            // Draw a picture on the window
            BitBlt(hdc, 0, 0, bitmap.bmWidth, bitmap.bmHeight, hdcMem, 0, 0, SRCCOPY);

            DeleteDC(hdcMem);
            EndPaint(hwnd, &ps);
        }
        break;

        case WM_DESTROY:
            PostQuitMessage(0);
        break;

        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}

// main func
static int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    // Load image (bitmap from file)

    wstring spg_file_path = (fs::current_path() / ::wd_files_dir_name / ::spg_file_name).wstring();

    hBitmap = (HBITMAP)LoadImage(NULL, spg_file_path.c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    if (hBitmap == NULL)
    {
        MessageBox(NULL, L"The image could not be loaded!", L"Error", MB_ICONERROR);
        return -1;
    }

    // Get bitmap information
    BITMAP bitmap = {};
    GetObject(hBitmap, sizeof(BITMAP), &bitmap);

    // Get screen dimensions
    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);

    // Calculate the position of the window in the center of the screen
    int xPos = (screenWidth - bitmap.bmWidth) / 2;
    int yPos = (screenHeight - bitmap.bmHeight) / 2;

    // Register the window class
    WNDCLASS wc = { 0 };
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = szClassName;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH); // Brak tła
    RegisterClass(&wc);

    

    // Create a window without a frame or title bar (WS_POPUP style) at the specified position
    HWND hwnd = CreateWindowEx
    (
        WS_EX_LAYERED,                   // Extended style: transparent window
        szClassName,                     // Window class name
        NULL,                            // No window title
        WS_POPUP,                        // Frameless window style
        xPos, yPos,                      // Window position (center of screen)
        bitmap.bmWidth, bitmap.bmHeight, // Window size adjusted to the image
        NULL,                            // No parent
        NULL,                            // No menu
        hInstance,                       // Instance handle
        NULL                             // Additional parameters
    );

    if (hwnd == NULL)
    {
        return 0;
    }

    auto hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
    SendMessage(hwnd, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
    SendMessage(hwnd, WM_SETICON, ICON_SMALL2, (LPARAM)hIcon);
    SendMessage(hwnd, WM_SETICON, ICON_BIG, (LPARAM)hIcon);


    // Set the window transparency and green color as transparent
    COLORREF greenColor = RGB(3, 222, 3); // Green color (RGB)
    SetLayeredWindowAttributes(hwnd, greenColor, 255, LWA_COLORKEY);

    // Show window
    ShowWindow(hwnd, SW_SHOW);

    // Display the window for 1 second
    MSG msg = { 0 };
    DWORD startTime = GetTickCount64();
    while (GetTickCount64() - startTime < 500) // The loop runs for 1 second
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
                break;

            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    // Destroy the window after 3 seconds
    DestroyWindow(hwnd);

    // Release resources
    DeleteObject(hBitmap);

    fs::path video_dir_path = fs::current_path() / ::video_dir_name;

    fs::create_directory(video_dir_path);

    for (auto& itm : fs::directory_iterator(video_dir_path))
    {
        if (!itm.is_directory())
        { 
            //debugf << "ffplay.exe -i " << itm.path().string() << " -fs -autoexit" << std::endl;
            system(("ffplay.exe -i " + itm.path().string() + " -fs -autoexit").c_str());
        }

    }

    fs::path config_file_path = fs::current_path() / ::run_gold_edition_cfg_file_name;

    if (!fs::exists(config_file_path))
    {
        ofstream out(config_file_path);
        out << "LAUNCHER=\"KnightShift.exe\"\n";
    }

    ifstream in(config_file_path);
    string line;
    getline(in, line);

    string launcher;
    auto pos = line.find('=');

    if (pos != string::npos) 
    {
        launcher = line.substr(pos + 1);
        if (!launcher.empty() && launcher.front() == '"') launcher.erase(0, 1);
        if (!launcher.empty() && launcher.back() == '"') launcher.pop_back();
    }

    system(launcher.c_str());
  
    return 0;
}