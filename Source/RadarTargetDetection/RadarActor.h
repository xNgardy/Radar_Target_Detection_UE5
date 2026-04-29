#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Sound/SoundBase.h"
#include "RadarActor.generated.h"

class ATargetActor;

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

    float LastAlarmTime;

    void ScanForTargets();
    void DrawSweepDebug() const;

    FString ConvertTargetTypeToString(const ATargetActor* Target) const;
    FString CalculateThreatLevel(const ATargetActor* Target, float Distance) const;
    bool IsTargetInsideSweepAngle(const ATargetActor* Target) const;

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
};