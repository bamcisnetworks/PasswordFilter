/**********************************************************************
	File name: PasswordFilter.h

	Contains the prototypes for the Password Filter interface
**********************************************************************/

#ifndef PASSWORD_FILTER_H
#define PASSWORD_FILTER_H

#include "stdafx.h"
#include <string>
#include <vector>

#define DllExport extern "C" __declspec(dllexport)

DllExport BOOLEAN __stdcall InitializeChangeNotify(void);
DllExport int __stdcall PasswordChangeNotify(PUNICODE_STRING *UserName, ULONG RelativeId, PUNICODE_STRING *NewPassword);
DllExport BOOLEAN __stdcall PasswordFilter(PUNICODE_STRING AccountName, PUNICODE_STRING FullName, PUNICODE_STRING Password, BOOLEAN SetOperation);

template <typename T> static bool contains(const std::basic_string<T> item1, const std::basic_string<T> item2, bool insensitive);
template <typename T> static std::vector<std::basic_string<T>> split(const std::basic_string<T> input, const T delimeter);


#endif // !PASSWORD_FILTER_H