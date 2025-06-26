#pragma once

#include <string>
#include <vector>

class Document
{
public: 
	Document();

	void Clear(); // ����� ��������
	
	void SetTitle(const std::wstring title);
	const std::wstring& GetTitle() const;

	void AddTask(const std::wstring& task, const std::wstring& date, int statusIcon); // ���������� ������
	size_t GetTaskCount() const; // ���������� �����

	const std::wstring& GetTask(size_t index) const; // ��������� ������ �� �������
	const std::wstring& GetDate(size_t index) const; // ��������� ���� ������ �� �������
	int GetStatusIcon(size_t index) const; // ��������� ������� ������ �� �������

	bool LoadFromJsonFile(const std::wstring& filename); // �������� ��������� �� �����
	bool SaveToJsonFile(const std::wstring& filename) const; // ���������� ��������� � ����

private:
	std::wstring m_title; // ��������� ���������
	std::vector<std::wstring> m_tasks; // ������ �����
	std::vector<std::wstring> m_dates; // ������ ���
	std::vector<int> m_statusIcons; // ������ �������� �����
};
	
