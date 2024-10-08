// Copyright (C) 2024 Neo Jin. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "TinyEncryptKeyExchange.generated.h"


USTRUCT(BlueprintType)
struct TINYENCRYPT_API FUInt128Ex
{
	GENERATED_BODY()

private:
	uint64 Hi;
	uint64 Lo;

public:
	// Default constructors.
	FORCEINLINE FUInt128Ex(const FUInt128Ex&) = default;
	FORCEINLINE FUInt128Ex(FUInt128Ex&&) = default;
	FORCEINLINE FUInt128Ex& operator=(FUInt128Ex const&) = default;
	FORCEINLINE FUInt128Ex& operator=(FUInt128Ex&&) = default;

	FORCEINLINE FUInt128Ex() : Hi(0), Lo(0) { }
	FORCEINLINE FUInt128Ex(uint64 A) : Hi(0), Lo(A) { }
	FORCEINLINE FUInt128Ex(uint64 A, uint64 B) : Hi(A), Lo(B) { }

	// Constructor. Initializes this uint128 with four uint32 values.
	FORCEINLINE FUInt128Ex(uint32 A, uint32 B, uint32 C, uint32 D)
		: Hi(((uint64)A << 32) | B)
		, Lo(((uint64)C << 32) | D)
	{}

	// Make a random uint128
	void MakeRandom();

	uint64 HiPart() const { return Hi; }
	uint64 LowPart() const { return Lo; }
public:
	FORCEINLINE bool IsZero() const
	{
		return Hi == 0 && Lo == 0;
	}

	FORCEINLINE bool IsOdd() const
	{
		return (Lo & 1) != 0;
	}

	FORCEINLINE void LeftShift()
	{
		uint64 Temp = (Lo >> 63) & 1;
		Hi = (Hi << 1) | Temp;
		Lo = Lo << 1;
	}

	FORCEINLINE void RightShift()
	{
		uint64 Temp = (Hi & 1) << 63;
		Hi = Hi >> 1;
		Lo = (Lo >> 1) | Temp;
	}

public:
	 // return  1 : a>b
	 // return  0 : a==b
	 // return -1 : a<b
	int32 Compare(const FUInt128Ex& Other) const;

	// Comparison operators
	FORCEINLINE bool operator>(const FUInt128Ex& Other) const
	{
		return Compare(Other) > 0;
	}

	FORCEINLINE bool operator>=(const FUInt128Ex& Other) const
	{
		return Compare(Other) >= 0;
	}

	FORCEINLINE bool operator==(const FUInt128Ex& Other) const
	{
		return Compare(Other) == 0;
	}

	FORCEINLINE bool operator<(const FUInt128Ex& Other) const
	{
		return Compare(Other) < 0;
	}

	FORCEINLINE bool operator<=(const FUInt128Ex& Other) const
	{
		return Compare(Other) <= 0;
	}

public:
	void Add(const FUInt128Ex& Other);
	void Sub(const FUInt128Ex& Other);

public:
	FORCEINLINE static int32 Compare(const FUInt128Ex& A, const FUInt128Ex& B)
	{
		return A.Compare(B);
	}
	FORCEINLINE static FUInt128Ex Add(const FUInt128Ex& A, const FUInt128Ex& B)
	{
		FUInt128Ex Result(A);
		Result.Add(B);
		return Result;
	}
	FORCEINLINE static FUInt128Ex Sub(const FUInt128Ex& A, const FUInt128Ex& B)
	{
		FUInt128Ex Result(A);
		Result.Sub(B);
		return Result;
	}
	//return A*B mod P
	static FUInt128Ex MulModP(FUInt128Ex A, FUInt128Ex B);
	//return A^B mod P(Reduce)
	static FUInt128Ex PowerModPReduce(const FUInt128Ex& A, const FUInt128Ex& B);
	//return A^B mod P
	static FUInt128Ex PowerModP(FUInt128Ex A, const FUInt128Ex& B);

	// Serialization
	friend FArchive& operator<<(FArchive& Ar, FUInt128Ex& Value)
	{
		return Ar << Value.Hi << Value.Lo;
	}

public:
	// Convert to TArray<uint8>
	TArray<uint8> ToArray() const;
	// Make from TArray<uint8>
	void MakeFromArray(const TArray<uint8>& Array);
	
	// Conver to HexString
	FString ToHexString() const;
	// Make from HexString
	void MakeFromHexString(const FString& HexString);

	// Convert to Base64
	FString ToBase64() const;
	// Make from Base64
	void MakeFromBase64(const FString& Base64String);

public:
	static const FUInt128Ex Zero;		//Zero value

	//Special value for Diffie¨CHellman algorithm
	static const FUInt128Ex P;			// The biggest 64bit prime, P =  2^128-159 = 0xffffffffffffffffffffffffffffff61
	static const FUInt128Ex INVERT_P;	// INVERT_P =  2^128 - P
	static const FUInt128Ex G;			// A small prime number G = 5
};

USTRUCT(BlueprintType, meta = (
	HasNativeMake  = "TinyEncrypt.TinyEncryptUtilities.MakeRandomDHKeyPair",
	HasNativeBreak = "TinyEncrypt.TinyEncryptUtilities.BreakDHKeyPair"))
struct TINYENCRYPT_API FDiffieHellmanKeyPair
{
	GENERATED_BODY()

public:
	FUInt128Ex PublicKey;
	FUInt128Ex PrivateKey;

public:
	FORCEINLINE void GenerateRandomKeyPair()
	{
		//Generate random private key
		PrivateKey.MakeRandom();

		//Generate public key from private key 
		// PublicKey = G^PrivateKey mod P
		PublicKey = FUInt128Ex::PowerModP(FUInt128Ex::G, PrivateKey);
	}

	FORCEINLINE FUInt128Ex GenerateSecretKey(const FUInt128Ex& AnotherPublicKey)
	{
		// SecretKey = AnotherPublicKey^PrivateKey mod P
		return FUInt128Ex::PowerModP(AnotherPublicKey, PrivateKey);
	}

public:
	// Default constructors.
	FDiffieHellmanKeyPair() : PublicKey(0ULL), PrivateKey(0ULL) {}
	FDiffieHellmanKeyPair(const FUInt128Ex& PubKey, const FUInt128Ex& PriKey) : PublicKey(PubKey), PrivateKey(PriKey) {}

	FORCEINLINE FDiffieHellmanKeyPair(const FDiffieHellmanKeyPair&) = default;
	FORCEINLINE FDiffieHellmanKeyPair(FDiffieHellmanKeyPair&&) = default;
	FORCEINLINE FDiffieHellmanKeyPair& operator=(FDiffieHellmanKeyPair const&) = default;
	FORCEINLINE FDiffieHellmanKeyPair& operator=(FDiffieHellmanKeyPair&&) = default;
};
