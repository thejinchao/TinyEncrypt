// Copyright (C) 2024 Neo Jin. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "TinyEncryptKeyExchange.h"
#include "TinyEncryptUtilities.generated.h"

UENUM(BlueprintType)
enum class EUInt128StringType : uint8
{
	Hex,
	Base64
};

UCLASS()
class TINYENCRYPT_API UTinyEncryptUtilities : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "TinyEncrypt", DisplayName = "Make Random UInt128")
	static FUInt128Ex MakeRandomUInt128();

	UFUNCTION(BlueprintPure, Category = "TinyEncrypt", DisplayName = "UInt128 to Array")
	static TArray<uint8> UInt128ToArray(const FUInt128Ex& Value);

	UFUNCTION(BlueprintPure, Category = "TinyEncrypt", DisplayName = "UInt128 from Array")
	static FUInt128Ex UInt128FromArray(const TArray<uint8>& Value);

	UFUNCTION(BlueprintPure, Category = "TinyEncrypt", DisplayName = "UInt128 to String")
	static FString UInt128ToString(const FUInt128Ex& Value, EUInt128StringType StringType);

	UFUNCTION(BlueprintPure, Category = "TinyEncrypt", DisplayName = "UInt128 from String")
	static FUInt128Ex UInt128FromString(const FString& Value, EUInt128StringType StringType);

	// Returns true if A is equal to B (A == B)
	UFUNCTION(BlueprintPure, Category = "TinyEncrypt", meta = (DisplayName = "Equal (UInt128)", CompactNodeTitle = "==", Keywords = "== equal", ScriptOperator = "=="))
	static bool EqualEqual_UInt128(const FUInt128Ex& A, const FUInt128Ex& B);

	UFUNCTION(BlueprintPure, Category = "TinyEncrypt", DisplayName = "Make Random DH KeyPair")
	static FDiffieHellmanKeyPair MakeRandomDHKeyPair()
	{
		FDiffieHellmanKeyPair KeyPair;
		KeyPair.GenerateRandomKeyPair();
		return KeyPair;
	}

	UFUNCTION(BlueprintPure, Category = "TinyEncrypt", DisplayName = "Break DH KeyPair")
	static void BreakDHKeyPair(const FDiffieHellmanKeyPair& KeyPair, FUInt128Ex& PublicKey, FUInt128Ex& PrivateKey)
	{
		PublicKey = KeyPair.PublicKey;
		PrivateKey = KeyPair.PrivateKey;
	}

	UFUNCTION(BlueprintCallable, Category = "TinyEncrypt", DisplayName = "Generate DH Secret Key")
	static FUInt128Ex GenerateDHSecretKey(const FUInt128Ex& PrivateKey, const FUInt128Ex& AnotherPublicKey)
	{
		//secret_key = other_key^prv_key mod P
		return FUInt128Ex::PowerModP(AnotherPublicKey, PrivateKey);
	}

	UFUNCTION(BlueprintCallable, Category = "TinyEncrypt", DisplayName = "Encrypt With TEA")
	static TArray<uint8> EncryptWithTEA(const TArray<uint8>& InputData, const FUInt128Ex& Key);

	UFUNCTION(BlueprintCallable, Category = "TinyEncrypt", DisplayName = "Decrypt With TEA")
	static TArray<uint8> DecryptWithTEA(const TArray<uint8>& InputData, const FUInt128Ex& Key);
};
