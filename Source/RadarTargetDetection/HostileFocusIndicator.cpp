#include "HostileFocusIndicator.h"
#include "TargetActor.h"

#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "DrawDebugHelpers.h"

AHostileFocusIndicator::AHostileFocusIndicator()
{
    PrimaryActorTick.bCanEverTick = true;

    RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
    RootComponent = RootScene;

    IndicatorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("IndicatorMesh"));
    IndicatorMesh->SetupAttachment(RootComponent);

    FocusedTarget = nullptr;

    TargetOffset = FVector(0.0f, 0.0f, 180.0f);
    FollowInterpSpeed = 10.0f;
    RotationSpeed = 180.0f;

    PulseTime = 0.0f;
}

void AHostileFocusIndicator::BeginPlay()
{
    Super::BeginPlay();

    SetActorHiddenInGame(true);
}

void AHostileFocusIndicator::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (!FocusedTarget || !FocusedTarget->bIsActiveTarget)
    {
        SetActorHiddenInGame(true);
        return;
    }

    SetActorHiddenInGame(false);

    PulseTime += DeltaTime;

    const FVector TargetLocation = FocusedTarget->GetActorLocation() + TargetOffset;

    const FVector NewLocation = FMath::VInterpTo(
        GetActorLocation(),
        TargetLocation,
        DeltaTime,
        FollowInterpSpeed
    );

    SetActorLocation(NewLocation);

    const FRotator RotationDelta = FRotator(0.0f, RotationSpeed * DeltaTime, 0.0f);
    AddActorLocalRotation(RotationDelta);

    const float PulseScale = 1.0f + FMath::Sin(PulseTime * 5.0f) * 0.15f;
    SetActorScale3D(FVector(PulseScale));
}

void AHostileFocusIndicator::SetFocusedTarget(ATargetActor* NewTarget)
{
    FocusedTarget = NewTarget;

    if (FocusedTarget)
    {
        const FVector StartLocation = FocusedTarget->GetActorLocation() + TargetOffset;
        SetActorLocation(StartLocation);
        SetActorHiddenInGame(false);
    }
    else
    {
        SetActorHiddenInGame(true);
    }
}

void AHostileFocusIndicator::ClearFocusedTarget()
{
    FocusedTarget = nullptr;
    SetActorHiddenInGame(true);
}