#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

#include "Inv_ItemFragment.generated.h"

// Instanced structs can be used polymorphicly with reflection, rq dest and constr,copy and move, we dont use UE shelter
// Saved in Item Manifist
USTRUCT(BlueprintType)
struct FInv_ItemFragment
{
	GENERATED_BODY()
		
	FInv_ItemFragment() {}
	FInv_ItemFragment(const FInv_ItemFragment& other) = default;
	FInv_ItemFragment& operator=(const FInv_ItemFragment& other) = default;	
	FInv_ItemFragment(FInv_ItemFragment&& other) = default;
	FInv_ItemFragment& operator=(FInv_ItemFragment&&) = default;
	virtual ~FInv_ItemFragment() {} 
		
	FGameplayTag GetFragmentTag() const { return FragmentTag; }
	void SetFragmentTag(FGameplayTag Tag) { FragmentTag = Tag; }
	
private:
	UPROPERTY(EditAnywhere, Category="Inventory")
	FGameplayTag FragmentTag = FGameplayTag::EmptyTag;
};

USTRUCT(BlueprintType)
struct FInv_GridFragment : public FInv_ItemFragment
{
	GENERATED_BODY()
	
	FIntPoint GetGridSize() const { return GridSize; }
	void SetGridSize(FIntPoint& Size) { GridSize = Size; }
	
	float GetGridPadding() const { return GridPadding; }
	void SetGridPadding(float Padding) { GridPadding = Padding; }
	
private:
	UPROPERTY(EditAnywhere, Category="Inventory")
	FIntPoint GridSize{1,1};
	
	UPROPERTY(EditAnywhere, Category="Inventory")	
	float GridPadding{0.f};
};

// Inventory icon, tooltip, ect
USTRUCT(BlueprintType)
struct FInv_ImageFragment : public FInv_ItemFragment
{
	GENERATED_BODY()

	UTexture2D* GetIcon() const { return Icon; }
	
private:
	UPROPERTY(EditAnywhere, Category="Inventory")
	TObjectPtr<UTexture2D> Icon{nullptr};
	
	UPROPERTY(EditAnywhere, Category="Inventory")
	FVector2D IconDimension{FVector2D(44.f, 44.f)};	
};

// For Stackable
USTRUCT(BlueprintType)
struct FInv_StackableFragment : public FInv_ItemFragment
{
	GENERATED_BODY()
	
	int32 GetMaxStackSize() const { return MaxStacks; }
	int32 GetStackCount() const { return StackCount; }
	
private:
	UPROPERTY(EditAnywhere, Category="Inventory")
	int32 MaxStacks{1};
	
	UPROPERTY(EditAnywhere, Category="Inventory")
	int32 StackCount{1};
};

