// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Types/Inv_GridTypes.h"

#include "Inv_InventoryGrid.generated.h"

struct FInv_ImageFragment;
struct FInv_GridFragment;
class UInv_SlottedItem;
class UInv_GridSlot;
class UCanvasPanel;
class UInv_InventoryComponent;
struct  FInv_ItemManifest;
class UInv_InventoryItem;
class UInv_ItemComponent;

UCLASS()
class INVENTORY_API UInv_InventoryGrid : public UUserWidget
{
	GENERATED_BODY()
public:	
	virtual void NativeOnInitialized() override;
	
	EInv_ItemCategory GetItemCategory() const { return ItemCategory;}
	FInv_SlotAvailabilityResult HasRoomForItem(const UInv_ItemComponent* ItemComponent);
	
	UFUNCTION()
	void AddItem( UInv_InventoryItem* Item); // Response to OnItemAdded from InventoryComponent
	
private:
	TWeakObjectPtr<UInv_InventoryComponent> InventoryComponent;
	
	void ConstructGrid();
	
	FInv_SlotAvailabilityResult HasRoomForItem(const UInv_InventoryItem* Item);
	FInv_SlotAvailabilityResult HasRoomForItem(const FInv_ItemManifest& Manifest);
	void AddItemToIndices(const FInv_SlotAvailabilityResult& Result, UInv_InventoryItem* NewItem);
	bool MatchesCategory(const UInv_InventoryItem* Item) const;
	FVector2D GetDrawSize( const FInv_GridFragment* GridFragment ) const;
	void SetSlottedItemImage(const UInv_SlottedItem* SlottedItem, const FInv_GridFragment* GridFragment, const FInv_ImageFragment* ImageFragment) const;
	void AddItemToIndex( UInv_InventoryItem* Item, const int32 Index, const bool bStackble, const int32 StackAmount) const;
	UInv_SlottedItem* CreateSlottedItem( UInv_InventoryItem* Item,
		const bool bStackble,
		const int32 StackAmount,
		const FInv_GridFragment* GridFragment,
		const FInv_ImageFragment* ImageFragment,
		const int32 Index) const;
		
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Inventory")
	EInv_ItemCategory ItemCategory;
	
	UPROPERTY(EditAnywhere,Category="Inventory")
	TSubclassOf<UInv_GridSlot> GridSlotClass;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCanvasPanel> CanvasPanel;
	
	TSubclassOf<UInv_SlottedItem> SlottedItemClass;
	
	UPROPERTY()
	TArray<TObjectPtr<UInv_GridSlot>> GridSlots;
	
	UPROPERTY(EditAnywhere,Category="Inventory")
	int32 Rows;
	
	UPROPERTY(EditAnywhere,Category="Inventory")
	int32 Columns;
	
	UPROPERTY(EditAnywhere,Category="Inventory")
	float TileSize;
	
	
	
};
	
