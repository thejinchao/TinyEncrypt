// Copyright (C) 2024 Neo Jin. All Rights Reserved.
#pragma once

#if !UE_BUILD_SHIPPING

#define TEST_TRUE_WITH_AUTONAME(Value) if(!(Value)) { \
	Detail = FString::Printf(TEXT("Failed at '%s.cpp' Line: %d"), *FPaths::GetBaseFilename(ANSI_TO_TCHAR(__FILE__)), __LINE__); \
	return false; \
}

bool TINYENCRYPT_API TestTinyEncryptUInt128(FString& Detail);
bool TINYENCRYPT_API TestTinyEncryptExchange(FString& Detail);
bool TINYENCRYPT_API TestTinyEncryptEncrypt(FString& Detail);

#endif
