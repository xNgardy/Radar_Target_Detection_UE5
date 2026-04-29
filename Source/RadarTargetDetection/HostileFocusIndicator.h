#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HostileFocusIndicator.generated.h"

class ATargetActor;
class UStaticMeshComponent;
class USceneComponent;

UCLASS()
class RADARTARGETDETECTION_API AHostileFocusIndicator : public AActor
{
    GENERATED_BODY()

public:
    AHostileFocusIndicator();

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

private:
    UPROPERTY()
    ATargetActor* FocusedTarget;

    float PulseTime;

public:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    USceneComponent* RootScene;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UStaticMeshComponent* IndicatorMesh;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Focus")
    FVector TargetOffset;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Focus")
    float FollowInterpSpeed;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Focus")
    float RotationSpeed;

    UFUNCTION(BlueprintCallable, Category = "Focus")
    void SetFocusedTarget(ATargetActor* NewTarget);

    UFUNCTION(BlueprintCallable, Category = "Focus")
    void ClearFocusedTarget();
};