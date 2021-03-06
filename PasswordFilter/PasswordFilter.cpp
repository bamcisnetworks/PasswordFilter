/**********************************************************************
	File name: PasswordFilter.cpp

	Contains the logic to execute password filtering
**********************************************************************/

// Has to be included first to compile
#include "stdafx.h"

// This includes stdafx.h, which includes windows.h and NTSecAPI.h
// NTSecAPI.h is needed to define PUNICODE_STING
// windows.h is needed to define BOOLEAN, TRUE, and FALSE
#include "PasswordFilter.h" 

// These are used to perform the password checking 
#include <string>
#include <vector>
#include <iterator>
#include <sstream>
#include <algorithm>
#include <cctype>

// Indicates that a password filter DLL is initialized.
// InitializeChangeNotify is called by the Local Security Authority (LSA) to verify that the password notification DLL is loaded and initialized.
extern "C" __declspec(dllexport) BOOLEAN __stdcall InitializeChangeNotify(void)
{
	// TRUE - The password filter DLL is initialized.
	// FALSE - The password filter DLL is not initialized.
	return TRUE;
}

// Indicates that a password has been changed, it notifies the DLL that a password was changed.
//
// UserName - The account name of the user whose password changed.
//	If the values of this parameter and the NewPassword parameter are NULL, this function should return STATUS_SUCCESS.
//
// RelativeId - The relative identifier (RID) of the user specified in UserName.
//
// NewPassword - A new plaintext password for the user specified in UserName. 
//	When you have finished using the password, clear the information by calling the SecureZeroMemory function. 
//	For more information about protecting passwords, see Handling Passwords.
//	If the values of this parameter and the UserName parameter are NULL, this function should return STATUS_SUCCESS.
extern "C" __declspec(dllexport) int __stdcall PasswordChangeNotify(_In_ PUNICODE_STRING *UserName, _In_ ULONG RelativeId, _In_ PUNICODE_STRING *NewPassword)
{
	if (UserName == NULL && NewPassword == NULL)
	{
		return 0; //STATUS_SUCCESS
	}
	else
	{
		// NewPassword is a pointer to a PUNICODE_STRING, which itself is a pointer to _LSA_UNICODE_STRING,
		// so we need to deference it twice if we wanted the LSA_UNICODE_STRING
		// Dereference the pointer to NewPassword and get the PUNICODE_STRING struct
		// Use -> here because PUNICODE_STRING is a pointer to _LSA_UNICODE which contains the property Length
		SecureZeroMemory(NewPassword, (*NewPassword)->Length);
		return 0;
	}
}

// Validates a new password based on password policy.
// AccountName - Pointer to a UNICODE_STRING that represents the name of the user whose password changed.
// FullName - Pointer to a UNICODE_STRING that represents the full name of the user whose password changed.
// Password - Pointer to a UNICODE_STRING that represents the new plaintext password. When you have finished using the password, clear it from memory by calling the SecureZeroMemory function. For more information on protecting the password, see Handling Passwords.
// SetOperation - TRUE if the password was set rather than changed.
extern "C" __declspec(dllexport) BOOLEAN __stdcall PasswordFilter(_In_ PUNICODE_STRING AccountName, _In_ PUNICODE_STRING FullName, _In_ PUNICODE_STRING Password, _In_ BOOLEAN SetOperation)
{
	BOOLEAN PasswordOk = TRUE;
	const uint32_t MinRepeatLength = 4;
	const uint32_t MinimumLength = 8;
	const uint32_t MaximumLength = 256;

	try
	{
		std::wstring User = AccountName->Buffer;
		std::wstring Name = FullName->Buffer;
		std::wstring Pass = Password->Buffer;

		// Now we can manipulate and review the password

		bool CannotContainUserName = true;

		if (CannotContainUserName)
		{
			if (contains<wchar_t>(Pass, User, true))
			{
				return FALSE;
			}
		}

		bool CannotContainName = true;

		if (CannotContainName)
		{
			std::vector<std::wstring> parts = split<wchar_t>(Name, ' ');

			for (std::wstring part : parts)
			{
				if (contains<wchar_t>(Pass, part, true))
				{
					return FALSE;
				}
			}
		}

		bool NoMoreThan3RepeatedChars = true;

		// Don't allow more than 3 of the same characters in a row
		// So MyYyPasss is ok, but MyYYYPaSSss is not
		if (NoMoreThan3RepeatedChars)
		{
			if (Pass.length() >= MinRepeatLength)
			{
				for (std::size_t i = 3; i < Pass.length(); i++)
				{
					if (std::toupper(Pass[i]) == std::toupper(Pass[i - 1]) && std::toupper(Pass[i]) == std::toupper(Pass[i - 2]) && std::toupper(Pass[i]) == std::toupper(Pass[i - 3]))
					{
						return FALSE;
					}
				}
			}
		}

		bool EnforceMinimumLength = true;

		if (EnforceMinimumLength)
		{
			if (Pass.length() < MinimumLength)
			{
				return FALSE;
			}
		}

		bool EnforceMaximumLength = true;

		if (EnforceMaximumLength)
		{
			if (Pass.length() > MaximumLength)
			{
				return FALSE;
			}
		}

		// Clear the text of the password
		Pass.clear();
	}
	// Catch all exceptions to ensure the process doesn't crash
	catch (...)
	{

	}

	// Make sure we zero the memory holding the password before returning
	wchar_t *Start = Password->Buffer;
	size_t Length = Password->MaximumLength;
	
	// RtlSecureZeroMemory
	SecureZeroMemory(Start, Length);

	// Return whether the password is valid or not
	return PasswordOk;
}

// Determines if item1 contains item2
template <typename T> static bool contains(const std::basic_string<T> item1, const std::basic_string<T> item2, bool insensitive = false)
{
	auto it = std::search(
		item1.cbegin(), item1.cend(),
		item2.cbegin(), item2.cend(),
		[insensitive](T ch1, T ch2) -> bool { return (insensitive ? std::toupper(ch1) == std::toupper(ch2) : ch1 == ch2); }
	);

	return (it != item1.cend());
}

template <typename T> std::vector<std::basic_string<T>> split(const std::basic_string<T> input, const T delimeter = ' ')
{
	// Holds the results from splitting
	std::vector<std::basic_string<T>> results;

	// The wstring input stream
	std::wstringstream wss(input);

	// Holds each split token 
	std::wstring temp;

	// While the input stream has lines delimited by the provided char, 
	// add them to the results vector
	while (std::getline(wss, temp, delimeter))
	{
		results.push_back(temp);
	}

	return results;
}