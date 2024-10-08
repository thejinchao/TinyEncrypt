// Copyright (C) 2024 Neo Jin. All Rights Reserved.
#include "TinyEncryptKeyExchange.h"
#include "Misc/Base64.h"

const FUInt128Ex FUInt128Ex::Zero = FUInt128Ex(0ULL, 0ULL);
const FUInt128Ex FUInt128Ex::P = FUInt128Ex(0xffffffffffffffffULL, 0xffffffffffffff61ULL);
const FUInt128Ex FUInt128Ex::INVERT_P = FUInt128Ex(0ULL, 159ULL);
const FUInt128Ex FUInt128Ex::G = FUInt128Ex(0ULL, 5ULL);

FORCEINLINE uint32 RandomUInt32()
{
	return (uint32)(((uint32)FMath::RandRange(0, 0xFFFF)) << 16) + (uint32)(FMath::RandRange(0, 0xFFFF));
}

void FUInt128Ex::MakeRandom()
{
	Hi = ((uint64)RandomUInt32() << 32) | (uint64)RandomUInt32();
	Lo = ((uint64)RandomUInt32() << 32) | (uint64)RandomUInt32();
}

int32 FUInt128Ex::Compare(const FUInt128Ex& Other) const
{
	if (Hi > Other.Hi)
	{
		return 1;
	}
	else if (Hi == Other.Hi)
	{
		if (Lo > Other.Lo) return 1;
		else if (Lo == Other.Lo) return 0;
		else return -1;
	}
	else
	{
		return -1;
	}
}

void FUInt128Ex::Add(const FUInt128Ex& Other)
{
	uint64 Overflow = 0;
	uint64 Low = Lo + Other.Lo;
	if (Low < Lo || Low < Other.Lo) {
		Overflow = 1;
	}
	Hi = Hi + Other.Hi + Overflow;
	Lo = Low;
}

void FUInt128Ex::Sub(const FUInt128Ex& Other)
{
	FUInt128Ex InvertB(~Other.Hi, ~Other.Lo);
	InvertB.Add(FUInt128Ex(0ULL, 1ULL));

	Add(InvertB);
}

FUInt128Ex FUInt128Ex::MulModP(FUInt128Ex A, FUInt128Ex B)
{
	FUInt128Ex Result;

	while (!B.IsZero()) 
	{
		if (B.IsOdd()) 
		{
			FUInt128Ex Temp = Sub(P, A);

			if (Result >= Temp)
			{
				Result.Sub(Temp);
			}
			else 
			{
				Result.Add(A);
			}
		}

		FUInt128Ex DoubleA = A;
		DoubleA.LeftShift();

		FUInt128Ex P_A = Sub(P, A);

		if (A >= P_A)
		{
			A = Add(DoubleA, INVERT_P);
		}
		else 
		{
			A = DoubleA;
		}

		B.RightShift();
	}
	return Result;
}

FUInt128Ex FUInt128Ex::PowerModPReduce(const FUInt128Ex& A, const FUInt128Ex& B)
{
	if (B.Hi == 0 && B.Lo == 1)
	{
		return A;
	}

	FUInt128Ex HalfB = B;
	HalfB.RightShift();

	FUInt128Ex Temp = PowerModPReduce(A, HalfB);
	Temp = MulModP(Temp, Temp);

	if (B.IsOdd())
	{
		Temp = MulModP(Temp, A);
	}
	return Temp;
}

FUInt128Ex FUInt128Ex::PowerModP(FUInt128Ex A, const FUInt128Ex& B)
{
	if (Compare(A, P) > 0)
	{
		A = Sub(A, P);
	}

	return PowerModPReduce(A, B);
}

TArray<uint8> FUInt128Ex::ToArray() const
{
	TArray<uint8> Output;
	Output.Init(0, 16);

	if (FGenericPlatformProperties::IsLittleEndian())
	{
		uint64 BigEndianHi = BYTESWAP_ORDER64(Hi);
		FMemory::Memcpy(Output.GetData(), (const void*)(&BigEndianHi), sizeof(uint64));

		uint64 BigEndianLo = BYTESWAP_ORDER64(Lo);
		FMemory::Memcpy(Output.GetData() + sizeof(uint64), (const void*)(&BigEndianLo), sizeof(uint64));
	}
	else
	{
		FMemory::Memcpy(Output.GetData(), (const void*)(&Hi), sizeof(uint64));
		FMemory::Memcpy(Output.GetData() + sizeof(uint64), (const void*)(&Lo), sizeof(uint64));
	}
	return Output;
}

void FUInt128Ex::MakeFromArray(const TArray<uint8>& Array)
{
	//reset
	Hi = Lo = 0;
	
	int32 ArrayLen = Array.Num();
	if (ArrayLen > 16)
	{
		//Error!, too many data, ignore data at the tail 
		ArrayLen = 16;
	}

	int32 Index = ArrayLen - 1;
	for (uint8 Data : Array)
	{
		if (Index < 0) break;
		if (Index > 7) 
		{
			Hi += (((uint64)Data) << (Index - 8) * 8);
		}
		else
		{
			Lo += (((uint64)Data) << Index * 8);
		}
		Index--;
	}
}

FString FUInt128Ex::ToHexString() const
{
	return FString::Printf(TEXT("%016llx%016llx"), HiPart(), LowPart());
}

void FUInt128Ex::MakeFromHexString(const FString& HexString)
{
	//reset
	Hi = Lo = 0;

	int32 CharLen = HexString.Len();
	if (CharLen > 32)
	{
		CharLen = 32; //Invalid format, ignore data at the tail
	}

	uint64 _Hi = 0, _Lo = 0;
	int32 Index = CharLen - 1;
	for (auto It = HexString.CreateConstIterator(); It; ++It)
	{
		if (Index < 0) break;
		TCHAR C = *It;
		uint64 V = 0;
		if (C >= '0' && C <= '9') { V = (uint64)(C - '0'); }
		else if (C >= 'a' && C <= 'f') { V = (uint64)(C - 'a' + 10); }
		else if (C >= 'A' && C <= 'F') { V = (uint64)(C - 'A' + 10); }
		else
		{
			//Invalid format, return zero
			return;
		}
		if (Index > 15)
		{
			_Hi += V << ((Index - 16) * 4);
		}
		else
		{
			_Lo += V << Index * 4;
		}
		Index--;
	}

	Hi = _Hi;
	Lo = _Lo;
}

FString FUInt128Ex::ToBase64() const
{
	return FBase64::Encode(ToArray());
}

void FUInt128Ex::MakeFromBase64(const FString& Base64String)
{
	//reset
	Hi = Lo = 0;

	TArray<uint8> DataArray;
	if (!FBase64::Decode(Base64String, DataArray))
	{
		//invalid format
		return;
	}
	MakeFromArray(DataArray);
}
