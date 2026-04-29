#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TargetActor.generated.h"

UENUM(BlueprintType)
enum class ETargetType : uint8
{
    Friendly UMETA(DisplayName = "Friendly"),
    Hostile UMETA(DisplayName = "Hostile"),
    Unknown UMETA(DisplayName = "Unknown")
};

UCLASS()
class RADARTARGETDETECTION_API ATargetActor : public AActor
{
    GENERATED_BODY()

public:
    ATargetActor();

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;


public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Target")
    FString TargetId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Target")
    ETargetType TargetType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Target")
    bool bIsActiveTarget;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    bool bCanMove;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float MovementSpeed;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float MovementRange;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    FVector MovementDirection;

private:
    FVector StartLocation;
    float MovementTime;
};
