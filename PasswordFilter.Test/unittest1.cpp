#include "stdafx.h"
#include "CppUnitTest.h"
#include "../PasswordFilter/PasswordFilter.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace PasswordFilterTest
{		
	TEST_CLASS(UnitTest1)
	{
	public:
		
		TEST_METHOD(TestNoUserNameInPassword)
		{
			LSA_UNICODE_STRING *UserLsa = new LSA_UNICODE_STRING();
			RtlInitUnicodeString(UserLsa, L"michael.haken");

			LSA_UNICODE_STRING *NameLsa = new LSA_UNICODE_STRING();
			RtlInitUnicodeString(NameLsa, L"John Smith");

			LSA_UNICODE_STRING *PassLsa = new LSA_UNICODE_STRING();
			RtlInitUnicodeString(PassLsa, L"michaelTestPass");

			BOOLEAN ExpectedResult = FALSE;
			BOOLEAN Result = PasswordFilter(UserLsa, NameLsa, PassLsa, FALSE);

			Assert::AreEqual(ExpectedResult, Result);

			RtlFreeUnicodeString(PassLsa);
			RtlFreeUnicodeString(NameLsa);
			RtlFreeUnicodeString(UserLsa);
		}

		TEST_METHOD(TestNoFullNameInPassword)
		{
			LSA_UNICODE_STRING *UserLsa = new LSA_UNICODE_STRING();
			RtlInitUnicodeString(UserLsa, L"michael.haken");

			LSA_UNICODE_STRING *NameLsa = new LSA_UNICODE_STRING();
			RtlInitUnicodeString(NameLsa, L"John Smith");

			LSA_UNICODE_STRING *PassLsa = new LSA_UNICODE_STRING();
			RtlInitUnicodeString(PassLsa, L"PassSmith1");

			BOOLEAN ExpectedResult = FALSE;
			BOOLEAN Result = PasswordFilter(UserLsa, NameLsa, PassLsa, FALSE);

			Assert::AreEqual(ExpectedResult, Result);

			RtlFreeUnicodeString(PassLsa);
			RtlFreeUnicodeString(NameLsa);
			RtlFreeUnicodeString(UserLsa);
		}

		TEST_METHOD(FourRepeatedCharacters)
		{
			LSA_UNICODE_STRING *UserLsa = new LSA_UNICODE_STRING();
			RtlInitUnicodeString(UserLsa, L"michael.haken");

			LSA_UNICODE_STRING *NameLsa = new LSA_UNICODE_STRING();
			RtlInitUnicodeString(NameLsa, L"John Smith");

			LSA_UNICODE_STRING *PassLsa = new LSA_UNICODE_STRING();
			RtlInitUnicodeString(PassLsa, L"MyPass1111abcaaaa");

			BOOLEAN ExpectedResult = FALSE;
			BOOLEAN Result = PasswordFilter(UserLsa, NameLsa, PassLsa, FALSE);

			Assert::AreEqual(ExpectedResult, Result);

			RtlFreeUnicodeString(PassLsa);
			RtlFreeUnicodeString(NameLsa);
			RtlFreeUnicodeString(UserLsa);
		}

		TEST_METHOD(TooShort)
		{
			LSA_UNICODE_STRING *UserLsa = new LSA_UNICODE_STRING();
			RtlInitUnicodeString(UserLsa, L"michael.haken");

			LSA_UNICODE_STRING *NameLsa = new LSA_UNICODE_STRING();
			RtlInitUnicodeString(NameLsa, L"John Smith");

			LSA_UNICODE_STRING *PassLsa = new LSA_UNICODE_STRING();
			RtlInitUnicodeString(PassLsa, L"pass");

			BOOLEAN ExpectedResult = FALSE;
			BOOLEAN Result = PasswordFilter(UserLsa, NameLsa, PassLsa, FALSE);

			Assert::AreEqual(ExpectedResult, Result);

			RtlFreeUnicodeString(PassLsa);
			RtlFreeUnicodeString(NameLsa);
			RtlFreeUnicodeString(UserLsa);
		}

		TEST_METHOD(TooLong)
		{
			LSA_UNICODE_STRING *UserLsa = new LSA_UNICODE_STRING();
			RtlInitUnicodeString(UserLsa, L"michael.haken");

			LSA_UNICODE_STRING *NameLsa = new LSA_UNICODE_STRING();
			RtlInitUnicodeString(NameLsa, L"John Smith");

			LSA_UNICODE_STRING *PassLsa = new LSA_UNICODE_STRING();
			std::wstring temp = L"12345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678990";

			for (int i = 0; i < 3; i++)
			{
				temp += temp;
			}

			PWSTR temp2 = const_cast<PWSTR>(temp.c_str());

			RtlInitUnicodeString(PassLsa, temp2);

			BOOLEAN ExpectedResult = FALSE;
			BOOLEAN Result = PasswordFilter(UserLsa, NameLsa, PassLsa, FALSE);

			Assert::AreEqual(ExpectedResult, Result);

			RtlFreeUnicodeString(PassLsa);
			RtlFreeUnicodeString(NameLsa);
			RtlFreeUnicodeString(UserLsa);
		}

		TEST_METHOD(SuccessfulPassword)
		{
			LSA_UNICODE_STRING *UserLsa = new LSA_UNICODE_STRING();
			RtlInitUnicodeString(UserLsa, L"michael.haken");

			LSA_UNICODE_STRING *NameLsa = new LSA_UNICODE_STRING();
			RtlInitUnicodeString(NameLsa, L"John Smith");

			LSA_UNICODE_STRING *PassLsa = new LSA_UNICODE_STRING();
			RtlInitUnicodeString(PassLsa, L"!QAZ@WSX1qaz2wsx");

			BOOLEAN ExpectedResult = TRUE;
			BOOLEAN Result = PasswordFilter(UserLsa, NameLsa, PassLsa, FALSE);

			Assert::AreEqual(ExpectedResult, Result);

			RtlFreeUnicodeString(PassLsa);
			RtlFreeUnicodeString(NameLsa);
			RtlFreeUnicodeString(UserLsa);
		}

	private:

		// Creates a UNICODE_STRING from the provided input
		void RtlInitUnicodeString( _Out_ PUNICODE_STRING DestinationString, _In_ PWSTR SourceString)
		{
			DestinationString->Buffer = new wchar_t[wcslen(SourceString) * sizeof(WCHAR)]{ 0 };
			std::wstring temp = SourceString;
			temp.copy(DestinationString->Buffer, wcslen(SourceString), 0);

			DestinationString->MaximumLength = wcslen(SourceString) * sizeof(WCHAR);
			DestinationString->Length = DestinationString->MaximumLength;
		}

		// Frees allocated memory used to created the UNICODE_STRING struct
		void RtlFreeUnicodeString(_In_ PUNICODE_STRING str)
		{
			delete[] str->Buffer;
			str->Buffer = NULL;
			delete str;
			str = NULL;
		}
	};
}