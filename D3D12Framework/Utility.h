#pragma once

extern void ShowErrorMessage(std::string_view file, int line, std::string_view message);
#define SHOW_ERROR(strMsg)		ShowErrorMessage(__FILE__, __LINE__, strMsg);

inline std::wstring ToWSting(const std::string& str)
{
	return std::format(L"{}", str);
}

inline std::string ToSting(const std::wstring& wstr)
{
	return std::format("{}", wstr);
}