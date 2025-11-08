#include "pch.hpp"

HWND g_hMainWnd = NULL;
HWND g_hLogEdit = NULL;
UINT const WM_APP_LOG = WM_APP + 1;

void LogMessage(const std::string& msg, int type = 1)
{
    std::string prefixMsg = "";
    if (!g_hMainWnd) return;

    char* pText = new char[msg.length() + 10];
    switch (type) {
    case 1:
        prefixMsg = "[INFO] " + msg;
        break;
    case 2:
        prefixMsg = "[WARNING] " + msg;
        break;
    case 3:
        prefixMsg = "[ERROR] " + msg;
        break;
    default:
        prefixMsg = "[OTHER] " + msg;
        break;
    }


    strcpy_s(pText, prefixMsg.length() + 1, prefixMsg.c_str());

    PostMessage(g_hMainWnd, WM_APP_LOG, 0, (LPARAM)pText);
}

DWORD WINAPI AppLogic(LPVOID lpParam)
{
    LOG_CLEAR();

    if (!utils::is_updated()) {
        LogMessage("Radar is not updated! Check LOG for more info.", 3);
        return {};
    }

    LogMessage("Radar is up to date.");
    LOG_INFO("Radar is up to date.");

    config_data_t config_data = {};
    switch (cfg::setup(config_data))
    {
        case 0:
            break;
        case 1:
            LogMessage("Couldn't open config.json file, please check files and configure it.", 3);
            return {};
            break;
        case 2:
            LogMessage("Failed to parse config.json, please check syntax.", 3);
            return {};
            break;
        case 3:
            LogMessage("Failed to deserialize config.json.", 3);
            return {};
            break;
        default:
            LogMessage("Error", 3);
            break;
    }
    LogMessage("Config system initialization completed.");

    if (!exc::setup())
    {
        LogMessage("Exception setup failed! Check LOG for more info.", 3);
        return {};
    }
    LogMessage("Exception handler initialization completed.");

    switch (m_memory->setup()) {
        case 0:
            LogMessage("Found CS2.exe, continuing...");
            break;
        case 1:
            LogMessage("One or more anti-cheats are running, please close them.", 3);
            return {};
            break;
        case 2:
            LogMessage("Waiting for CS2.exe process...");
            while (m_memory->setup() == 2) {SLEEP(1)};
            LogMessage("Found CS2.exe, initializing...");
            SLEEP(5);
            break;
        case 3:
            LogMessage("Memory initialization failed.", 3);
            return {};
            break;
    }
    LogMessage("Memory initialization completed.");

    if (!i::setup())
    {
        LogMessage("Waiting for game to load...");
        while (!i::setup()) { SLEEP(3); };
        LogMessage("Game loaded.");
    }
    LogMessage("Interfaces initialization completed.");

    if (!schema::setup())
    {
        LogMessage("Schema setup failed! Check LOG for more info.", 3);
        return {};
    }
    LogMessage("Schema initialization completed.");

    WSADATA wsa_data = {};
    const auto wsa_startup = WSAStartup(MAKEWORD(2, 2), &wsa_data);
    if (wsa_startup != 0)
    {
        return {};
    }
    LogMessage("Winsock initialization completed.");

    const auto ipv4_address = utils::get_ipv4_address(config_data);
    if (ipv4_address.empty())
        LogMessage(std::format("Failed to automatically get your ipv4 address!\n                 we will use '{}' from 'config.json'. If the local ip is wrong, please set it.", config_data.m_local_ip), 2);

    const auto formatted_address = std::format("ws://{}:22006/cs2_webradar", ipv4_address);
    static auto web_socket = easywsclient::WebSocket::from_url(formatted_address);

    while (!web_socket) {
        web_socket = easywsclient::WebSocket::from_url(formatted_address);
        if (!web_socket)
        {
            LogMessage(std::format("Failed to connect to the web socket ({}), retrying...", formatted_address.c_str()), 3);
        }
    }
    LogMessage(std::format("Connected to the web socket ({}).", formatted_address.data()));

    auto start = std::chrono::system_clock::now();
    bool in_match = false;

    for (;;)
    {
        const auto now = std::chrono::system_clock::now();
        const auto duration = now - start;
        if (duration >= std::chrono::milliseconds(100))
        {
            start = now;

            sdk::update();
            in_match = f::run();
            if (!in_match) {
                f::m_data["m_map"] = "invalid";
            }
            web_socket->send(f::m_data.dump());
        }

        web_socket->poll();
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    LogMessage("WebRadar had stopped for no apparent reason...", 3);

    return true;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
    {
        g_hLogEdit = CreateWindow(TEXT("EDIT"),
            TEXT(""),
            WS_CHILD | WS_VISIBLE | WS_BORDER |
            ES_MULTILINE | ES_AUTOVSCROLL | ES_READONLY,
            10, 0, 360, 210,
            hWnd, (HMENU)1, NULL, NULL);

        SendMessage(g_hLogEdit, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), TRUE);
        break;
    }

    case WM_APP_LOG:
    {
        char* pText = (char*)lParam;

        int len = GetWindowTextLength(g_hLogEdit);
        SendMessage(g_hLogEdit, EM_SETSEL, (WPARAM)len, (LPARAM)len);
        SendMessage(g_hLogEdit, EM_REPLACESEL, 0, (LPARAM)pText);
        SendMessage(g_hLogEdit, EM_REPLACESEL, 0, (LPARAM)TEXT("\r\n"));

        delete[] pText;
        break;
    }

    case WM_DESTROY:
    {
        g_hLogEdit = NULL;
        g_hMainWnd = NULL;
        PostQuitMessage(0);
        break;
    }

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

int APIENTRY WinMain(HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR     lpCmdLine,
    int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    const char CLASS_NAME[] = "AppWindowClass";

    WNDCLASS wc = {};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

    RegisterClass(&wc);

    g_hMainWnd = CreateWindowEx(
        0,
        CLASS_NAME,
        "WBFCS",
        WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX & ~WS_THICKFRAME,

        CW_USEDEFAULT, CW_USEDEFAULT, 400, 260,

        NULL,
        NULL,
        hInstance,
        NULL
    );

    if (g_hMainWnd == NULL)
    {
        return 0;
    }

    ShowWindow(g_hMainWnd, nCmdShow);
    UpdateWindow(g_hMainWnd);

    HANDLE hThread = CreateThread(
        NULL,
        0,
        AppLogic,
        (LPVOID)g_hMainWnd,
        0,
        NULL
    );

    if (hThread == NULL)
    {
        MessageBox(g_hMainWnd, "Failed to create worker thread!", "Fatal Error", MB_OK | MB_ICONERROR);
        return 0;
    }
    CloseHandle(hThread);

    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}