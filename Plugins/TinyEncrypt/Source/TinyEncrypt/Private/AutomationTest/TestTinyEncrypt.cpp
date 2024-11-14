// Copyright (C) 2024 Neo Jin. All Rights Reserved.
#include "TinyEncryptAlgorithm.h"
#include "TinyEncryptUtilities.h"
#include "Misc/Paths.h"
#include "AutomationTest/TinyEncryptAutomationTestInterface.h"
#include "Misc/AutomationTest.h"
#include "Misc/CString.h"

#if !UE_BUILD_SHIPPING

bool TestTinyEncryptEncrypt(FString& Detail)
{
	const int32 MaxOutputLength = 256;
	uint8 EncryptOutputBuff[MaxOutputLength] = { 0 };
	uint8 DecryptOutputBuff[MaxOutputLength] = { 0 };

	{
		TEST_TRUE_WITH_AUTONAME(FTinyEncrypt::GetEncryptLength(0) == 8);
		TEST_TRUE_WITH_AUTONAME(FTinyEncrypt::GetEncryptLength(1) == 8);
		TEST_TRUE_WITH_AUTONAME(FTinyEncrypt::GetEncryptLength(7) == 8);
		TEST_TRUE_WITH_AUTONAME(FTinyEncrypt::GetEncryptLength(8) == 16);

		TEST_TRUE_WITH_AUTONAME(FTinyEncrypt::GetDecryptLength(0) == 0);
		TEST_TRUE_WITH_AUTONAME(FTinyEncrypt::GetDecryptLength(8) == 8);

		FUInt128Ex EncryptKey;
		EncryptKey.MakeRandom();
		FTinyEncrypt TEA(EncryptKey);

		int32 RealEncryptLength = TEA.Encrypt(nullptr, 0, EncryptOutputBuff);
		TEST_TRUE_WITH_AUTONAME(RealEncryptLength == 8);

		int32 RealDecryptLength = TEA.Decrypt(EncryptOutputBuff, RealEncryptLength, DecryptOutputBuff);
		TEST_TRUE_WITH_AUTONAME(RealDecryptLength == 0);
	}

	{
		uint8* PlainText = (uint8*)"Hello,World!";
		int32 PlainTextLen = TCString<char>::Strlen((const char*)PlainText);
		TEST_TRUE_WITH_AUTONAME(PlainTextLen == 12);

		const int32 EncryptLength = FTinyEncrypt::GetEncryptLength(PlainTextLen);
		TEST_TRUE_WITH_AUTONAME(EncryptLength == 16);

		//Make random TEA
		FUInt128Ex RandomKey;
		RandomKey.MakeRandom();
		FTinyEncrypt RandomTEA(RandomKey);

		//Test encrypt
		int32 RealEncryptLength = RandomTEA.Encrypt(PlainText, PlainTextLen, EncryptOutputBuff);
		TEST_TRUE_WITH_AUTONAME(RealEncryptLength == EncryptLength);

		//Test decrypt
		const int32 DecryptLength = FTinyEncrypt::GetDecryptLength(EncryptLength);
		TEST_TRUE_WITH_AUTONAME(DecryptLength == EncryptLength);

		int32 RealDecryptLength = RandomTEA.Decrypt(EncryptOutputBuff, EncryptLength, DecryptOutputBuff);
		TEST_TRUE_WITH_AUTONAME(RealDecryptLength == PlainTextLen);
		TEST_TRUE_WITH_AUTONAME(FMemory::Memcmp(PlainText, DecryptOutputBuff, PlainTextLen) == 0);

		//Make a solid TEA
		FUInt128Ex SolidKey(0x651085792dd1313e, 0x5b550778601818ae);
		FTinyEncrypt SolidTEA(SolidKey);

		//Test encrypt
		RealEncryptLength = SolidTEA.Encrypt(PlainText, PlainTextLen, EncryptOutputBuff);
		TEST_TRUE_WITH_AUTONAME(RealEncryptLength == EncryptLength);

		//Expect encrypt data
		uint8 ExceptEncryptData[] = { 0x5d, 0xb2, 0xed, 0xc1, 0x95, 0x35, 0x90, 0x14, 0x53, 0x8b, 0x5c, 0xbb, 0x73, 0x48, 0x07, 0x97 };
		TEST_TRUE_WITH_AUTONAME(FMemory::Memcmp(EncryptOutputBuff, ExceptEncryptData, RealEncryptLength) == 0);

		//Test decrypt
		RealDecryptLength = SolidTEA.Decrypt(EncryptOutputBuff, EncryptLength, DecryptOutputBuff);
		TEST_TRUE_WITH_AUTONAME(RealDecryptLength == PlainTextLen);
		TEST_TRUE_WITH_AUTONAME(FMemory::Memcmp(PlainText, DecryptOutputBuff, PlainTextLen) == 0);
	}

	//Test Again(Multiples of 8)
	{
		uint8* PlainText = (uint8*)"ABCDEFGHIGKLMNOP";
		int32 PlainTextLen = TCString<char>::Strlen((const char*)PlainText);
		TEST_TRUE_WITH_AUTONAME(PlainTextLen == 16);

		int32 EncryptLength = FTinyEncrypt::GetEncryptLength(PlainTextLen);
		TEST_TRUE_WITH_AUTONAME(EncryptLength == 24);

		//Make random TEA
		FUInt128Ex RandomKey;
		RandomKey.MakeRandom();
		FTinyEncrypt TEA(RandomKey);

		//Test encrypt
		int32 RealEncryptLength = TEA.Encrypt(PlainText, PlainTextLen, EncryptOutputBuff);
		TEST_TRUE_WITH_AUTONAME(RealEncryptLength == EncryptLength);

		//Test decrypt
		int32 DecryptLength = FTinyEncrypt::GetDecryptLength(EncryptLength);
		TEST_TRUE_WITH_AUTONAME(DecryptLength == EncryptLength);

		int32 RealDecryptLength = TEA.Decrypt(EncryptOutputBuff, EncryptLength, DecryptOutputBuff);
		TEST_TRUE_WITH_AUTONAME(RealDecryptLength == PlainTextLen);
		TEST_TRUE_WITH_AUTONAME(FMemory::Memcmp(PlainText, DecryptOutputBuff, PlainTextLen) == 0);
	}

	//Test Blueprint Utilities
	{
		uint8* PlainText = (uint8*)"Hello,World!";
		int32 PlainTextLen = TCString<char>::Strlen((const char*)PlainText);

		TArray<uint8> InputData(PlainText, PlainTextLen);
		TEST_TRUE_WITH_AUTONAME(InputData.Num() == 12);

		const int32 EncryptLength = FTinyEncrypt::GetEncryptLength(PlainTextLen);
		TEST_TRUE_WITH_AUTONAME(EncryptLength == 16);

		//Make random TEA
		FUInt128Ex RandomKey;
		RandomKey.MakeRandom();

		//Test encrypt
		TArray<uint8> EncryptedData = UTinyEncryptUtilities::EncryptWithTEA(InputData, RandomKey);
		TEST_TRUE_WITH_AUTONAME(EncryptedData.Num() == EncryptLength);

		//Test decrypt
		TArray<uint8> DecryptedData = UTinyEncryptUtilities::DecryptWithTEA(EncryptedData, RandomKey);
		TEST_TRUE_WITH_AUTONAME(DecryptedData.Num() == PlainTextLen);
		TEST_TRUE_WITH_AUTONAME(FMemory::Memcmp(PlainText, DecryptedData.GetData(), PlainTextLen) == 0);
	}

	//Test Blueprint Utilities again(Multiples of 8)
	{
		uint8* PlainText = (uint8*)"ABCDEFGHIGKLMNOP";
		int32 PlainTextLen = TCString<char>::Strlen((const char*)PlainText);

		TArray<uint8> InputData(PlainText, PlainTextLen);
		TEST_TRUE_WITH_AUTONAME(InputData.Num() == 16);

		const int32 EncryptLength = FTinyEncrypt::GetEncryptLength(PlainTextLen);
		TEST_TRUE_WITH_AUTONAME(EncryptLength == 24);

		//Make random TEA
		FUInt128Ex RandomKey;
		RandomKey.MakeRandom();

		//Test encrypt
		TArray<uint8> EncryptedData = UTinyEncryptUtilities::EncryptWithTEA(InputData, RandomKey);
		TEST_TRUE_WITH_AUTONAME(EncryptedData.Num() == EncryptLength);

		//Test decrypt
		TArray<uint8> DecryptedData = UTinyEncryptUtilities::DecryptWithTEA(EncryptedData, RandomKey);
		TEST_TRUE_WITH_AUTONAME(DecryptedData.Num() == PlainTextLen);
		TEST_TRUE_WITH_AUTONAME(FMemory::Memcmp(PlainText, DecryptedData.GetData(), PlainTextLen) == 0);
	}
	return true;
}

#endif //!UE_BUILD_SHIPPING

#if WITH_DEV_AUTOMATION_TESTS && !UE_BUILD_SHIPPING

IMPLEMENT_CUSTOM_SIMPLE_AUTOMATION_TEST(FTinyEncryptTestTEA, FAutomationTestBase, "TinyEncrypt.TEA", EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::EngineFilter)

bool FTinyEncryptTestTEA::RunTest(const FString& Parameters)
{
	FString Detail;
	bool bSuccess = TestTinyEncryptEncrypt(Detail);
	TestTrue(Detail, bSuccess);
	return true;
}

#endif
