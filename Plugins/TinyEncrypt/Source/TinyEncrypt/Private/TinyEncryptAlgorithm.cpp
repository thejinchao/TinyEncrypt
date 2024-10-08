// Copyright (C) 2024 Neo Jin. All Rights Reserved.
#include "TinyEncryptAlgorithm.h"

FTinyEncrypt::FTinyEncrypt(const FUInt128Ex& _Key)
{
	Key[0] = (uint32)(_Key.LowPart() & 0xFFFFFFFFULL);
	Key[1] = (uint32)((_Key.LowPart() >> 32) & 0xFFFFFFFFULL);
	Key[2] = (uint32)(_Key.HiPart() & 0xFFFFFFFFULL);
	Key[3] = (uint32)((_Key.HiPart() >> 32) & 0xFFFFFFFFULL);
}

int32 FTinyEncrypt::GetEncryptLength(int32 InLen)
{
	int32 Blocks = InLen / 8;
	return Blocks * 8 + 8;
}

int32 FTinyEncrypt::GetDecryptLength(int32 InLen)
{
	return InLen;
}

void FTinyEncrypt::EncryptBlock(const uint8* InBuf, uint8* OutBuf)
{
	static const uint32 Delta = 0x9E3779B9;	//(sqrt(5)-1)/2*2^32

	uint32 v0 = (((uint32)InBuf[0]) << 24) | (((uint32)InBuf[1]) << 16) | (((uint32)InBuf[2]) << 8) | ((uint32)InBuf[3]);
	uint32 v1 = (((uint32)InBuf[4]) << 24) | (((uint32)InBuf[5]) << 16) | (((uint32)InBuf[6]) << 8) | ((uint32)InBuf[7]);

	uint32 Sum = 0;
	for (int32 i = 0; i < 32; ++i)
	{
		v0 += (((v1 << 4) ^ (v1 >> 5)) + v1) ^ (Sum + Key[Sum & 3]);
		Sum += Delta;
		v1 += (((v0 << 4) ^ (v0 >> 5)) + v0) ^ (Sum + Key[(Sum >> 11) & 3]);
	}

	uint16 o1 = (uint16)((v0 >> 16) & 0xFFFF);
	uint16 o2 = (uint16)(v0 & 0xFFFF);
	uint16 o3 = (uint16)((v1 >> 16) & 0xFFFF);
	uint16 o4 = (uint16)(v1 & 0xFFFF);

	OutBuf[0] = (uint8)(o1 >> 8);
	OutBuf[1] = (uint8)(o1 & 0xFF);
	OutBuf[2] = (uint8)(o2 >> 8);
	OutBuf[3] = (uint8)(o2 & 0xFF);
	OutBuf[4] = (uint8)(o3 >> 8);
	OutBuf[5] = (uint8)(o3 & 0xFF);
	OutBuf[6] = (uint8)(o4 >> 8);
	OutBuf[7] = (uint8)(o4 & 0xFF);
}

void FTinyEncrypt::DecryptBlock(const uint8* InBuf, uint8* OutBuf)
{
	static const uint32 Delta = 0x9E3779B9;	//A key of schedule constant: (sqrt(5)-1)*2^31
	uint32 Sum = Delta * 32;

	uint32 v0 = (((uint32)InBuf[0]) << 24) | (((uint32)InBuf[1]) << 16) | (((uint32)InBuf[2]) << 8) | ((uint32)InBuf[3]);
	uint32 v1 = (((uint32)InBuf[4]) << 24) | (((uint32)InBuf[5]) << 16) | (((uint32)InBuf[6]) << 8) | ((uint32)InBuf[7]);

	for (int32 i = 0; i < 32; i++) 
	{
		v1 -= (((v0 << 4) ^ (v0 >> 5)) + v0) ^ (Sum + Key[(Sum >> 11) & 3]);
		Sum -= Delta;
		v0 -= (((v1 << 4) ^ (v1 >> 5)) + v1) ^ (Sum + Key[Sum & 3]);
	}

	uint16 o1 = (uint16)((v0 >> 16) & 0xFFFF);
	uint16 o2 = (uint16)(v0 & 0xFFFF);
	uint16 o3 = (uint16)((v1 >> 16) & 0xFFFF);
	uint16 o4 = (uint16)(v1 & 0xFFFF);

	OutBuf[0] = (uint8)(o1 >> 8);
	OutBuf[1] = (uint8)(o1 & 0xFF);
	OutBuf[2] = (uint8)(o2 >> 8);
	OutBuf[3] = (uint8)(o2 & 0xFF);
	OutBuf[4] = (uint8)(o3 >> 8);
	OutBuf[5] = (uint8)(o3 & 0xFF);
	OutBuf[6] = (uint8)(o4 >> 8);
	OutBuf[7] = (uint8)(o4 & 0xFF);
}

int32 FTinyEncrypt::Encrypt(const uint8* InBuf, int32 InLen, uint8* OutBuf)
{
	int32 BlockCounts = InLen / 8;
	int32 BlockBytes = BlockCounts * 8;

	for (int32 i = 0; i < BlockBytes; i += 8)
	{
		EncryptBlock(InBuf + i, OutBuf + i);
	}

	int32 PadLen = 8 - (InLen - BlockBytes);

	//fill tail buf(last data and pad length)
	uint8 TailBuff[8] = { 0 };
	if (PadLen < 8)
	{
		FMemory::Memcpy(TailBuff, InBuf + BlockBytes, 8 - PadLen);
	}
	FMemory::Memset(TailBuff + (8 - PadLen), PadLen, PadLen);
	EncryptBlock(TailBuff, OutBuf + BlockBytes);

	return BlockBytes + 8;
}

int32 FTinyEncrypt::Decrypt(const uint8* InBuf, int32 InLen, uint8* OutBuf)
{
	int32 BlockCounts = InLen / 8;
	int32 BlockBytes = (BlockCounts - 1) * 8;

	for (int32 i = 0; i < BlockBytes; i += 8)
	{
		DecryptBlock(InBuf + i, OutBuf + i);
	}

	uint8 TailBuff[8] = { 0 };
	DecryptBlock(InBuf + BlockBytes, TailBuff);

	int32 PadLen = (int32)TailBuff[8 - 1];

	if (PadLen < 8)
	{
		FMemory::Memcpy(OutBuf + BlockBytes, TailBuff, 8 - PadLen);
	}
	return BlockBytes + (8 - PadLen);
}
