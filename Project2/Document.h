#pragma once

#include <string>
#include <vector>

class Document
{
public: 
	void Clear(); // ����� ��������
	void LoadFromFile(const std::wstring& filename); // �������� ��������� �� �����
	void SaveToFile(const std::wstring& filename); // ���������� ��������� � ����

	std::wstring title; // ��������� ���������
	std::vector<std::wstring> items; // ������ ��������� ���������

};
	
