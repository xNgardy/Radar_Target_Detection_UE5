#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
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

    void ScanForTargets();

    FString ConvertTargetTypeToString(const ATargetActor* Target) const;

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Radar")
    float ScanRadius;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Radar")
    float ScanInterval;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Radar")
    bool bDrawDebugSphere;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Radar Visual")
    float RotationSpeed;
};