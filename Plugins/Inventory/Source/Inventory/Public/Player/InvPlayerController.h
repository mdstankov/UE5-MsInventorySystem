// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InvPlayerController.generated.h"

/**
 * 
 */

class UInv_InventoryComponent;
class UInputMappingContext;
class UInputAction;
class UInv_HUDWidget;

UCLASS()
class INVENTORY_API AInvPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AInvPlayerController();
	
	virtual void Tick(float DeltaSeconds) override;
	
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void ToggleInventory(); // Exposed to be called from outside sources, like UI close buttons
	
protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

private:
	void PrimaryInteract();
	void CreateHUDWidget();
	void TraceForItem();

	TWeakObjectPtr<UInv_InventoryComponent> InventoryComponent; //Added on the blueprint
		
	
	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	TArray<TObjectPtr<UInputMappingContext>> DefaultIMCs;

	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	TObjectPtr<UInputAction> PrimaryInteractAction;

	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	TObjectPtr<UInputAction> ToggleInventoryAction;
	
	UPROPERTY(EditDefaultsOnly, Category= "Inventory")
	TSubclassOf<UInv_HUDWidget> HUDWidgetClass;
	
	UPROPERTY()
	TObjectPtr<UInv_HUDWidget> HUDWidget;

	UPROPERTY(EditDefaultsOnly, Category= "Inventory")
	double TraceLength;
	
	UPROPERTY(EditDefaultsOnly, Category= "Inventory")
	TEnumAsByte<ECollisionChannel> ItemTraceChannel; // TEnumAsByte required to expose details panel
	
	TWeakObjectPtr<AActor> ThisActor;
	TWeakObjectPtr<AActor> LastActor;
	
	
};
