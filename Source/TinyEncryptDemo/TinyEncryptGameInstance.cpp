#include "TinyEncryptGameInstance.h"
#include "AutomationTest/TinyEncryptAutomationTestInterface.h"

void UTinyEncryptGameInstance::Init()
{
    Super::Init();
}

FString UTinyEncryptGameInstance::PrintBinaryData(const TArray<uint8>& InputArray, int32 MaxLength)
{
    FString Output;
    int32 Length = 0;
    for (uint8 Data : InputArray)
    {
        Output += FString::Printf(TEXT("%02x "), Data);
        if (++Length >= MaxLength)
        {
            Output += TEXT("...");
            break;
        }
    }
    return Output;
}

TArray<uint8> UTinyEncryptGameInstance::StringToBinaryData(const FString& InputData)
{
#if ENGINE_MAJOR_VERSION>=5
    auto StringConverter = StringCast<UTF8CHAR>(*InputData);
#else
    auto StringConverter = FTCHARToUTF8(*InputData);
#endif

    TArray<uint8> Output;
    Output.Init(0, StringConverter.Length());

    FMemory::Memcpy(Output.GetData(), StringConverter.Get(), StringConverter.Length());
    return Output;
}

FString UTinyEncryptGameInstance::BinaryDataToString(const TArray<uint8>& InputData)
{
#if ENGINE_MAJOR_VERSION>=5
    return FString((TCHAR*)StringCast<TCHAR>((const UTF8CHAR*)InputData.GetData()).Get());
#else
    return FString(UTF8_TO_TCHAR(InputData.GetData()));
#endif
}

bool UTinyEncryptGameInstance::UnitTest(FString& Detail)
{
#if !UE_BUILD_SHIPPING
    if (!TestTinyEncryptUInt128(Detail)) return false;
    if (!TestTinyEncryptExchange(Detail)) return false;
    if (!TestTinyEncryptEncrypt(Detail)) return false;
#endif
    return true;
}
