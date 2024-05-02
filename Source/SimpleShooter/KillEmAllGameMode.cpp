// Fill out your copyright notice in the Description page of Project Settings.


#include "KillEmAllGameMode.h"
#include "ShooterAIController.h"
#include "EngineUtils.h"
#include "GameFramework/Controller.h"
#include "Algo/AllOf.h"

void AKillEmAllGameMode::PawnKilled(APawn* PawnKilled)
{
    Super::PawnKilled(PawnKilled);

    UE_LOG(LogTemp, Display, TEXT("AKillEmAllGameMode: Pawn %s was killed."), *PawnKilled->GetName());

    // If the player was killed, end the game and the player lost.
    if (auto* Controller = PawnKilled->GetController(); 
        Controller && Controller->IsPlayerController())
    {
        EndGame(false);
    }
    else
    {
        // If all enemies are dead, end the game and the player wins.
        if (Algo::AllOf(TActorRange<AShooterAIController>(GetWorld()), &AShooterAIController::IsDead))
        {
            EndGame(true);
        }
    }
}

void AKillEmAllGameMode::EndGame(bool bIsPlayerWinner)
{
    // Notify all controllers that the game has ended and if they are in the winning team.
    for (auto* Controller : TActorRange<AController>(GetWorld()))
    {
        const bool bIsWinner = Controller->IsPlayerController() == bIsPlayerWinner;
        Controller->GameHasEnded(Controller->GetPawn()/*Actor to focus on*/, bIsWinner);
    }
}
