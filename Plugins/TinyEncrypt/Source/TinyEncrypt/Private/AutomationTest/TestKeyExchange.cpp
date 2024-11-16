// Copyright (C) 2024 Neo Jin. All Rights Reserved.
#include "TinyEncryptKeyExchange.h"
#include "Misc/Paths.h"
#include "AutomationTest/TinyEncryptAutomationTestInterface.h"
#include "Misc/AutomationTest.h"
#include "Runtime/Launch/Resources/Version.h"

#if !UE_BUILD_SHIPPING

bool TestTinyEncryptUInt128(FString & Detail)
{
	{
		FUInt128Ex Value;
		TEST_TRUE_WITH_AUTONAME(Value.HiPart() == 0 && Value.LowPart() == 0);
		TEST_TRUE_WITH_AUTONAME(Value.IsZero());
		TEST_TRUE_WITH_AUTONAME(!Value.IsOdd());
	}
	{
		FUInt128Ex Value(0x1234ULL);
		TEST_TRUE_WITH_AUTONAME(Value.HiPart() == 0 && Value.LowPart() == 0x1234ULL);
		TEST_TRUE_WITH_AUTONAME(!Value.IsZero());
		TEST_TRUE_WITH_AUTONAME(!Value.IsOdd());
	}
	{
		FUInt128Ex Value(0x1234ULL, 0x5678ULL);
		TEST_TRUE_WITH_AUTONAME(Value.HiPart() == 0x1234ULL && Value.LowPart() == 0x5678ULL);
		TEST_TRUE_WITH_AUTONAME(!Value.IsZero());
		TEST_TRUE_WITH_AUTONAME(!Value.IsOdd());
	}
	{
		FUInt128Ex Value(0x123U, 0x456U, 0x789U, 0xABCU);
		TEST_TRUE_WITH_AUTONAME(Value.HiPart() == 0x0000012300000456ULL && Value.LowPart() == 0x0000078900000ABCULL);
		TEST_TRUE_WITH_AUTONAME(!Value.IsZero());
		TEST_TRUE_WITH_AUTONAME(!Value.IsOdd());
	}
	{
		FUInt128Ex Value(0xFFFFFFFFFFFFFFFFULL, 0xFFFFFFFFFFFFFFFFULL);
		TEST_TRUE_WITH_AUTONAME(Value.HiPart() == 0xFFFFFFFFFFFFFFFFULL && Value.LowPart() == 0xFFFFFFFFFFFFFFFFULL);
		TEST_TRUE_WITH_AUTONAME(!Value.IsZero());
		TEST_TRUE_WITH_AUTONAME(Value.IsOdd());
	}

	//LeftShift
	{
		FUInt128Ex Value = FUInt128Ex(0ULL);
		Value.LeftShift();
		TEST_TRUE_WITH_AUTONAME(Value.IsZero());

		Value = FUInt128Ex(1ULL);
		Value.LeftShift();
		TEST_TRUE_WITH_AUTONAME(Value == FUInt128Ex(2ULL));

		Value = FUInt128Ex(0xFFFFFFFFFFFFFFFFULL);
		Value.LeftShift();
		TEST_TRUE_WITH_AUTONAME(Value == FUInt128Ex(0x1ULL, 0xFFFFFFFFFFFFFFFEULL));

		Value = FUInt128Ex(1ULL, 0ULL);
		Value.LeftShift();
		TEST_TRUE_WITH_AUTONAME(Value == FUInt128Ex(0x2ULL, 0ULL));

		Value = FUInt128Ex(0x8000000000000000ULL, 0ULL);
		Value.LeftShift();
		TEST_TRUE_WITH_AUTONAME(Value == FUInt128Ex(0ULL, 0ULL));

		Value = FUInt128Ex(0xFFFFFFFFFFFFFFFFULL, 0xFFFFFFFFFFFFFFFFULL);
		Value.LeftShift();
		TEST_TRUE_WITH_AUTONAME(Value == FUInt128Ex(0xFFFFFFFFFFFFFFFFULL, 0xFFFFFFFFFFFFFFFEULL));
	}

	//RightShift
	{
		FUInt128Ex Value = FUInt128Ex(0ULL);
		Value.RightShift();
		TEST_TRUE_WITH_AUTONAME(Value.IsZero());

		Value = FUInt128Ex(1ULL);
		Value.RightShift();
		TEST_TRUE_WITH_AUTONAME(Value.IsZero());

		Value = FUInt128Ex(1ULL, 0ULL);
		Value.RightShift();
		TEST_TRUE_WITH_AUTONAME(Value == FUInt128Ex(0ULL, 0x8000000000000000ULL));

		Value = FUInt128Ex(0x8000000000000000ULL, 0ULL);
		Value.RightShift();
		TEST_TRUE_WITH_AUTONAME(Value == FUInt128Ex(0x4000000000000000ULL, 0ULL));

		Value = FUInt128Ex(0xFFFFFFFFFFFFFFFFULL, 0xFFFFFFFFFFFFFFFFULL);
		Value.RightShift();
		TEST_TRUE_WITH_AUTONAME(Value == FUInt128Ex(0x7FFFFFFFFFFFFFFFULL, 0xFFFFFFFFFFFFFFFFULL));
	}

	//Add
	{
		FUInt128Ex ValueA = FUInt128Ex(0ULL);
		FUInt128Ex ValueB = FUInt128Ex(0ULL);
		ValueA.Add(ValueB);
		TEST_TRUE_WITH_AUTONAME(ValueA.IsZero());

		ValueA = FUInt128Ex(0x123ULL);
		ValueB = FUInt128Ex(0x456ULL);
		FUInt128Ex ValueC = FUInt128Ex::Add(ValueA, ValueB);
		TEST_TRUE_WITH_AUTONAME(ValueC == FUInt128Ex(0ULL, 0x579ULL));
		ValueA.Add(ValueB);
		TEST_TRUE_WITH_AUTONAME(ValueA == ValueC);

		ValueA = FUInt128Ex(0ULL, 0xFFFFFFFFFFFFFFFFULL);
		ValueB = FUInt128Ex(0x1ULL);
		ValueA.Add(ValueB);
		TEST_TRUE_WITH_AUTONAME(ValueA == FUInt128Ex(1ULL, 0LL));

		ValueA = FUInt128Ex(0x1234ULL, 0ULL);
		ValueB = FUInt128Ex(0ULL, 0x5678ULL);
		ValueA.Add(ValueB);
		TEST_TRUE_WITH_AUTONAME(ValueA == FUInt128Ex(0x1234ULL, 0x5678ULL));

		ValueA = FUInt128Ex(0xFFFFFFFFFFFFFFFFULL, 0xFFFFFFFFFFFFFFFFULL);
		ValueB = FUInt128Ex(0x1ULL);
		ValueA.Add(ValueB);
		TEST_TRUE_WITH_AUTONAME(ValueA == FUInt128Ex(0ULL, 0ULL));
	}

	//Sub
	{
		FUInt128Ex ValueA = FUInt128Ex(0ULL);
		FUInt128Ex ValueB = FUInt128Ex(0ULL);
		ValueA.Sub(ValueB);
		TEST_TRUE_WITH_AUTONAME(ValueA.IsZero());

		ValueA = FUInt128Ex(0x456ULL);
		ValueB = FUInt128Ex(0x123ULL);
		FUInt128Ex ValueC = FUInt128Ex::Sub(ValueA, ValueB);
		TEST_TRUE_WITH_AUTONAME(ValueC == FUInt128Ex(0ULL, 0x333ULL));
		ValueA.Sub(ValueB);
		TEST_TRUE_WITH_AUTONAME(ValueA == ValueC);

		ValueA = FUInt128Ex(1ULL, 0ULL);
		ValueB = FUInt128Ex(1ULL);
		ValueA.Sub(ValueB);
		TEST_TRUE_WITH_AUTONAME(ValueA == FUInt128Ex(0ULL, 0xFFFFFFFFFFFFFFFFULL));

		ValueA = FUInt128Ex(0xFFFFFFFFFFFFFFFFULL, 0xFFFFFFFFFFFFFFFFULL);
		ValueB = FUInt128Ex(0x1ULL);
		ValueA.Sub(ValueB);
		TEST_TRUE_WITH_AUTONAME(ValueA == FUInt128Ex(0xFFFFFFFFFFFFFFFFULL, 0xFFFFFFFFFFFFFFFEULL));

		ValueA = FUInt128Ex(0xFFFFFFFFFFFFFFFFULL, 0xFFFFFFFFFFFFFFFFULL);
		ValueB = FUInt128Ex(0xFFFFFFFFFFFFFFFFULL, 0xFFFFFFFFFFFFFFFFULL);
		ValueA.Sub(ValueB);
		TEST_TRUE_WITH_AUTONAME(ValueA.IsZero());

		ValueA = FUInt128Ex(0ULL);
		ValueB = FUInt128Ex(1ULL);
		ValueA.Sub(ValueB);
		TEST_TRUE_WITH_AUTONAME(ValueA == FUInt128Ex(0xFFFFFFFFFFFFFFFFULL, 0xFFFFFFFFFFFFFFFFULL));
	}

	//MulModP
	{
		FUInt128Ex ValueA = FUInt128Ex(0ULL);
		FUInt128Ex ValueB = FUInt128Ex(0ULL);
		FUInt128Ex ValueC = FUInt128Ex::MulModP(ValueA, ValueB);
		TEST_TRUE_WITH_AUTONAME(ValueC.IsZero());

		ValueA = FUInt128Ex(0x456ULL);
		ValueB = FUInt128Ex(0x123ULL);
		ValueC = FUInt128Ex::MulModP(ValueA, ValueB);
		TEST_TRUE_WITH_AUTONAME(ValueC == FUInt128Ex(0ULL, 323010ULL));

		ValueA = FUInt128Ex(0x123ULL);
		ValueB = FUInt128Ex(0xFFFFFFFFFFFFFFFFULL, 0x456ULL);
		ValueC = FUInt128Ex::MulModP(ValueA, ValueB);
		TEST_TRUE_WITH_AUTONAME(ValueC == FUInt128Ex(0xfffffffffffffedd, 0x000000000005a1e0));

		ValueA = FUInt128Ex(0xFFFFFFFFFFFFFFFFULL, 0x456ULL);
		ValueB = FUInt128Ex(0xFFFFFFFFFFFFFFFFULL, 0x123ULL);
		ValueC = FUInt128Ex::MulModP(ValueA, ValueB);
		TEST_TRUE_WITH_AUTONAME(ValueC == FUInt128Ex(0xfffffffffffff949, 0x000000000008b6aa));
	}

	//PowerModP
	{
		FUInt128Ex ValueA = FUInt128Ex(2ULL);
		FUInt128Ex ValueB = FUInt128Ex(1ULL);
		FUInt128Ex ValueC = FUInt128Ex::PowerModP(ValueA, ValueB);
		TEST_TRUE_WITH_AUTONAME(ValueC == ValueA);

		ValueA = FUInt128Ex(0x123ULL);
		ValueB = FUInt128Ex(0x456ULL);
		ValueC = FUInt128Ex::PowerModP(ValueA, ValueB);
		TEST_TRUE_WITH_AUTONAME(ValueC == FUInt128Ex(0x651085792dd1313eULL, 0x5b550778601818aeULL));

		ValueA = FUInt128Ex(0xFFFFFFFFFFFFFFFFULL, 0xFFFFFFFFFFFFFFFEULL);
		ValueB = FUInt128Ex(0x456ULL);
		ValueC = FUInt128Ex::PowerModP(ValueA, ValueB);
		TEST_TRUE_WITH_AUTONAME(ValueC == FUInt128Ex(0x838b0ab9fdcfdecfULL, 0x5fb0295c033f790bULL));
	}

	// Convert to Array
	{
		FUInt128Ex Value = FUInt128Ex(0ULL);
		TArray<uint8> ArrayValue1 = Value.ToArray();
		TEST_TRUE_WITH_AUTONAME(ArrayValue1.Num() == 16);
		TEST_TRUE_WITH_AUTONAME(ArrayValue1 == TArray<uint8>({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }));

		Value = FUInt128Ex(0x466e256e79d823c2, 0x3e6afbad1918ca9b);
		TArray<uint8> ArrayValue2 = Value.ToArray();
		TEST_TRUE_WITH_AUTONAME(ArrayValue2.Num() == 16);
		TEST_TRUE_WITH_AUTONAME(ArrayValue2 == TArray<uint8>({ 0x46, 0x6e, 0x25, 0x6e, 0x79, 0xd8, 0x23, 0xc2, 0x3e, 0x6a, 0xfb, 0xad, 0x19, 0x18, 0xca, 0x9b }));
	}

	//Convert from Array
	{
		FUInt128Ex Value;
		Value.MakeFromArray(TArray<uint8>({ 0 }));
		TEST_TRUE_WITH_AUTONAME(Value.IsZero());

		Value.MakeFromArray(TArray<uint8>({ 0x12, 0x34, 0x56, 0x78 }));
		TEST_TRUE_WITH_AUTONAME(Value == FUInt128Ex(0ULL, 0x12345678ULL));

		Value.MakeFromArray(TArray<uint8>({ 0x21, 0x02, 0x0c, 0x4a, 0x4c, 0x94, 0x9f, 0xd1, 0x4d, 0x0c, 0xf2, 0xa2, 0x40, 0x2f, 0x2a, 0xff }));
		TEST_TRUE_WITH_AUTONAME(Value == FUInt128Ex(0x21020c4a4c949fd1ULL, 0x4d0cf2a2402f2affULL));

		Value.MakeFromArray(TArray<uint8>({ 0x50, 0x0c, 0x1b, 0xdb, 0xa6, 0x7f, 0x00, 0x68, 0x47, 0x15, 0xfa, 0x5c, 0xda, 0xf8, 0x27, 0x24,
			0xad, 0x75, 0x21, 0xda, 0x95, 0xe2, 0x7f, 0xc1 }));
		TEST_TRUE_WITH_AUTONAME(Value == FUInt128Ex(0x500c1bdba67f0068ULL, 0x4715fa5cdaf82724ULL));
	}

	//Convert to String
	{
		FUInt128Ex Value;
		FString HexString = Value.ToHexString();
		TEST_TRUE_WITH_AUTONAME(HexString == TEXT("00000000000000000000000000000000"));

		Value = FUInt128Ex(0x500c1bdba67f0068ULL, 0x4715fa5cdaf82724ULL);
		HexString = Value.ToHexString();
		TEST_TRUE_WITH_AUTONAME(HexString == TEXT("500c1bdba67f00684715fa5cdaf82724"));

		Value = FUInt128Ex::Zero;
		FString Base64String = Value.ToBase64();
		TEST_TRUE_WITH_AUTONAME(Base64String == TEXT("AAAAAAAAAAAAAAAAAAAAAA=="));

		Value = FUInt128Ex(0x500c1bdba67f0068ULL, 0x4715fa5cdaf82724ULL);
		Base64String = Value.ToBase64();
		TEST_TRUE_WITH_AUTONAME(Base64String == TEXT("UAwb26Z/AGhHFfpc2vgnJA=="));
	}

	//Convert from String
	{
		FUInt128Ex Value;

		Value.MakeFromHexString(TEXT(""));
		TEST_TRUE_WITH_AUTONAME(Value.IsZero());

		Value.MakeFromHexString(TEXT("1234"));
		TEST_TRUE_WITH_AUTONAME(Value == FUInt128Ex(0ULL, 0x1234ULL));

		Value.MakeFromHexString(TEXT("500c1bdba67f00684715fa5cdaf82724"));
		TEST_TRUE_WITH_AUTONAME(Value == FUInt128Ex(0x500c1bdba67f0068ULL, 0x4715fa5cdaf82724ULL));

		Value.MakeFromHexString(TEXT("C0DEHello"));
		TEST_TRUE_WITH_AUTONAME(Value == FUInt128Ex(0ULL, 0ULL));

		Value.MakeFromHexString(TEXT("ad7521da95e27fc1e96c4bcda7e650b612345678"));
		TEST_TRUE_WITH_AUTONAME(Value == FUInt128Ex(0xad7521da95e27fc1ULL, 0xe96c4bcda7e650b6ULL));

		Value.MakeFromBase64(TEXT(""));
		TEST_TRUE_WITH_AUTONAME(Value.IsZero());

		Value.MakeFromBase64(TEXT("UAwb26Z/AGhHFfpc2vgnJA=="));
		TEST_TRUE_WITH_AUTONAME(Value == FUInt128Ex(0x500c1bdba67f0068ULL, 0x4715fa5cdaf82724ULL));

		Value.MakeFromBase64(TEXT("Hello"));
		TEST_TRUE_WITH_AUTONAME(Value.IsZero());
	}
	return true;
}

bool TestTinyEncryptExchange(FString& Detail)
{
	//Exchange
	{
		for (int i = 0; i < 10; i++)
		{
			FDiffieHellmanKeyPair Alice, Bob;
			Alice.GenerateRandomKeyPair();
			Bob.GenerateRandomKeyPair();

			FUInt128Ex SecretKeyA = Alice.GenerateSecretKey(Bob.PublicKey);
			FUInt128Ex SecretKeyB = Bob.GenerateSecretKey(Alice.PublicKey);

			TEST_TRUE_WITH_AUTONAME(SecretKeyA == SecretKeyB);
		}
	}
	return true;
}

#endif //!UE_BUILD_SHIPPING

#if WITH_DEV_AUTOMATION_TESTS && !UE_BUILD_SHIPPING

#if (ENGINE_MAJOR_VERSION >= 5 && ENGINE_MINOR_VERSION >= 5)
IMPLEMENT_CUSTOM_SIMPLE_AUTOMATION_TEST(FTinyEncryptTestUInt128, FAutomationTestBase, "TinyEncrypt.UInt128", EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::EngineFilter)
#else
IMPLEMENT_CUSTOM_SIMPLE_AUTOMATION_TEST(FTinyEncryptTestUInt128, FAutomationTestBase, "TinyEncrypt.UInt128", EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::EngineFilter)
#endif
bool FTinyEncryptTestUInt128::RunTest(const FString& Parameters)
{
	FString Detail;
	bool bSuccess = TestTinyEncryptUInt128(Detail);
	TestTrue(Detail, bSuccess);
	return true;
}

#if (ENGINE_MAJOR_VERSION >= 5 && ENGINE_MINOR_VERSION >= 5)
IMPLEMENT_CUSTOM_SIMPLE_AUTOMATION_TEST(FTinyEncryptTestKeyExchange, FAutomationTestBase, "TinyEncrypt.KeyExchange", EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::EngineFilter)
#else
IMPLEMENT_CUSTOM_SIMPLE_AUTOMATION_TEST(FTinyEncryptTestKeyExchange, FAutomationTestBase, "TinyEncrypt.KeyExchange", EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::EngineFilter)
#endif
bool FTinyEncryptTestKeyExchange::RunTest(const FString& Parameters)
{
	FString Detail;
	bool bSuccess = TestTinyEncryptExchange(Detail);
	TestTrue(Detail, bSuccess);

	return true;
}

#endif  // WITH_DEV_AUTOMATION_TESTS && !UE_BUILD_SHIPPING
