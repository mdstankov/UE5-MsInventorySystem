// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Manifest/Inv_ItemManifest.h"
#include "UObject/Object.h"
#include "Inv_InventoryItem.generated.h"

/**
 * 
 */
UCLASS()
class INVENTORY_API UInv_InventoryItem : public UObject
{
	GENERATED_BODY()
public:
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override; // Required for replication
	
	void SetItemManifest(const FInv_ItemManifest& InManifest);
	
private:
	UPROPERTY(VisibleAnywhere, meta = (BaseStruct = "/Script/Inventory.Inv_ItemManifest"), Replicated, Category="Inventory")
	FInstancedStruct ItemManifest;
	
	
};
