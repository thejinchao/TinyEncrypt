// Copyright (C) 2024 Neo Jin. All Rights Reserved.
#include "TinyEncryptUtilities.h"
#include "TinyEncryptAlgorithm.h"

FUInt128Ex UTinyEncryptUtilities::MakeRandomUInt128()
{
	FUInt128Ex Value;
	Value.MakeRandom();
	return Value;
}

TArray<uint8> UTinyEncryptUtilities::UInt128ToArray(const FUInt128Ex& Value)
{
	return Value.ToArray();
}

FUInt128Ex UTinyEncryptUtilities::UInt128FromArray(const TArray<uint8>& Value)
{
	FUInt128Ex Output;
	Output.MakeFromArray(Value);
	return Output;
}

FString UTinyEncryptUtilities::UInt128ToString(const FUInt128Ex& Value, EUInt128StringType StringType)
{
	FString Output;
	switch (StringType)
	{
	case EUInt128StringType::Hex:
		Output = Value.ToHexString();
		break;

	case EUInt128StringType::Base64:
		Output = Value.ToBase64();
		break;
	}
	return Output;
}

FUInt128Ex UTinyEncryptUtilities::UInt128FromString(const FString& Value, EUInt128StringType StringType)
{
	FUInt128Ex Output;
	switch (StringType)
	{
	case EUInt128StringType::Hex:
	{
		Output.MakeFromHexString(Value);
	}
	break;

	case EUInt128StringType::Base64:
	{
		Output.MakeFromBase64(Value);
	}
	break;
	}

	return Output;
}

bool UTinyEncryptUtilities::EqualEqual_UInt128(const FUInt128Ex& A, const FUInt128Ex& B)
{
	return A == B;
}

TArray<uint8> UTinyEncryptUtilities::EncryptWithTEA(const TArray<uint8>& InputData, const FUInt128Ex& Key)
{
	FTinyEncrypt TEA(Key);

	int32 InputLen = InputData.Num();
	const uint8* InputDataPtr = InputData.GetData();
	const int32 EncryptLength = FTinyEncrypt::GetEncryptLength(InputLen);

	TArray<uint8> OutputData;
	OutputData.Init(0, EncryptLength);
	TEA.Encrypt(InputDataPtr, InputLen, OutputData.GetData());

	return OutputData;
}

TArray<uint8> UTinyEncryptUtilities::DecryptWithTEA(const TArray<uint8>& InputData, const FUInt128Ex& Key)
{
	FTinyEncrypt TEA(Key);

	int32 InputLen = InputData.Num();
	const uint8* InputDataPtr = InputData.GetData();
	const int32 DecryptLength = FTinyEncrypt::GetDecryptLength(InputLen);

	TArray<uint8> OutputData;
	OutputData.Init(0, DecryptLength);
	int32 OutputLength = 0;
	if (InputDataPtr != nullptr && InputLen > 0)
	{
		OutputLength = TEA.Decrypt(InputDataPtr, InputLen, OutputData.GetData());
	}

	OutputData.SetNum(OutputLength);
	return OutputData;
}
