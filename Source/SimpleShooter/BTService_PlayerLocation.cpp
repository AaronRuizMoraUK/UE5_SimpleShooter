// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_PlayerLocation.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"

UBTService_PlayerLocation::UBTService_PlayerLocation()
{
	NodeName = TEXT("Update Player Location");
}

void UBTService_PlayerLocation::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	auto* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!BlackboardComp)
	{
		return;
	}

	if (auto* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0))
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
}
