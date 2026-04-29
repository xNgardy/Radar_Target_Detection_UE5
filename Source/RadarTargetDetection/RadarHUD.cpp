#include "RadarHUD.h"
#include "RadarActor.h"
#include "TargetActor.h"

#include "Engine/Canvas.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"

void ARadarHUD::BeginPlay()
{
    Super::BeginPlay();

    FindRadarActor();
}

void ARadarHUD::DrawHUD()
{
    Super::DrawHUD();

    if (!RadarActor)
    {
        FindRadarActor();
    }

    if (RadarActor)
    {
        DrawRadarPanel();
    }
}

void ARadarHUD::FindRadarActor()
{
    TArray<AActor*> FoundActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ARadarActor::StaticClass(), FoundActors);

    if (FoundActors.Num() > 0)
    {
        RadarActor = Cast<ARadarActor>(FoundActors[0]);
    }
}

void ARadarHUD::DrawRadarPanel()
{
    if (!Canvas || !RadarActor)
    {
        return;
    }

    const float PanelRadius = 110.0f;
    const FVector2D PanelCenter(Canvas->SizeX - 150.0f, 150.0f);

    DrawTextLabel(TEXT("RADAR"), FVector2D(PanelCenter.X - 35.0f, PanelCenter.Y - PanelRadius - 30.0f), FLinearColor::Green);

    DrawRadarCircle(PanelCenter, PanelRadius, FLinearColor::Green, 1.5f);
    DrawRadarCircle(PanelCenter, PanelRadius * 0.66f, FLinearColor(0.0f, 0.6f, 0.0f, 1.0f), 1.0f);
    DrawRadarCircle(PanelCenter, PanelRadius * 0.33f, FLinearColor(0.0f, 0.4f, 0.0f, 1.0f), 1.0f);

    DrawLine(
        PanelCenter.X - PanelRadius,
        PanelCenter.Y,
        PanelCenter.X + PanelRadius,
        PanelCenter.Y,
        FLinearColor(0.0f, 0.4f, 0.0f, 1.0f),
        1.0f
    );

    DrawLine(
        PanelCenter.X,
        PanelCenter.Y - PanelRadius,
        PanelCenter.X,
        PanelCenter.Y + PanelRadius,
        FLinearColor(0.0f, 0.4f, 0.0f, 1.0f),
        1.0f
    );

    // Radar center point
    DrawRect(
        FLinearColor::Green,
        PanelCenter.X - 3.0f,
        PanelCenter.Y - 3.0f,
        6.0f,
        6.0f
    );

    DrawRadarSweepLine(PanelCenter, PanelRadius);
    DrawTargetDots(PanelCenter, PanelRadius);
    DrawContactList(FVector2D(PanelCenter.X - PanelRadius, PanelCenter.Y + PanelRadius + 20.0f));
}

void ARadarHUD::DrawRadarCircle(const FVector2D& Center, float Radius, const FLinearColor& Color, float Thickness)
{
    const int32 Segments = 64;

    for (int32 i = 0; i < Segments; i++)
    {
        const float AngleA = (static_cast<float>(i) / static_cast<float>(Segments)) * 2.0f * PI;
        const float AngleB = (static_cast<float>(i + 1) / static_cast<float>(Segments)) * 2.0f * PI;

        const FVector2D PointA(
            Center.X + FMath::Cos(AngleA) * Radius,
            Center.Y + FMath::Sin(AngleA) * Radius
        );

        const FVector2D PointB(
            Center.X + FMath::Cos(AngleB) * Radius,
            Center.Y + FMath::Sin(AngleB) * Radius
        );

        DrawLine(PointA.X, PointA.Y, PointB.X, PointB.Y, Color, Thickness);
    }
}

void ARadarHUD::DrawRadarSweepLine(const FVector2D& Center, float Radius)
{
    if (!RadarActor)
    {
        return;
    }

    FVector Forward = RadarActor->GetActorForwardVector();
    Forward.Z = 0.0f;

    if (Forward.IsNearlyZero())
    {
        return;
    }

    Forward = Forward.GetSafeNormal();

    // World X/Y to screen radar X/Y
    const FVector2D SweepDirection(Forward.X, Forward.Y);

    const FVector2D EndPoint(
        Center.X + SweepDirection.X * Radius,
        Center.Y + SweepDirection.Y * Radius
    );

    DrawLine(
        Center.X,
        Center.Y,
        EndPoint.X,
        EndPoint.Y,
        FLinearColor::Green,
        2.5f
    );
}

void ARadarHUD::DrawTargetDots(const FVector2D& Center, float Radius)
{
    if (!RadarActor)
    {
        return;
    }

    TArray<AActor*> FoundTargets;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATargetActor::StaticClass(), FoundTargets);

    for (AActor* Actor : FoundTargets)
    {
        ATargetActor* Target = Cast<ATargetActor>(Actor);

        if (!Target || !Target->bIsActiveTarget)
        {
            continue;
        }

        if (!IsTargetVisibleOnRadar(Target))
        {
            continue;
        }

        const FVector2D DotPosition = ConvertWorldLocationToRadarPosition(
            Target->GetActorLocation(),
            Center,
            Radius
        );

        const FLinearColor DotColor = GetTargetColor(Target);

        DrawRect(
            DotColor,
            DotPosition.X - 4.0f,
            DotPosition.Y - 4.0f,
            8.0f,
            8.0f
        );
    }
}

FVector2D ARadarHUD::ConvertWorldLocationToRadarPosition(
    const FVector& TargetLocation,
    const FVector2D& RadarCenter,
    float RadarPanelRadius
) const
{
    if (!RadarActor)
    {
        return RadarCenter;
    }

    FVector RelativeLocation = TargetLocation - RadarActor->GetActorLocation();

    const float NormalizedX = FMath::Clamp(RelativeLocation.X / RadarActor->ScanRadius, -1.0f, 1.0f);
    const float NormalizedY = FMath::Clamp(RelativeLocation.Y / RadarActor->ScanRadius, -1.0f, 1.0f);

    return FVector2D(
        RadarCenter.X + NormalizedX * RadarPanelRadius,
        RadarCenter.Y + NormalizedY * RadarPanelRadius
    );
}

FLinearColor ARadarHUD::GetTargetColor(const ATargetActor* Target) const
{
    if (!Target)
    {
        return FLinearColor::White;
    }

    switch (Target->TargetType)
    {
    case ETargetType::Friendly:
        return FLinearColor::Green;

    case ETargetType::Hostile:
        return FLinearColor::Red;

    case ETargetType::Unknown:
    default:
        return FLinearColor::Yellow;
    }
}

bool ARadarHUD::IsTargetVisibleOnRadar(const ATargetActor* Target) const
{
    if (!RadarActor || !Target)
    {
        return false;
    }

    const float Distance = FVector::Dist(RadarActor->GetActorLocation(), Target->GetActorLocation());

    if (Distance > RadarActor->ScanRadius)
    {
        return false;
    }

    if (!RadarActor->bUseDirectionalSweep)
    {
        return true;
    }

    FVector DirectionToTarget = Target->GetActorLocation() - RadarActor->GetActorLocation();
    DirectionToTarget.Z = 0.0f;

    if (DirectionToTarget.IsNearlyZero())
    {
        return true;
    }

    DirectionToTarget = DirectionToTarget.GetSafeNormal();

    FVector RadarForward = RadarActor->GetActorForwardVector();
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

    return AngleDegrees <= RadarActor->SweepAngleDegrees;
}

void ARadarHUD::DrawTextLabel(const FString& Text, const FVector2D& Position, const FLinearColor& Color)
{
    if (!Canvas)
    {
        return;
    }

    DrawText(
        Text,
        Color,
        Position.X,
        Position.Y,
        nullptr,
        1.0f,
        false
    );
}

void ARadarHUD::DrawContactList(const FVector2D& StartPosition)
{
    if (!Canvas || !RadarActor)
    {
        return;
    }

    DrawTextLabel(TEXT("CONTACTS"), StartPosition, FLinearColor::Green);

    float CurrentY = StartPosition.Y + 18.0f;

    if (RadarActor->CurrentContacts.Num() == 0)
    {
        DrawTextLabel(TEXT("No active contacts"), FVector2D(StartPosition.X, CurrentY), FLinearColor(0.5f, 0.5f, 0.5f, 1.0f));
        return;
    }

    const int32 MaxContactsToShow = 5;
    const int32 ContactCount = FMath::Min(RadarActor->CurrentContacts.Num(), MaxContactsToShow);

    for (int32 i = 0; i < ContactCount; i++)
    {
        const FRadarContactInfo& Contact = RadarActor->CurrentContacts[i];

        const FString ContactLine = FString::Printf(
            TEXT("%s  %s  %s  %.0fcm/s  %s"),
            *Contact.TrackId,
            *Contact.TargetId,
            *Contact.TargetType,
            Contact.Speed,
            *Contact.ThreatLevel
        );

        FLinearColor TextColor = FLinearColor::White;

        if (Contact.TargetType == TEXT("Hostile"))
        {
            TextColor = FLinearColor::Red;
        }
        else if (Contact.TargetType == TEXT("Friendly"))
        {
            TextColor = FLinearColor::Green;
        }
        else if (Contact.TargetType == TEXT("Unknown"))
        {
            TextColor = FLinearColor::Yellow;
        }

        DrawTextLabel(ContactLine, FVector2D(StartPosition.X, CurrentY), TextColor);

        CurrentY += 16.0f;
    }
}