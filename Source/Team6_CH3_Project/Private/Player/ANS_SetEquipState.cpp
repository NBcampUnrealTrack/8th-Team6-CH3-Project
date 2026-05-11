// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/ANS_SetEquipState.h"
#include "TP_ThirdPerson/LunarAsylumCharacter.h"

void UANS_SetEquipState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
    Super::NotifyBegin(MeshComp, Animation, TotalDuration);

    if (MeshComp && MeshComp->GetOwner())
    {
        if (ALunarAsylumCharacter* LunarAsylumCharacter = Cast<ALunarAsylumCharacter>(MeshComp->GetOwner()))
        {
            LunarAsylumCharacter->SetEquipState(StateToSet);
        }
    }
}
