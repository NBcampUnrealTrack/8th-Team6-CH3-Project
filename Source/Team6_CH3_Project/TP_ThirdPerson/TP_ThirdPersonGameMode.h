// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TP_ThirdPersonGameMode.generated.h"

UCLASS(minimalapi)
class ATP_ThirdPersonGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ATP_ThirdPersonGameMode();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stage Settings")
	int32 CurrentKillCount;
};



