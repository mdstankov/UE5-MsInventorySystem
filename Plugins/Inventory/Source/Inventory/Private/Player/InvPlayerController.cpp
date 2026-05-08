// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/InvPlayerController.h"
#include <EnhancedInputSubsystems.h>
#include "EnhancedInputComponent.h"

void AInvPlayerController::BeginPlay()
{
	Super::BeginPlay();

	UEnhancedInputLocalPlayerSubsystem* subSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (IsValid(subSystem))
	{
		for (UInputMappingContext* CurrentContext : DefaultIMCs)
		{
			subSystem->AddMappingContext(CurrentContext, 0);
		}
	}
}

void AInvPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// InputComponent inherited from PC
	UEnhancedInputComponent* enchancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);

	enchancedInputComponent->BindAction(PrimaryInteractAction, ETriggerEvent::Started, this,&AInvPlayerController::PrimaryInteract);

}

void AInvPlayerController::PrimaryInteract()
{
	UE_LOG(LogTemp, Log, TEXT("Primary Interact"))

}
