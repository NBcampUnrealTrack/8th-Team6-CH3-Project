// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/ANS_SetActionState.h"
#include "TP_ThirdPerson/LunarAsylumCharacter.h"

void UANS_SetActionState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

    if (MeshComp && MeshComp->GetOwner())
    {
        if (ALunarAsylumCharacter* LunarAsylumCharacter = Cast<ALunarAsylumCharacter>(MeshComp->GetOwner()))
        {
            LunarAsylumCharacter->SetActionState(StateToSet);
        }
    }
}

void UANS_SetActionState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    Super::NotifyEnd(MeshComp, Animation);

    if (MeshComp && MeshComp->GetOwner())
    {
        if (ALunarAsylumCharacter* LunarAsylumCharacter = Cast<ALunarAsylumCharacter>(MeshComp->GetOwner()))
        {
            LunarAsylumCharacter->RestorePreviousState();
        }
    }
}
