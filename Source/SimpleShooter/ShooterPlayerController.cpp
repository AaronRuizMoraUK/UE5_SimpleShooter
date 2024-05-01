// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterPlayerController.h"

void AShooterPlayerController::GameHasEnded(AActor* EndGameFocus, bool bIsWinner)
{
    Super::GameHasEnded(EndGameFocus, bIsWinner);

    UE_LOG(LogTemp, Display, TEXT("GameHasEnded - Won? %s"), bIsWinner ? TEXT("YES") : TEXT("NO"));

    FTimerHandle RestartLevelTimerHandle;
    GetWorldTimerManager().SetTimer(RestartLevelTimerHandle, this, &APlayerController::RestartLevel, RestartDelay);
}
