// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SynopsisDockWidget.generated.h"

/**
 * 
 */
UCLASS()
class TEAM6_CH3_PROJECT_API USynopsisDockWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	// 화면에 출력할 전체 텍스트
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Synopsis|Settings")
	FString FullStoryText;

	// 스토리 타이핑 연출을 시작
	UFUNCTION(BlueprintCallable, Category = "Synopsis|Actions")
	void StartStoryTyping();

protected:
	// 한 글자가 추가될 때마다 블루프린트 텍스트 블록을 갱신하기 위해 호출
	UFUNCTION(BlueprintImplementableEvent, Category = "Synopsis|Events")
	void OnTextUpdate(const FString& NewText);

	// 전체 스토리 타이핑이 완전히 끝났을 때 호출
	UFUNCTION(BlueprintImplementableEvent, Category = "Synopsis|Events")
	void OnIntroFinish();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Synopsis|Settings")
	float TypingSpeed = 0.05f;

private:

	// 타이머에 의해 주기적으로 실행되며 다음 글자를 추출
	void TypeNextChar();

	// 현재까지 화면에 조립되어 누적된 텍스트 
	FString CurrentDisplayedText;

	// 현재 몇 번째 글자까지 읽었는지 나타내는 인덱스 
	int32 CurrentCharIndex = 0;

	// 타이핑 주기 제어용 타이머 핸들
	FTimerHandle TypingTimerHandle;
};