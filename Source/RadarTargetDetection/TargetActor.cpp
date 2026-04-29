#include "TargetActor.h"

ATargetActor::ATargetActor()
{
    PrimaryActorTick.bCanEverTick = true;

    TargetId = TEXT("Target-001");
    TargetType = ETargetType::Unknown;
    bIsActiveTarget = true;

    bCanMove = false;
    MovementSpeed = 1.0f;
    MovementRange = 300.0f;
    MovementDirection = FVector(1.0f, 0.0f, 0.0f);

    MovementTime = 0.0f;
}

void ATargetActor::BeginPlay()
{
    Super::BeginPlay();

    StartLocation = GetActorLocation();

    if (!MovementDirection.IsNearlyZero())
    {
        MovementDirection = MovementDirection.GetSafeNormal();
    }
}

void ATargetActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (!bCanMove)
    {
        return;
    }

    MovementTime += DeltaTime * MovementSpeed;

    const float MovementOffset = FMath::Sin(MovementTime) * MovementRange;
    const FVector NewLocation = StartLocation + MovementDirection * MovementOffset;

    SetActorLocation(NewLocation);
}