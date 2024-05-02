// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_PlayerLocationIfSeen.h"
#include "AIController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"

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
		if (BlackboardKey.SelectedKeyType == UBlackboardKeyType_Vector::StaticClass())
		{
			BlackboardComp->SetValueAsVector(GetSelectedBlackboardKey(), PlayerPawn->GetActorLocation());
		}
		else if (BlackboardKey.SelectedKeyType == UBlackboardKeyType_Object::StaticClass())
		{
			BlackboardComp->SetValueAsObject(GetSelectedBlackboardKey(), PlayerPawn);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Blackboard key %s is not an object or vector type"), *GetSelectedBlackboardKey().ToString())
		}
	}
	else
	{
		BlackboardComp->ClearValue(GetSelectedBlackboardKey());
	}
}
