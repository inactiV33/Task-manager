#pragma once

#include <Windows.h>
#include "Document.h"
#include <commctrl.h>
#pragma comment(lib, "comctl32.lib")

#define MENU_CMD_NEW				1
#define MENU_CMD_OPEN				2
#define MENU_CMD_SAVE				3
#define MENU_CMD_EXIT				4

#define IDC_BUTTON_ADD				1001
#define IDC_TASK_LISTVIEW			1002

#define LISTVIEW_COLUMN_NUMBER		0
#define LISTVIEW_COLUMN_TASK		1
#define LISTVIEW_COLUMN_STATUS		2
// #define LISTVIEW_COLUMN_DATE		3

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

class Window
{
public:
	Window();
	Window(const Window&) = delete;
	Window& operator = (const Window&) = delete;
	~Window();

	bool ProcessMessages();

	Document& GetDocument();

	void CreateControls(); // Создание элементов управления
	void LoadIcons(); // Загрузка иконок для статусов задач

	HWND GetInputHandle() const { return m_hwndInput; }
	HWND GetListViewHandle() const { return m_hwndListView; }
	HWND GetAddButtonHandle() const { return m_hwndButton; }

	HIMAGELIST GetStatusImageList() const { return m_hImageList; }
	std::vector<int>& GetStatusIcon(); 
	const std::vector<int>& GetStatusIcon() const;

	void ResetUI();

private:
	HINSTANCE m_hInstance;
	HWND m_hWnd;

	Document m_document; // Документ, связанный с окном

	HWND m_hwndInput; // Элемент редактирования для ввода текста
	HWND m_hwndButton; // Кнопка "Добавить"
	HWND m_hwndListView; // Список отображаемых задач

	HIMAGELIST m_hImageList = nullptr;

	std::vector<int> m_taskStatusIcons;
};

