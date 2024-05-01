// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Shoot.h"
#include "AIController.h"
#include "ShooterCharacter.h"

UBTTask_Shoot::UBTTask_Shoot()
{
	NodeName = TEXT("Shoot");
}

EBTNodeResult::Type UBTTask_Shoot::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	if (auto* AIController = OwnerComp.GetAIOwner())
	{
		if (auto* ShooterCharacter = Cast<AShooterCharacter>(AIController->GetPawn()))
		{
			ShooterCharacter->Shoot();
			return EBTNodeResult::Succeeded;
		}
	}

	return EBTNodeResult::Failed;
}
