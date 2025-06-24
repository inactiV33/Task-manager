#include "Document.h"

void Document::Clear() {

	title.clear();
	items.clear();

}

void Document::LoadFromFile(const std::wstring& filename) {

	// Заглушка
	title = L"Открытый документ";
	items = {L"Элемент 1", L"Элемент 2"};

}

void Document::SaveToFile(const std::wstring& filename) {

	//Заглушка

}