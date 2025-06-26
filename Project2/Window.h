#pragma once

#include <Windows.h>
#include "Document.h"
#include <commctrl.h>
#include <commdlg.h>

#pragma comment(lib, "comctl32.lib")

#define MENU_CMD_NEW				1
#define MENU_CMD_OPEN				2
#define MENU_CMD_SAVE				3
#define MENU_CMD_EXIT				4

#define IDC_BUTTON_ADD				1001
#define IDC_TASK_LISTVIEW			1002

enum Columns
{
	LISTVIEW_COLUMN_NUMBER = 0,
	LISTVIEW_COLUMN_TASK = 1,
	LISTVIEW_COLUMN_DATE = 2
};

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

class Window
{
public:
	Window();
	Window(const Window&) = delete;
	Window& operator = (const Window&) = delete;
	~Window();

	bool ProcessMessages();
	bool ShowSaveFileDialog();

	Document& GetDocument();

	void CreateControls(); // �������� ��������� ����������
	void LoadIcons(); // �������� ������ ��� �������� �����

	HWND GetInputHandle() const { return m_hwndInput; }
	HWND GetListViewHandle() const { return m_hwndListView; }
	HWND GetAddButtonHandle() const { return m_hwndButton; }
	HWND GetDatePickerHandle() const { return m_hWndDatePicker; }

	HIMAGELIST GetStatusImageList() const { return m_hImageList; }
	std::vector<int>& GetStatusIcon(); 
	const std::vector<int>& GetStatusIcon() const;
	OPENFILENAMEW& GetSaveDialogConfig();

	const wchar_t* GetSelectedSavePath() const;

	void SetModified(bool value);
	bool IsModified() const { return m_isModified; }

	void ResetUI();

private:
	HINSTANCE m_hInstance;
	HWND m_hWnd;

	Document m_document; // ��������, ��������� � �����

	HWND m_hwndInput; // ������� �������������� ��� ����� ������
	HWND m_hwndButton; // ������ "��������"
	HWND m_hwndListView; // ������ ������������ �����

	HIMAGELIST m_hImageList = nullptr;
	HWND m_hWndDatePicker = nullptr;

	OPENFILENAMEW m_ofnSave = {};
	wchar_t m_saveFileName[MAX_PATH] = {}; 

	bool m_isModified = false; // ���� ��������� ���������

	std::vector<int> m_taskStatusIcons;
};

