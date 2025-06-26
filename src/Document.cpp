#include "Document.h"
#include <stdio.h>

Document::Document()
{
}

void Document::Clear() {
	m_title.clear();
	m_tasks.clear();
	m_dates.clear();
	m_statusIcons.clear();
}

void Document::SetTitle(const std::wstring title) {
	m_title = title;
}

const std::wstring& Document::GetTitle() const {
	return m_title;
}

void Document::AddTask(const std::wstring& task, const std::wstring& date, int statusIcon) {
	m_tasks.push_back(task);
	m_dates.push_back(date);
	m_statusIcons.push_back(statusIcon);
}

size_t Document::GetTaskCount() const {
	return m_tasks.size();
}

const std::wstring& Document::GetTask(size_t index) const {
	return m_tasks.at(index);
}

const std::wstring& Document::GetDate(size_t index) const {
	return m_dates.at(index);
}

int Document::GetStatusIcon(size_t index) const {
	if (index < m_statusIcons.size()) {
		return m_statusIcons.at(index);
	}
	return -1; // Возвращаем -1, если индекс вне диапазона
}

bool Document::LoadFromJsonFile(const std::wstring& filename) {

	FILE* file = nullptr;

	_wfopen_s(&file, filename.c_str(), L"rt, ccs=UTF-8");
	if (!file) return false;

	Clear(); // Очищаем текущий документ перед загрузкой

	wchar_t line[512];
	std::wstring currentTask, currentDate;
	int currentStatus = 0;

	while (fgetws(line, sizeof(line) / sizeof(wchar_t), file))
	{
		if (wcsstr(line, L"\"text\""))
		{
			wchar_t buffer[256] = {};
			swscanf_s(line, L" \"text\": \"%[^\"]", buffer, static_cast<unsigned>(_countof(buffer)));
			currentTask = buffer;
		}
		else if (wcsstr(line, L"\"date\""))
		{
			wchar_t buffer[64] = {};
			swscanf_s(line, L" \"date\": \"%[^\"]", buffer, static_cast<unsigned>(_countof(buffer)));
			currentDate = buffer;
		}
		else if (wcsstr(line, L"\"status\""))
		{
			swscanf_s(line, L" \"status\": %d", &currentStatus);
			AddTask(currentTask, currentDate, currentStatus);
		}
	}
	fclose(file);
	return true; 
}

bool Document::SaveToJsonFile(const std::wstring& filename) const {

	FILE* file = nullptr;
	_wfopen_s(&file, filename.c_str(), L"wt, ccs=UTF-8");

	if (!file) return false;

	fwprintf(file, L"\n\"title\": \"%s\",\n	\"tasks\": [\n", m_title.c_str());

	for (size_t i = 0; i < m_tasks.size(); ++i) {
		
		fwprintf(file, L"	{\n");
		fwprintf(file, L"	\t\"text\": \"%s\",\n", m_tasks[i].c_str());
		fwprintf(file, L"	\t\"date\": \"%s\",\n", m_dates[i].c_str());
		fwprintf(file, L"	\t\"status\": \"%d\"\n", m_statusIcons[i]);
		fwprintf(file, L"	}%s\n", (i < m_tasks.size() - 1) ? L"," : L"");
	}

	fwprintf(file, L"	]\n}\n");
	fclose(file);
	return true; // Успешное сохранение
}