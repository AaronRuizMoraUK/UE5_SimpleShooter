// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterPlayerController.h"
#include "Blueprint/UserWidget.h"

void AShooterPlayerController::BeginPlay()
{
    Super::BeginPlay();

    HUD = CreateWidget(this, HUDClass);
    if (HUD)
    {
        HUD->AddToViewport();
    }
}


void AShooterPlayerController::GameHasEnded(AActor* EndGameFocus, bool bIsWinner)
{
    Super::GameHasEnded(EndGameFocus, bIsWinner);

    UE_LOG(LogTemp, Display, TEXT("GameHasEnded - Won? %s"), bIsWinner ? TEXT("YES") : TEXT("NO"));

    if (HUD)
    {
        HUD->RemoveFromParent();
    }

    UUserWidget* EndGameScreen = (bIsWinner)
        ? CreateWidget(this, WinScreenClass)
        : CreateWidget(this, LoseScreenClass);

    if (EndGameScreen)
    {
        EndGameScreen->AddToViewport();
    }

    FTimerHandle RestartLevelTimerHandle;
    GetWorldTimerManager().SetTimer(RestartLevelTimerHandle, this, &APlayerController::RestartLevel, RestartDelay);
}
