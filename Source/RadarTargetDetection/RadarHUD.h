#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "RadarHUD.generated.h"

class ARadarActor;
class ATargetActor;

UCLASS()
class RADARTARGETDETECTION_API ARadarHUD : public AHUD
{
    GENERATED_BODY()

public:
    virtual void DrawHUD() override;

protected:
    virtual void BeginPlay() override;

private:
    ARadarActor* RadarActor;

    void FindRadarActor();
    void DrawRadarPanel();
    void DrawRadarCircle(const FVector2D& Center, float Radius, const FLinearColor& Color, float Thickness);
    void DrawContactList(const FVector2D& StartPosition);
    void DrawRadarSweepLine(const FVector2D& Center, float Radius);
    void DrawTargetDots(const FVector2D& Center, float Radius);
    void DrawTextLabel(const FString& Text, const FVector2D& Position, const FLinearColor& Color);

    FVector2D ConvertWorldLocationToRadarPosition(
        const FVector& TargetLocation,
        const FVector2D& RadarCenter,
        float RadarPanelRadius
    ) const;

    FLinearColor GetTargetColor(const ATargetActor* Target) const;
    bool IsTargetVisibleOnRadar(const ATargetActor* Target) const;
};