#include "Window.h"
#include "resource.h"
#include <commdlg.h>

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if (uMsg == WM_NCCREATE)
    {
        CREATESTRUCT* pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
        SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pCreate->lpCreateParams));
    }

    switch (uMsg)
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
        FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));
        EndPaint(hWnd, &ps);
        return 0;
    }

    case WM_COMMAND:
    {
        Window* pWindow = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
        switch (LOWORD(wParam))
        {
        case MENU_CMD_NEW:
            if (pWindow)
            {
                pWindow->GetDocument().Clear();
                pWindow->GetDocument().SetTitle(L"Новый документ");
                SetWindowText(hWnd, pWindow->GetDocument().GetTitle().c_str());
                pWindow->CreateControls();
                pWindow->ResetUI();
            }
            break;

        case MENU_CMD_OPEN:

            if (pWindow && pWindow->ShowSaveFileDialog()) {
				const wchar_t* path = pWindow->GetSelectedSavePath();
                if (pWindow->GetDocument().LoadFromJsonFile(path)) {
					SetWindowText(hWnd, pWindow->GetDocument().GetTitle().c_str());
                    pWindow->CreateControls();
                    pWindow->ResetUI();

					// Fill ListView with loaded tasks
					const Document& doc = pWindow->GetDocument();
					HWND hListView = pWindow->GetListViewHandle();
					auto& icons = pWindow->GetStatusIcon();

                    for (size_t i = 0; i < doc.GetTaskCount(); ++i) 
                    {
                        wchar_t number[16];
						swprintf_s(number, _countof(number), L"%zu", i + 1);

						LVITEM lvi = {};
                        lvi.mask = LVIF_TEXT | LVIF_IMAGE;
                        lvi.iItem = static_cast<int>(i);
                        lvi.pszText = number;
                        lvi.iImage = doc.GetStatusIcon(i);

						icons.push_back(lvi.iImage);

                        ListView_InsertItem(hListView, &lvi);
                        ListView_SetItemText(hListView, i, LISTVIEW_COLUMN_TASK, const_cast<LPWSTR>(doc.GetTask(i).c_str()));
                        ListView_SetItemText(hListView, i, LISTVIEW_COLUMN_DATE, const_cast<LPWSTR>(doc.GetDate(i).c_str()));

                    }

                    pWindow->SetModified(false);
                }
                else {
                    MessageBox(hWnd, L"Не удалось загрузить файл", L"Ошибка", MB_ICONERROR);
                }
            }

            break;

        case MENU_CMD_SAVE:
            if (pWindow) {

                if (pWindow->ShowSaveFileDialog()) 
                {
                    std::wstring path = pWindow->GetSelectedSavePath();

                    if (pWindow->GetDocument().SaveToJsonFile(path))
                    {
                        MessageBox(hWnd, L"Файл успешно сохранен", L"Сохранение", MB_OK);
                        pWindow->SetModified(false);
                    } 
                    else 
                    {
                        MessageBox(hWnd, L"Ошибка при сохранении файла", L"Ошибка", MB_OK | MB_ICONERROR);
					}
                }
            }

            break;

        case MENU_CMD_EXIT:
            PostQuitMessage(0);
            break;

        case IDC_BUTTON_ADD:
            if (pWindow)
            {
                wchar_t taskBuffer[256];
                GetWindowText(pWindow->GetInputHandle(), taskBuffer, 256);
                if (wcslen(taskBuffer) > 0)
                {
                    EnableMenuItem(GetMenu(hWnd), MENU_CMD_SAVE, MF_ENABLED);
                    DrawMenuBar(hWnd);

                    int nextIndex = ListView_GetItemCount(pWindow->GetListViewHandle());
                    std::wstring indexStr = std::to_wstring(nextIndex + 1);

                    wchar_t numberText[16];
                    swprintf_s(numberText, _countof(numberText), L"%d", nextIndex + 1);

					auto& iconList = pWindow->GetStatusIcon();
					iconList.push_back(0); // Добавляем иконку по умолчанию (0 - "В процессе")

                    SYSTEMTIME st = {};
					DateTime_GetSystemtime(pWindow->GetDatePickerHandle(), &st);
					//GetLocalTime(&st);
                    wchar_t dateBuffer[64];
                    swprintf_s(dateBuffer, _countof(dateBuffer), L"%02d.%02d.%04d", st.wDay, st.wMonth, st.wYear);

                    LVITEM lvi = {};
                    lvi.mask = LVIF_TEXT | LVIF_IMAGE;
                    lvi.iItem = nextIndex;
                    lvi.iSubItem = 0;
					lvi.iImage = iconList[nextIndex]; // Устанавливаем иконку для элемента
                    lvi.pszText = numberText;
                    //lvi.pszText = const_cast<LPWSTR>(indexStr.c_str());

                    ListView_InsertItem(pWindow->GetListViewHandle(), &lvi);

                    ListView_SetItemText(pWindow->GetListViewHandle(), nextIndex, LISTVIEW_COLUMN_TASK, taskBuffer);
                    ListView_SetItemText(pWindow->GetListViewHandle(), nextIndex, LISTVIEW_COLUMN_DATE, dateBuffer);

                    //pWindow->GetDocument()..push_back(buffer);
                    //pWindow->GetStatusIcon().push_back(0);

					GetWindowText(pWindow->GetInputHandle(), taskBuffer, _countof(taskBuffer));

                    pWindow->GetDocument().AddTask(taskBuffer, dateBuffer, 0);

                    SetWindowText(pWindow->GetInputHandle(), L"");
                    SetFocus(pWindow->GetInputHandle());

                    ListView_SetItemState(pWindow->GetListViewHandle(), nextIndex, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
                }
            }
            break;
        }
        return 0;
    }

    case WM_NOTIFY:
    {
        //Window* pWindow = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
        //if (!pWindow || !lParam)
        //    break;

        //LPNMHDR pnmhdr = reinterpret_cast<LPNMHDR>(lParam);
        //if (pnmhdr->idFrom == IDC_TASK_LISTVIEW && pnmhdr->code == NM_CUSTOMDRAW)
        //{
        //    LPNMLVCUSTOMDRAW pcd = reinterpret_cast<LPNMLVCUSTOMDRAW>(lParam);
        //    if ((pcd->nmcd.dwDrawStage & CDDS_ITEMPREPAINT) && (pcd->nmcd.dwDrawStage & CDDS_SUBITEM))
        //    {
        //        if (pcd->iSubItem == LISTVIEW_COLUMN_DATE)
        //        {
        //            RECT rc = pcd->nmcd.rc;
        //            int iconIndex = 0;
        //            int item = static_cast<int>(pcd->nmcd.dwItemSpec);
        //            if (item < static_cast<int>(pWindow->GetStatusIcon().size()))
        //                iconIndex = pWindow->GetStatusIcon()[item];

        //            HIMAGELIST hImageList = pWindow->GetStatusImageList();
        //            if (hImageList)
        //            {
        //                int iconSize = 16;
        //                int yCenter = rc.top + ((rc.bottom - rc.top - iconSize) / 2);
        //                int xOffset = rc.left + 4;
        //                ImageList_Draw(hImageList, iconIndex, pcd->nmcd.hdc, xOffset, yCenter, ILD_TRANSPARENT);
        //            }
        //            return CDRF_SKIPDEFAULT;
        //        }
        //    }
        //    return CDRF_DODEFAULT;
        //}
        return 0;
    }
    }
    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

Window::Window()
    : m_hInstance(GetModuleHandle(nullptr)),
    m_hwndInput(nullptr),
    m_hwndListView(nullptr),
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

    DWORD style = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU;

    int width = 800;
	int height = 600;  

    RECT rect;
    rect.left = 250;
    rect.top = 250;
    rect.right = rect.left + width;
    rect.bottom = rect.top + height;

    AdjustWindowRect(&rect, style, false);

	INITCOMMONCONTROLSEX icex = { sizeof(icex), ICC_LISTVIEW_CLASSES };
	InitCommonControlsEx(&icex);

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

bool Window::ShowSaveFileDialog()
{
    //ZeroMemory(&m_ofnSave, sizeof(m_ofnSave));
    ZeroMemory(m_saveFileName, sizeof(m_saveFileName));;

	m_ofnSave.lStructSize = sizeof(m_ofnSave);
    m_ofnSave.hwndOwner = m_hWnd;
    m_ofnSave.lpstrFilter = L"JSON Files (*.json)\0*.json\0All files (*.*)\0*.*\0";
	m_ofnSave.lpstrFile = m_saveFileName;
	m_ofnSave.nMaxFile = MAX_PATH;
    m_ofnSave.Flags = OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST;
    m_ofnSave.lpstrDefExt = L"json";

    return GetSaveFileNameW(&m_ofnSave) == TRUE;
}

Document& Window::GetDocument() {
    return m_document;
}

void Window::CreateControls() {

    if (m_hwndInput) return; // Если элемент создан ничего не делаем

    // Список задач слева
    m_hwndListView = CreateWindowEx(WS_EX_CLIENTEDGE, WC_LISTVIEW, nullptr,
        WS_CHILD | WS_VISIBLE | LVS_REPORT | LVS_SINGLESEL,
        20, 20, 520, 540,
        m_hWnd, (HMENU)IDC_TASK_LISTVIEW,
        m_hInstance, nullptr);

    LVCOLUMN lvc = {};
    lvc.mask = LVCF_TEXT | LVCF_WIDTH;

    // Колонка #1 - "Номер задачи"
    lvc.pszText = const_cast<LPWSTR>(L"№");
    lvc.cx = 50;
    ListView_InsertColumn(m_hwndListView, LISTVIEW_COLUMN_NUMBER, &lvc);

	// Колонка #2 - "Задачи"
	lvc.pszText = const_cast<LPWSTR>(L"Задачи");
    lvc.cx = 330;
	ListView_InsertColumn(m_hwndListView, LISTVIEW_COLUMN_TASK, &lvc);

    // Колонка #3 - "Статус"
    lvc.pszText = const_cast<LPWSTR>(L"Срок");
    lvc.cx = 120;
    ListView_InsertColumn(m_hwndListView, LISTVIEW_COLUMN_DATE, &lvc);

    // ======================= Load icons =======================

    LoadIcons();

	// ======================= Field for input =======================
    int inputX = 560;
    int inputY = 20;
    int inputWidth = 210;
    int inputHeight = 25;

    m_hwndInput = CreateWindowEx(0, L"EDIT", nullptr,
        WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
        inputX, inputY, inputWidth, inputHeight,
        m_hWnd, nullptr,
        m_hInstance, nullptr);

    // ======================= DatePicker =======================
    
    m_hWndDatePicker = CreateWindowEx(0, DATETIMEPICK_CLASS, 
        nullptr, WS_CHILD | WS_VISIBLE | DTS_SHORTDATEFORMAT, 
        inputX, inputY + 60, inputWidth, 25, 
        m_hWnd, nullptr,
        m_hInstance, nullptr);

    // ======================= Buttons =======================

    // Центрирование кнопки под полем
    int buttonWidth = 100;
    int buttonX = inputX + (inputWidth - buttonWidth) / 2;
    int buttonY = inputY + inputHeight + 70;

    m_hwndButton = CreateWindowEx(0, L"BUTTON", L"Добавить",
        WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
        buttonX, buttonY, buttonWidth, 25,
        m_hWnd, (HMENU)IDC_BUTTON_ADD,
        m_hInstance, nullptr);
}

std::vector<int>& Window::GetStatusIcon()
{
	return m_taskStatusIcons;
}

const std::vector<int>& Window::GetStatusIcon() const
{
    return m_taskStatusIcons;
}

OPENFILENAMEW& Window::GetSaveDialogConfig()
{
	return m_ofnSave;
}

const wchar_t* Window::GetSelectedSavePath() const
{
	return m_saveFileName;
}

void Window::SetModified(bool value)
{
	m_isModified = value;
	EnableMenuItem(GetMenu(m_hWnd), MENU_CMD_SAVE, value ? MF_ENABLED : MF_GRAYED);
	DrawMenuBar(m_hWnd);
}

void Window::ResetUI() {

    if (m_hwndInput) {
        SetWindowText(m_hwndInput, L"");
    }
    if (m_hwndListView) {
		ListView_DeleteAllItems(m_hwndListView);
    }
    if (m_hWndDatePicker) {
        ListView_DeleteAllItems(m_hWndDatePicker);
    }
}

void Window::LoadIcons()
{
    // Загружаем иконки для статусов задач
	m_hImageList = ImageList_Create(16, 16, ILC_COLOR32 | ILC_MASK, 3, 1);

    HICON hIconCompleted = LoadIcon(m_hInstance, MAKEINTRESOURCE(IDI_ICON2));
    //if (!hIconCompleted) MessageBox(NULL, L"IDI_ICON2 не загружен", L"Ошибка загрузки", MB_OK);
    HICON hIconProcess = LoadIcon(m_hInstance, MAKEINTRESOURCE(IDI_ICON3));
    HICON hIconClosed = LoadIcon(m_hInstance, MAKEINTRESOURCE(IDI_ICON4));

	ImageList_AddIcon(m_hImageList, hIconCompleted);
	ImageList_AddIcon(m_hImageList, hIconProcess);
    ImageList_AddIcon(m_hImageList, hIconClosed);

	ListView_SetImageList(m_hwndListView, m_hImageList, LVSIL_SMALL);
}