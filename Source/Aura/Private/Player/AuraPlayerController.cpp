// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/AuraPlayerController.h"

#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Interaction/EnemyInterface.h"

AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true;
}

void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();

	check(AuraContext);

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	check(Subsystem);
	Subsystem->AddMappingContext(AuraContext, 0);

	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;

	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeData.SetHideCursorDuringCapture(false);
	SetInputMode(InputModeData);
}

void AAuraPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	CursorTrace();
}

void AAuraPlayerController::CursorTrace()
{
	FHitResult CursorHit;
	GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);
	if (!CursorHit.bBlockingHit) return;

	LastActor = CurrActor;
	CurrActor = Cast<IEnemyInterface>(CursorHit.GetActor());

	/**
	 * Line trace from cursor. There are several scenarios:
	 *  A. LastActor == null && CurrActor == null
	 *		- Do nothing
	 *  B. LastActor == null && CurrActor != null
	 *		- Call CurrActor::Hightlight
	 *	C. LastActor != null && CurrActor == null
	 *		- Call CurrActor::UnHighlight
	 *	D. LastActor != null && CurrActor != null && LastActor != CurrActor
	 *		- Call LastActor::UnHighlight && CurrActor::Highlight
	 *	E. LastActor != null && CurrActor != null && LastActor == CurrActor
	 *		- Do nothing
	 */

	//if (LastActor == nullptr)
	//{
	//	if (CurrActor != nullptr)
	//	{
	//		// Case B
	//		CurrActor->HighlightActor();
	//	} else // CurrActor is valid
	//	{
	//		// Case A - do nothing
	//	}
	//} else // LastActor is valid
	//{
	//	if (CurrActor == nullptr)
	//	{
	//		// Cast C
	//		LastActor->UnHighlightActor();
	//	} else // CurrActor is valid
	//	{
	//		if (LastActor == CurrActor)
	//		{
	//			// Case E - do nothing
	//		} else 
	//		{
	//			// Case D
	//			LastActor->UnHighlightActor();
	//			CurrActor->HighlightActor();
	//		}
	//	}
	//}

	if (LastActor != CurrActor)
	{
		if (LastActor != nullptr)
		{
			LastActor->UnHighlightActor();
		} 
		if (CurrActor != nullptr)
		{
			CurrActor->HighlightActor();
		}
	}
}

void AAuraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);

	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAuraPlayerController::Move);
}

void AAuraPlayerController::Move(const FInputActionValue& InputActionValue)
{
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation = FRotator(0.f, Rotation.Yaw, 0.f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
		ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);
	}
}

