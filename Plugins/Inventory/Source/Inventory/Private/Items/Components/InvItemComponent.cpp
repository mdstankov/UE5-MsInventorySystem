// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Components/InvItemComponent.h"


UInvItemComponent::UInvItemComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	
	PickupMessage = FString("E - Pick Up");	
}

