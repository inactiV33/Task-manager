#include "Window.h"
#include "resource.h"

LRESULT WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if (uMsg == WM_NCCREATE)
    {
        CREATESTRUCT* pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
        SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pCreate->lpCreateParams));
    }

    switch(uMsg)
    {
    case WM_CLOSE:
        DestroyWindow(hWnd);
        break;
	case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);

        // Заливка белым фоном
        FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));

        EndPaint(hWnd, &ps);
        return 0;
    }
    case WM_COMMAND:
    {
        Window* pWindow = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

        switch (LOWORD(wParam)) {
        case MENU_CMD_NEW:
            if (pWindow) {
                pWindow->GetDocument().Clear(); // Обновляем документ
                pWindow->GetDocument().title = L"Новый документ";
                SetWindowText(hWnd, pWindow->GetDocument().title.c_str());
                pWindow->CreateControls();
                pWindow->ResetUI();
            }
            break;
        case MENU_CMD_OPEN:
            MessageBox(hWnd, L"Нажато 'Открыть'", L"Меню", MB_OK);
            break;
        case MENU_CMD_SAVE:
            MessageBox(hWnd, L"Нажато 'Сохранить'", L"Меню", MB_OK);
            break;
        case MENU_CMD_EXIT:
            PostQuitMessage(0);
            break;
        case IDC_BUTTON_ADD:
            if (pWindow) {
                wchar_t buffer[256];

                GetWindowText(pWindow->GetInputHandle(), buffer, 256);

                if (wcslen(buffer) > 0) {
                    LRESULT index = SendMessage(pWindow->GetListHandle(), LB_ADDSTRING, 0, (LPARAM)buffer);
                    SendMessage(pWindow->GetListHandle(), LB_SETCURSEL, index, 0);

                    pWindow->GetDocument().items.push_back(buffer);

                    SetWindowText(pWindow->GetInputHandle(), L"");
                }
            }
            break;
        }
        return 0;
    }
    }
    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

Window::Window()
    : m_hInstance(GetModuleHandle(nullptr)),
      m_hwndInput(nullptr),
      m_hwndList(nullptr),
      m_hwndButton(nullptr)
{

    const wchar_t* CLASS_NAME = L"Hugos Window Class";

    WNDCLASS wndClass = {};

    wndClass.lpszClassName = CLASS_NAME;
	wndClass.hInstance = m_hInstance;
    wndClass.hIcon = LoadIcon(m_hInstance, MAKEINTRESOURCE(IDI_ICON1));
    wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndClass.lpfnWndProc = WindowProc;

    RegisterClass(&wndClass);

    DWORD style = WS_SYSMENU | WS_CAPTION | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_THICKFRAME;

    int width = 640;
	int height = 480;  

    RECT rect;
    rect.left = 250;
    rect.top = 250;
    rect.right = rect.left + width;
    rect.bottom = rect.top + height;

    AdjustWindowRect(&rect, style, false);

    m_hWnd = CreateWindowEx(
        0,
        CLASS_NAME,
        L"Task manager",
        style,
        rect.left,
        rect.top,
        rect.right - rect.left,
        rect.bottom - rect.top,
        NULL,
        NULL,
        m_hInstance,
        this
    );

	HMENU hMenu = CreateMenu();
	HMENU hFileMenu = CreatePopupMenu();

    AppendMenu(hFileMenu, MF_STRING, MENU_CMD_NEW, L"Новый");
    AppendMenu(hFileMenu, MF_STRING, MENU_CMD_OPEN, L"Открыть");
    AppendMenu(hFileMenu, MF_STRING | MF_GRAYED, MENU_CMD_SAVE, L"Сохранить");
    AppendMenu(hFileMenu, MF_SEPARATOR, 0, nullptr);
    AppendMenu(hFileMenu, MF_STRING, MENU_CMD_EXIT, L"Выход");

    AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hFileMenu, L"Файл");
    SetMenu(m_hWnd, hMenu);

    ShowWindow(m_hWnd, SW_SHOW);
}

Window::~Window()
{

    const wchar_t* CLASS_NAME = L"Hugos Window Class";

    UnregisterClass(CLASS_NAME, m_hInstance);

}

bool Window::ProcessMessages()
{
    MSG msg = {};

    while (PeekMessage(&msg, nullptr, 0u, 0u, PM_REMOVE)) {
        
        if (msg.message == WM_QUIT) {
            return false;
        }

        TranslateMessage(&msg);
		DispatchMessage(&msg);
    }

    return true;
}

Document& Window::GetDocument() {
    return m_document;
}

void Window::CreateControls() {

    if (m_hwndInput) return; // Если элемент создан ничего не делаем

    // Список задач слева
    m_hwndList = CreateWindowEx(WS_EX_CLIENTEDGE, L"LISTBOX", nullptr,
        WS_CHILD | WS_VISIBLE | WS_VSCROLL | LBS_NOTIFY,
        20, 20, 250, 400,
        m_hWnd, (HMENU)IDC_TASK_LIST,
        m_hInstance, nullptr);

    int inputX = 290;
    int inputY = 20;
    int inputWidth = 300;
    int inputHeight = 25;

    m_hwndInput = CreateWindowEx(0, L"EDIT", nullptr,
        WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
        inputX, inputY, inputWidth, inputHeight,
        m_hWnd, nullptr,
        m_hInstance, nullptr);

    // Центрирование кнопки под полем
    int buttonWidth = 100;
    int buttonHeight = 25;
    int buttonX = inputX + (inputWidth - buttonWidth) / 2;
    int buttonY = inputY + inputHeight + 15;

    m_hwndButton = CreateWindowEx(0, L"BUTTON", L"Добавить",
        WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
        buttonX, buttonY, buttonWidth, buttonHeight,
        m_hWnd, (HMENU)IDC_BUTTON_ADD,
        m_hInstance, nullptr);
}

void Window::ResetUI() {

    if (m_hwndInput) {
        SetWindowText(m_hwndInput, L"");
    }
    if (m_hwndList) {
        SendMessage(m_hwndList, LB_RESETCONTENT, 0, 0);
    }

}
