#pragma once

#include <string>
#include <vector>

class Document
{
public: 
	void Clear(); // Новый документ
	void LoadFromFile(const std::wstring& filename); // Загрузка документа из файла
	void SaveToFile(const std::wstring& filename); // Сохранение документа в файл

	std::wstring title; // Заголовок документа
	std::vector<std::wstring> items; // Список элементов документа

};
	
