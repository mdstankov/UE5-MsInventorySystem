// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InvItemComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent),Blueprintable)
class INVENTORY_API UInvItemComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UInvItemComponent();

	FString GetPickupMessage() const { return PickupMessage; }
	
protected:

private:
	UPROPERTY(EditAnywhere,Category= "Inventory")
	FString PickupMessage;
	
	
};
