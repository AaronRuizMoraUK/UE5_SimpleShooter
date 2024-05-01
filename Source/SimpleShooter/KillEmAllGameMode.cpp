// Fill out your copyright notice in the Description page of Project Settings.


#include "KillEmAllGameMode.h"

void AKillEmAllGameMode::PawnKilled(APawn* PawnKilled)
{
    Super::PawnKilled(PawnKilled);

    UE_LOG(LogTemp, Display, TEXT("AKillEmAllGameMode: Pawn %s was killed."), *PawnKilled->GetName());

    if (auto* PlayerController = Cast<APlayerController>(PawnKilled->GetController()))
    {
        PlayerController->GameHasEnded(nullptr, false /*Lose*/);
    }
}
