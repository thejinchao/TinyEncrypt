#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "TinyEncryptGameInstance.generated.h"


UCLASS()
class TINYENCRYPTDEMO_API UTinyEncryptGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	virtual void Init() override;

	UFUNCTION(BlueprintCallable)
	FString PrintBinaryData(const TArray<uint8>& InputArray, int32 MaxLength=16);

	UFUNCTION(BlueprintCallable)
	TArray<uint8> StringToBinaryData(const FString& InputData);

	UFUNCTION(BlueprintCallable)
	FString BinaryDataToString(const TArray<uint8>& InputData);

	UFUNCTION(BlueprintCallable)
	bool UnitTest(FString& Detail);
};
