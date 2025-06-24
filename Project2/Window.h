#pragma once

#include <Windows.h>
#include "Document.h"

#define MENU_CMD_NEW		1
#define MENU_CMD_OPEN		2
#define MENU_CMD_SAVE		3
#define MENU_CMD_EXIT		4

#define IDC_BUTTON_ADD		1001
#define IDC_TASK_LIST		1002

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

	HWND GetInputHandle() const { return m_hwndInput; }
	HWND GetListHandle() const { return m_hwndList; }
	HWND GetAddButtonHandle() const { return m_hwndButton; }

	void ResetUI();

private:
	HINSTANCE m_hInstance;
	HWND m_hWnd;

	Document m_document; // Документ, связанный с окном

	HWND m_hwndInput; // Элемент редактирования для ввода текста
	HWND m_hwndButton; // Кнопка "Добавить"
	HWND m_hwndList; // Список отображаемых задач
};

