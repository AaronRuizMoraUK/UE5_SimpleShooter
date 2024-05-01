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
		BlackboardComp->SetValueAsVector(TEXT("StartLocation"), GetPawn()->GetActorLocation());
	}
}

void AShooterAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
