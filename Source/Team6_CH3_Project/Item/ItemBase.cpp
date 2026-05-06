// ItemaBase.h
#include "ItemBase.h"

AItemBase::AItemBase()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AItemBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AItemBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AItemBase::Interact(ACharacter* Character)
{

}
