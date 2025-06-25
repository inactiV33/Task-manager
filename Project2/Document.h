#pragma once

#include <string>
#include <vector>

class Document
{
public: 
	Document();

	void Clear(); // Новый документ
	
	void SetTitle(const std::wstring title);
	const std::wstring& GetTitle() const;

	void AddTask(const std::wstring& task, const std::wstring& date, int statusIcon); // Добавление задачи
	size_t GetTaskCount() const; // Количество задач

	const std::wstring& GetTask(size_t index) const; // Получение задачи по индексу
	const std::wstring& GetDate(size_t index) const; // Получение даты задачи по индексу
	int GetStatusIcon(size_t index) const; // Получение статуса задачи по индексу

	bool LoadFromJsonFile(const std::wstring& filename); // Загрузка документа из файла
	bool SaveToJsonFile(const std::wstring& filename) const; // Сохранение документа в файл

private:
	std::wstring m_title; // Заголовок документа
	std::vector<std::wstring> m_tasks; // Список задач
	std::vector<std::wstring> m_dates; // Список дат
	std::vector<int> m_statusIcons; // Список статусов задач
};
	
