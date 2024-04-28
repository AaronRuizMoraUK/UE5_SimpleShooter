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
		}
	}
}

void AShooterAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	auto* BlackboardComp = GetBlackboardComponent();
	if (!BlackboardComp)
	{
		return;
	}

	const EAIFocusPriority::Type FocusPriority = EAIFocusPriority::Gameplay;

	if (APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0); 
		PlayerPawn && LineOfSightTo(PlayerPawn))
	{
		BlackboardComp->SetValueAsVector(TEXT("PlayerLocation"), PlayerPawn->GetActorLocation());
		BlackboardComp->SetValueAsVector(TEXT("LastKnownPlayerLocation"), PlayerPawn->GetActorLocation());
	}
	else
	{
		BlackboardComp->ClearValue(TEXT("PlayerLocation"));
	}
}
