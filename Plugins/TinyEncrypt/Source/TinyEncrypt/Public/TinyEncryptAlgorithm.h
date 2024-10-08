// Copyright (C) 2024 Neo Jin. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "TinyEncryptKeyExchange.h"

/*
The Tiny Encryption Algorithm(TEA) Implementation
*/
class TINYENCRYPT_API FTinyEncrypt
{
private:
	uint32 Key[4];	//Encrypt or Decrypt key

public:
	static int32 GetEncryptLength(int32 InLen);
	static int32 GetDecryptLength(int32 InLen);

	//Encrypt data, the length of output buf should get from `GetEncryptLength`
	int32 Encrypt(const uint8* InBuf, int32 InLen, uint8* OutBuf);
	//Decrypt data
	int32 Decrypt(const uint8* InBuf, int32 InLen, uint8* OutBuf);
private:
	//Encrypt data block(8 bytes)
	void EncryptBlock(const uint8* InBuf, uint8* OutBuf);
	//Decrypt data block(8 bytes)
	void DecryptBlock(const uint8* InBuf, uint8* OutBuf);

public:
	FTinyEncrypt(const FUInt128Ex& _Key);

	// Default constructors.
	FORCEINLINE FTinyEncrypt(const FTinyEncrypt&) = default;
	FORCEINLINE FTinyEncrypt(FTinyEncrypt&&) = default;
	FORCEINLINE FTinyEncrypt& operator=(FTinyEncrypt const&) = default;
	FORCEINLINE FTinyEncrypt& operator=(FTinyEncrypt&&) = default;
};
