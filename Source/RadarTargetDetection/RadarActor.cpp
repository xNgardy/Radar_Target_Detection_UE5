#include "RadarActor.h"
#include "TargetActor.h"

#include "Engine/World.h"
#include "Engine/Engine.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "Sound/SoundBase.h"

ARadarActor::ARadarActor()
{
    PrimaryActorTick.bCanEverTick = true;

    ScanRadius = 1500.0f;
    ScanInterval = 0.1f;
    bDrawDebugSphere = true;

    bUseDirectionalSweep = true;
    SweepAngleDegrees = 20.0f;

    RotationSpeed = 90.0f;

    bDrawSweepDebug = true;
    SweepDebugSegments = 16;
    SweepDebugHeightOffset = 40.0f;

    HostileAlarmSound = nullptr;
    bEnableHostileAlarm = true;
    LastAlarmTime = -999.0f;
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

    if (bDrawSweepDebug)
    {
        DrawSweepDebug();
    }
}

void ARadarActor::ScanForTargets()
{
    TArray<AActor*> FoundActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATargetActor::StaticClass(), FoundActors);

    TSet<ATargetActor*> CurrentDetectedTargets;

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
            0.08f,
            0,
            1.5f
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
            if (bUseDirectionalSweep && !IsTargetInsideSweepAngle(Target))
            {
                continue;
            }

            DetectedCount++;
            CurrentDetectedTargets.Add(Target);

            const FString TargetTypeText = ConvertTargetTypeToString(Target);
            const FString ThreatLevel = CalculateThreatLevel(Target, Distance);

            if (!PreviouslyDetectedTargets.Contains(Target))
            {
                const FString EnteredMessage = FString::Printf(
                    TEXT("TARGET ENTERED RADAR RANGE: %s"),
                    *Target->TargetId
                );

                GEngine->AddOnScreenDebugMessage(
                    -1,
                    1.0f,
                    FColor::Green,
                    EnteredMessage
                );
            }

            const FString TrackingMessage = FString::Printf(
                TEXT("TRACKING: %s | Type: %s | Distance: %.1f cm | Threat: %s"),
                *Target->TargetId,
                *TargetTypeText,
                Distance,
                *ThreatLevel
            );

            GEngine->AddOnScreenDebugMessage(
                -1,
                0.15f,
                FColor::Yellow,
                TrackingMessage
            );

            if (Target->TargetType == ETargetType::Hostile)
            {
                const FString WarningMessage = FString::Printf(
                    TEXT("WARNING: HOSTILE TARGET DETECTED -> %s | Threat: %s"),
                    *Target->TargetId,
                    *ThreatLevel
                );

                GEngine->AddOnScreenDebugMessage(
                    -1,
                    0.15f,
                    FColor::Red,
                    WarningMessage
                );

                const float CurrentTime = GetWorld()->GetTimeSeconds();
                const float AlarmCooldown = 1.0f;

                if (
                    bEnableHostileAlarm &&
                    HostileAlarmSound &&
                    CurrentTime - LastAlarmTime >= AlarmCooldown
                    )
                {
                    UGameplayStatics::PlaySoundAtLocation(
                        this,
                        HostileAlarmSound,
                        GetActorLocation()
                    );

                    LastAlarmTime = CurrentTime;
                }
            }

            DrawDebugLine(
                GetWorld(),
                GetActorLocation(),
                Target->GetActorLocation(),
                FColor::Red,
                false,
                0.08f,
                0,
                3.0f
            );
        }
    }

    for (ATargetActor* PreviousTarget : PreviouslyDetectedTargets)
    {
        if (!CurrentDetectedTargets.Contains(PreviousTarget))
        {
            if (IsValid(PreviousTarget))
            {
                const FString LostMessage = FString::Printf(
                    TEXT("TARGET LOST: %s"),
                    *PreviousTarget->TargetId
                );

                GEngine->AddOnScreenDebugMessage(
                    -1,
                    1.0f,
                    FColor(255, 165, 0),
                    LostMessage
                );
            }
        }
    }

    PreviouslyDetectedTargets = CurrentDetectedTargets;

    const FString SummaryMessage = FString::Printf(
        TEXT("Radar scan completed. Active tracks: %d"),
        DetectedCount
    );

    GEngine->AddOnScreenDebugMessage(
        -1,
        0.15f,
        FColor::Cyan,
        SummaryMessage
    );
}

void ARadarActor::DrawSweepDebug() const
{
    const UWorld* World = GetWorld();

    if (!World)
    {
        return;
    }

    const FVector RadarLocation = GetActorLocation() + FVector(0.0f, 0.0f, SweepDebugHeightOffset);

    FVector ForwardVector = GetActorForwardVector();
    ForwardVector.Z = 0.0f;

    if (ForwardVector.IsNearlyZero())
    {
        return;
    }

    ForwardVector = ForwardVector.GetSafeNormal();

    const FVector CenterEnd = RadarLocation + ForwardVector * ScanRadius;

    DrawDebugLine(
        World,
        RadarLocation,
        CenterEnd,
        FColor::Green,
        false,
        0.0f,
        0,
        3.0f
    );

    const FVector LeftDirection = ForwardVector.RotateAngleAxis(-SweepAngleDegrees, FVector::UpVector);
    const FVector RightDirection = ForwardVector.RotateAngleAxis(SweepAngleDegrees, FVector::UpVector);

    const FVector LeftEnd = RadarLocation + LeftDirection * ScanRadius;
    const FVector RightEnd = RadarLocation + RightDirection * ScanRadius;

    DrawDebugLine(
        World,
        RadarLocation,
        LeftEnd,
        FColor::Cyan,
        false,
        0.0f,
        0,
        2.0f
    );

    DrawDebugLine(
        World,
        RadarLocation,
        RightEnd,
        FColor::Cyan,
        false,
        0.0f,
        0,
        2.0f
    );

    const int32 SegmentCount = FMath::Max(1, SweepDebugSegments);

    FVector PreviousArcPoint = LeftEnd;

    for (int32 i = 1; i <= SegmentCount; i++)
    {
        const float Alpha = static_cast<float>(i) / static_cast<float>(SegmentCount);
        const float CurrentAngle = FMath::Lerp(-SweepAngleDegrees, SweepAngleDegrees, Alpha);

        const FVector CurrentDirection = ForwardVector.RotateAngleAxis(CurrentAngle, FVector::UpVector);
        const FVector CurrentArcPoint = RadarLocation + CurrentDirection * ScanRadius;

        DrawDebugLine(
            World,
            PreviousArcPoint,
            CurrentArcPoint,
            FColor::Yellow,
            false,
            0.0f,
            0,
            1.5f
        );

        DrawDebugLine(
            World,
            RadarLocation,
            CurrentArcPoint,
            FColor(180, 180, 180),
            false,
            0.0f,
            0,
            0.5f
        );

        PreviousArcPoint = CurrentArcPoint;
    }
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

FString ARadarActor::CalculateThreatLevel(const ATargetActor* Target, float Distance) const
{
    if (!Target)
    {
        return TEXT("UNKNOWN");
    }

    if (Target->TargetType == ETargetType::Friendly)
    {
        return TEXT("LOW");
    }

    if (Target->TargetType == ETargetType::Hostile)
    {
        if (Distance <= ScanRadius * 0.5f)
        {
            return TEXT("HIGH");
        }

        return TEXT("MEDIUM");
    }

    if (Target->TargetType == ETargetType::Unknown)
    {
        if (Distance <= ScanRadius * 0.4f)
        {
            return TEXT("MEDIUM");
        }

        return TEXT("LOW");
    }

    return TEXT("UNKNOWN");
}

bool ARadarActor::IsTargetInsideSweepAngle(const ATargetActor* Target) const
{
    if (!Target)
    {
        return false;
    }

    const FVector RadarLocation = GetActorLocation();
    const FVector TargetLocation = Target->GetActorLocation();

    FVector DirectionToTarget = TargetLocation - RadarLocation;
    DirectionToTarget.Z = 0.0f;

    if (DirectionToTarget.IsNearlyZero())
    {
        return true;
    }

    DirectionToTarget = DirectionToTarget.GetSafeNormal();

    FVector RadarForward = GetActorForwardVector();
    RadarForward.Z = 0.0f;

    if (RadarForward.IsNearlyZero())
    {
        return false;
    }

    RadarForward = RadarForward.GetSafeNormal();

    const float DotProduct = FVector::DotProduct(RadarForward, DirectionToTarget);
    const float ClampedDotProduct = FMath::Clamp(DotProduct, -1.0f, 1.0f);
    const float AngleRadians = FMath::Acos(ClampedDotProduct);
    const float AngleDegrees = FMath::RadiansToDegrees(AngleRadians);

    return AngleDegrees <= SweepAngleDegrees;
}