// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryManagement/Components/Inv_InventoryComponent.h"

#include "Net/UnrealNetwork.h"
#include "Widgets/Inventory/Inv_InventoryBase.h"

UInv_InventoryComponent::UInv_InventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);
	bReplicateUsingRegisteredSubObjectList = true;
	bInventoryMenuOpen = false;
}

void UInv_InventoryComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ThisClass, InventoryList);
}

void UInv_InventoryComponent::TryAddItem(UInv_ItemComponent* ItemComponent)
{
	FInv_SlotAvailabilityResult Result = InventoryMenu->HasRoomForItem(ItemComponent);
	
	if (Result.TotalRoomToFill == 0)
	{
		NoRoomInInventory.Broadcast();
		return;
	}
	
	if (Result.Item.IsValid() && Result.bStackable)
	{
		// Add stacks to an item that already exists. Update stack count
		Server_AddStacksToItem(ItemComponent,Result.TotalRoomToFill, Result.Remainder);
	}
	else if (Result.TotalRoomToFill > 0 )
	{
		// Create new item and update all perminent slots.
		Server_AddNewItem(ItemComponent, Result.bStackable ? Result.TotalRoomToFill : 0);
	}
}

void UInv_InventoryComponent::Server_AddNewItem_Implementation(UInv_ItemComponent* ItemComponent, int32 StackCount)
{
	UInv_InventoryItem* newItem = InventoryList.AddEntry(ItemComponent);
	
	// Tell item component to destroy its owning actor from the world
	
}

void UInv_InventoryComponent::Server_AddStacksToItem_Implementation(UInv_ItemComponent* ItemComponent, int32 StackCount, int32 Remainder)
{
	
}

void UInv_InventoryComponent::ToggleInventoryMenu()
{
	if (bInventoryMenuOpen)
	{
		CloseInventoryMenu();		
	}
	else
	{
		OpenInventoryMenu();
	}
}

void UInv_InventoryComponent::AddRepSubObj(UObject* SubObj)
{
	// Replicating uobjects on some actor or actor component that support replication(setup in constructor)
	if (IsUsingRegisteredSubObjectList() && IsReadyForReplication() && IsValid(SubObj))
	{
		AddReplicatedSubObject(SubObj);
	}
}	

void UInv_InventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	
	ConstructInventory();	
}

void UInv_InventoryComponent::ConstructInventory()
{
	OwningController = Cast<APlayerController>(GetOwner());
	checkf(OwningController.IsValid(), TEXT("Inventory Component should have Player Controller as owner")); //forces crash
	
	if (!OwningController->IsLocalController()) return;;
	
	InventoryMenu = CreateWidget<UInv_InventoryBase>(OwningController.Get(), InventoryMenuClass);
	InventoryMenu->AddToViewport();
	CloseInventoryMenu();	
}

void UInv_InventoryComponent::OpenInventoryMenu()
{
	if (!IsValid(InventoryMenu)) return;
	
	InventoryMenu->SetVisibility(ESlateVisibility::Visible);
	bInventoryMenuOpen = true;
	
	if (!OwningController.IsValid()) return;
	
	FInputModeGameAndUI inputMode;
	OwningController->SetInputMode(inputMode);
	OwningController->SetShowMouseCursor(true);	
}

void UInv_InventoryComponent::CloseInventoryMenu()
{
	if (!IsValid(InventoryMenu)) return;
	
	if (!OwningController.IsValid()) return;
	
	InventoryMenu->SetVisibility(ESlateVisibility::Collapsed);
	bInventoryMenuOpen = false;
	
	FInputModeGameOnly inputMode;
	OwningController->SetInputMode(inputMode);
	OwningController->SetShowMouseCursor(false);	
}
