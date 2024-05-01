// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_PlayerLocationIfSeen.h"
#include "AIController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTService_PlayerLocationIfSeen::UBTService_PlayerLocationIfSeen()
{
	NodeName = TEXT("Update Player Location If Seen");
}

void UBTService_PlayerLocationIfSeen::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	auto* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!BlackboardComp)
	{
		return;
	}

	auto* AIController = OwnerComp.GetAIOwner();
	if (!AIController)
	{
		return;
	}

	if (auto* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
		AIController->LineOfSightTo(PlayerPawn))
	{
		BlackboardComp->SetValueAsVector(GetSelectedBlackboardKey(), PlayerPawn->GetActorLocation());
	}
	else
	{
		BlackboardComp->ClearValue(GetSelectedBlackboardKey());
	}
}
