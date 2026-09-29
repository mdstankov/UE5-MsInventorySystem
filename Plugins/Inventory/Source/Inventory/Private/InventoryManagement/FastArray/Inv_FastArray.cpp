
#include "InventoryManagement/FastArray/Inv_FastArray.h"

#include "InventoryManagement/Components/Inv_InventoryComponent.h"
#include "Items/Components/Inv_InventoryItem.h"


TArray<UInv_InventoryItem*> FInv_InventoryFastArray::GetAllItems() const
{
	TArray<UInv_InventoryItem*> Results;
	Results.Reserve(Entries.Num());
	for (const auto& Entry : Entries)
	{
		if (!IsValid(Entry.Item))
		{
			continue;
		}		
		Results.Add(Entry.Item);		
	}
	
	return Results;
}

void FInv_InventoryFastArray::PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize)
{
	UInv_InventoryComponent* IC = Cast<UInv_InventoryComponent>(OwnerComponent);
	if (!IsValid(IC))
	{
		return;
	}
	
	for (int32 index : RemovedIndices)
	{
		IC->OnItemRemoved.Broadcast(Entries[index].Item);
	}	
}

void FInv_InventoryFastArray::PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize)
{
	UInv_InventoryComponent* IC = Cast<UInv_InventoryComponent>(OwnerComponent);
	if (!IsValid(IC))
	{
		return;
	}
	
	for (int32 index : AddedIndices)
	{
		IC->OnItemAdded.Broadcast(Entries[index].Item);
	}
}

UInv_InventoryItem* FInv_InventoryFastArray::AddEntry(UInv_InventoryComponent* ItemComponent)
{
	// TODO:
	return nullptr;
}

UInv_InventoryItem* FInv_InventoryFastArray::AddEntry(UInv_InventoryItem* Item)
{
	// By FastArraySerializer instructions
	check(OwnerComponent);
	AActor* OwningActor = OwnerComponent->GetOwner();	
	check(OwningActor->HasAuthority());
	// No return, crash if this happens on a client
	
	FInv_InventoryEntry& NewEntry  = Entries.AddDefaulted_GetRef();
	NewEntry.Item = Item;
	
	// For Serializer
	MarkItemDirty(NewEntry);
	return Item;	
}

void FInv_InventoryFastArray::RemoveEntry(UInv_InventoryItem* Item)
{
	for (auto EntryIt = Entries.CreateIterator();EntryIt; ++EntryIt)
	{
		FInv_InventoryEntry& Entry = *EntryIt;
		if (EntryIt->Item == Item)
		{
			EntryIt.RemoveCurrent();
			// For Serializer
			MarkArrayDirty();
		}
	}
}
