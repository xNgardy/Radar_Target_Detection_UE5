#include "RadarActor.h"
#include "TargetActor.h"

#include "Engine/World.h"
#include "Engine/Engine.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

ARadarActor::ARadarActor()
{
    PrimaryActorTick.bCanEverTick = true;

    RotationSpeed = 90.0f;
    ScanRadius = 1500.0f;
    ScanInterval = 1.0f;
    bDrawDebugSphere = true;
}

void ARadarActor::BeginPlay()
{
    Super::BeginPlay();

    GetWorldTimerManager().SetTimer(
        ScanTimerHandle,
        this,
        &ARadarActor::ScanForTargets,
        ScanInterval,
        true
    );
}

void ARadarActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    const FRotator RotationDelta = FRotator(0.0f, RotationSpeed * DeltaTime, 0.0f);
    AddActorLocalRotation(RotationDelta);
}

void ARadarActor::ScanForTargets()
{
    TArray<AActor*> FoundActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATargetActor::StaticClass(), FoundActors);

    int32 DetectedCount = 0;

    if (bDrawDebugSphere)
    {
        DrawDebugSphere(
            GetWorld(),
            GetActorLocation(),
            ScanRadius,
            32,
            FColor::Green,
            false,
            ScanInterval,
            0,
            2.0f
        );
    }

    for (AActor* Actor : FoundActors)
    {
        ATargetActor* Target = Cast<ATargetActor>(Actor);

        if (!Target || !Target->bIsActiveTarget)
        {
            continue;
        }

        const float Distance = FVector::Dist(GetActorLocation(), Target->GetActorLocation());

        if (Distance <= ScanRadius)
        {
            DetectedCount++;

            const FString TargetTypeText = ConvertTargetTypeToString(Target);

            const FString Message = FString::Printf(
                TEXT("Detected: %s | Type: %s | Distance: %.1f cm"),
                *Target->TargetId,
                *TargetTypeText,
                Distance
            );

            GEngine->AddOnScreenDebugMessage(
                -1,
                ScanInterval,
                FColor::Yellow,
                Message
            );

            DrawDebugLine(
                GetWorld(),
                GetActorLocation(),
                Target->GetActorLocation(),
                FColor::Red,
                false,
                ScanInterval,
                0,
                2.0f
            );
        }
    }

    const FString SummaryMessage = FString::Printf(
        TEXT("Radar scan completed. Detected targets: %d"),
        DetectedCount
    );

    GEngine->AddOnScreenDebugMessage(
        -1,
        ScanInterval,
        FColor::Cyan,
        SummaryMessage
    );
}

FString ARadarActor::ConvertTargetTypeToString(const ATargetActor* Target) const
{
    if (!Target)
    {
        return TEXT("Invalid");
    }

    switch (Target->TargetType)
    {
    case ETargetType::Friendly:
        return TEXT("Friendly");

    case ETargetType::Hostile:
        return TEXT("Hostile");

    case ETargetType::Unknown:
    default:
        return TEXT("Unknown");
    }
}