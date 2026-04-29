#include "TargetActor.h"

ATargetActor::ATargetActor()
{
    PrimaryActorTick.bCanEverTick = false;

    TargetId = TEXT("Target-001");
    TargetType = ETargetType::Unknown;
    bIsActiveTarget = true;
}

void ATargetActor::BeginPlay()
{
    Super::BeginPlay();
}