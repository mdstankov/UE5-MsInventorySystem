// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/InvPlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Interaction/InvHighlightable.h"
#include "Items/Components/InvItemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Widgets/HUD/Inv_HUDWidget.h"

AInvPlayerController::AInvPlayerController()
{
	PrimaryActorTick.bCanEverTick = true;
	TraceLength = 500.0;
	ItemTraceChannel = ECC_GameTraceChannel1;
}

void AInvPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);	
	TraceForItem();
}

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
		
		CreateHUDWidget();
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

void AInvPlayerController::CreateHUDWidget()
{
	if (!IsLocalController()) return;
	
	HUDWidget = CreateWidget<UInv_HUDWidget>(this, HUDWidgetClass);
	if(IsValid(HUDWidget))
	{
		HUDWidget->AddToViewport();
	}	
}

void AInvPlayerController::TraceForItem()
{
	if (!IsValid(GEngine) || !IsValid(GEngine->GameViewport)) return;
	
	FVector2d viewportSize;
	GEngine->GameViewport->GetViewportSize(viewportSize);
	
	const FVector2d viewportCenter = viewportSize / 2.f;
	
	FVector TraceStart;
	FVector Forward;	
	if (!UGameplayStatics::DeprojectScreenToWorld(this, viewportCenter,TraceStart,Forward )) return;
		
	const FVector TraceEnd = TraceStart + Forward * TraceLength;
	FHitResult HitResult;
	GetWorld()->LineTraceSingleByChannel(HitResult,TraceStart,TraceEnd, ItemTraceChannel);
	
	LastActor = ThisActor;
	ThisActor = HitResult.GetActor();
	
	if (!ThisActor.IsValid())
	{
		if (IsValid(HUDWidget))
		{
			HUDWidget->HidePickupMessage();
		}
	}	
	
	if (ThisActor == LastActor) return;
	
	if (ThisActor.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("Started Tracing NEW actor"));		
		if ( UActorComponent* Highlightable = ThisActor->FindComponentByInterface(UInvHighlightable::StaticClass()); IsValid(Highlightable))
		{
			IInvHighlightable::Execute_Highlight(Highlightable);
		}		
		
		UInvItemComponent* ItemComponent = ThisActor->FindComponentByClass<UInvItemComponent>();
		if (!IsValid(ItemComponent)) return;
		
		if (IsValid(HUDWidget))
		{
			HUDWidget->ShowPickupMessage(ItemComponent->GetPickupMessage());
		}
	}	
	
	if (LastActor.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("Stopped Tracing LAST actor"));
		if ( UActorComponent* Highlightable = LastActor->FindComponentByInterface(UInvHighlightable::StaticClass()); IsValid(Highlightable))
		{
			IInvHighlightable::Execute_UnHighlight(Highlightable);
		}
	}
		
	
}
