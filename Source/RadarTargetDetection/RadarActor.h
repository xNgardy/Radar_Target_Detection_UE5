#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Sound/SoundBase.h"
#include "RadarActor.generated.h"

class ATargetActor;
class AHostileFocusIndicator;

USTRUCT(BlueprintType)
struct FRadarContactInfo
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FString TrackId;

    UPROPERTY(BlueprintReadOnly)
    FString TargetId;

    UPROPERTY(BlueprintReadOnly)
    FString TargetType;

    UPROPERTY(BlueprintReadOnly)
    float Distance;

    UPROPERTY(BlueprintReadOnly)
    float Speed;

    UPROPERTY(BlueprintReadOnly)
    FString ThreatLevel;
};

UCLASS()
class RADARTARGETDETECTION_API ARadarActor : public AActor
{
    GENERATED_BODY()

public:
    ARadarActor();

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

private:
    FTimerHandle ScanTimerHandle;

    TSet<ATargetActor*> PreviouslyDetectedTargets;
    TMap<ATargetActor*, FVector> PreviousTargetLocations;
    TMap<ATargetActor*, FString> TargetTrackIds;

    ATargetActor* CurrentFocusedHostileTarget;

    float LastAlarmTime;
    int32 NextTrackNumber;

    void ScanForTargets();
    void DrawSweepDebug() const;

    FString ConvertTargetTypeToString(const ATargetActor* Target) const;
    FString CalculateThreatLevel(const ATargetActor* Target, float Distance) const;
    FString GetOrCreateTrackId(ATargetActor* Target);

    bool IsTargetInsideSweepAngle(const ATargetActor* Target) const;
    float CalculateTargetSpeed(ATargetActor* Target, float DeltaTime);
    void UpdateHostileFocusIndicator(ATargetActor* FocusTarget);

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Radar")
    float ScanRadius;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Radar")
    float ScanInterval;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Radar")
    bool bDrawDebugSphere;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Radar")
    bool bUseDirectionalSweep;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Radar")
    float SweepAngleDegrees;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Radar Visual")
    float RotationSpeed;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Radar Debug")
    bool bDrawSweepDebug;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Radar Debug")
    int32 SweepDebugSegments;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Radar Debug")
    float SweepDebugHeightOffset;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Radar Audio")
    USoundBase* HostileAlarmSound;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Radar Audio")
    bool bEnableHostileAlarm;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Radar Focus")
    AHostileFocusIndicator* FocusIndicator;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Radar Focus")
    bool bEnableHostileFocus;

    UPROPERTY(BlueprintReadOnly, Category = "Radar Contacts")
    TArray<FRadarContactInfo> CurrentContacts;
};