// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterAIController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"

// Called when the game starts or when spawned
void AShooterAIController::BeginPlay()
{
	Super::BeginPlay();

	RunBehaviorTree(AIBehaviour);

	if (auto* BlackboardComp = GetBlackboardComponent())
	{
		if (APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0))
		{
			BlackboardComp->SetValueAsVector(TEXT("StartLocation"), GetPawn()->GetActorLocation());
			BlackboardComp->SetValueAsVector(TEXT("PlayerLocation"), PlayerPawn->GetActorLocation());
		}
	}
}

void AShooterAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//const EAIFocusPriority::Type FocusPriority = EAIFocusPriority::Gameplay;

	//if (APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0); 
	//	PlayerPawn && LineOfSightTo(PlayerPawn))
	//{
	//	SetFocus(PlayerPawn, FocusPriority);
	//	MoveToActor(PlayerPawn, AcceptanceRadius);
	//}
	//else
	//{
	//	ClearFocus(FocusPriority);
	//	StopMovement();
	//}
}
