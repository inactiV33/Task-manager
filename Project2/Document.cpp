#include "Document.h"

void Document::Clear() {

	title.clear();
	items.clear();

}

void Document::LoadFromFile(const std::wstring& filename) {

	// ��������
	title = L"�������� ��������";
	items = {L"������� 1", L"������� 2"};

}

void Document::SaveToFile(const std::wstring& filename) {

	//��������

}