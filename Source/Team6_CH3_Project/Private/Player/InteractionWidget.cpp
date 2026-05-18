// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/InteractionWidget.h"
#include "TP_ThirdPerson/LunarAsylumCharacter.h"

void UInteractionWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (APawn* OwningPawn = GetOwningPlayerPawn())
    {
        LunarAsylumCharacter = Cast<ALunarAsylumCharacter>(OwningPawn);
    }

    if (LunarAsylumCharacter)
    {
        UE_LOG(LogTemp, Log, TEXT("LunarAsylumCharacter"));
        if (GuideText)
        {
            GuideText->SetVisibility(ESlateVisibility::Collapsed);
        }

        if (AcquisitionText)
        {
            AcquisitionText->SetVisibility(ESlateVisibility::Collapsed);
        }

        LunarAsylumCharacter->OnTargetItemChanged.AddDynamic(this, &UInteractionWidget::HandleTargetItemChanged);
        LunarAsylumCharacter->OnItemAcquired.AddDynamic(this, &UInteractionWidget::ShowAcquisitionMessage);
    }
    else
    {
        UE_LOG(LogTemp, Log, TEXT("LunarAsylumCharacter is NULL"));
    }
}

void UInteractionWidget::ShowGuide(const FString& ItemName)
{
    if (GuideText)
    {
        GuideText->SetText(FText::FromString(FString::Printf(TEXT("[F] %s"), *ItemName)));
        GuideText->SetVisibility(ESlateVisibility::Visible);
    }

    UE_LOG(LogTemp, Log, TEXT("ShowGuide"));
}

void UInteractionWidget::HideGuide()
{
    if (GuideText)
    {
        GuideText->SetVisibility(ESlateVisibility::Collapsed);
    }
}

void UInteractionWidget::ShowAcquisitionMessage(const FString& ItemName)
{
    if (AcquisitionText)
    {

        AcquisitionText->SetText(FText::FromString(FString::Printf(TEXT("%s을(를) 획득했습니다!"), *ItemName)));
        AcquisitionText->SetVisibility(ESlateVisibility::Visible);


        GetWorld()->GetTimerManager().ClearTimer(AcquisitionTimerHandle);

        GetWorld()->GetTimerManager().SetTimer(
            AcquisitionTimerHandle,
            this,
            &UInteractionWidget::HideAcquisitionMessage,
            2.5f,
            false
        );
    }
}

void UInteractionWidget::HandleTargetItemChanged(const FString& ItemName)
{
    if (ItemName.IsEmpty())
    {
        HideGuide();
    }
    else
    {
        ShowGuide(ItemName);
    }
}

void UInteractionWidget::HideAcquisitionMessage()
{
    if (AcquisitionText)
    {
        AcquisitionText->SetVisibility(ESlateVisibility::Collapsed);
    }
}
