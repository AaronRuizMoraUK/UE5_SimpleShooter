// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterAIController.h"
#include "Kismet/GameplayStatics.h"

// Called when the game starts or when spawned
void AShooterAIController::BeginPlay()
{
	Super::BeginPlay();
}

void AShooterAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	const EAIFocusPriority::Type FocusPriority = EAIFocusPriority::Gameplay;

	if (APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0); 
		PlayerPawn && LineOfSightTo(PlayerPawn))
	{
		SetFocus(PlayerPawn, FocusPriority);
		MoveToActor(PlayerPawn, AcceptanceRadius);
	}
	else
	{
		ClearFocus(FocusPriority);
		StopMovement();
	}
}
