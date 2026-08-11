// Fill out your copyright notice in the Description page of Project Settings.


#include "RunnerCharacter.h"

#include "VM_Stamina.h"
#include "View/MVVMView.h"
#include "Blueprint/UserWidget.h"
#include "MVVMSubsystem.h"
#include "Spikes.h"
#include "WallSpike.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine.h"

// Sets default values
ARunnerCharacter::ARunnerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Overlap);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	SideViewCamera = CreateDefaultSubobject<UCameraComponent>(FName("SideViewCamera"));
	SideViewCamera->bUsePawnControlRotation	= false;	
		
	//Remove this later for future game design choice
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f);
	
	GetCharacterMovement()->GravityScale = 2.0f;
	GetCharacterMovement()->AirControl = 0.8f;
	GetCharacterMovement()->JumpZVelocity = 1000.0f;
	GetCharacterMovement()->GroundFriction = 3.0f;
	GetCharacterMovement()->MaxWalkSpeed = 600.0f;
	GetCharacterMovement()->MaxFlySpeed = 600.0f;

	tempPos = GetActorLocation();
	zPosition = tempPos.Z + 300.0f;
	
}

// Called when the game starts or when spawned
void ARunnerCharacter::BeginPlay()
{
	Super::BeginPlay();

	CanMove = true;

	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &ARunnerCharacter::OnOverlapBegin);

	StaminaViewModel = NewObject<UVM_Stamina>(this);

	APlayerController* PC = Cast<APlayerController>(GetController());

	if (!PC)
	{
		PC = GetWorld()->GetFirstPlayerController();
	}

	if (StaminaWidgetClass && PC)
	{
		StaminaWidgetInstance = CreateWidget<UUserWidget>(PC, StaminaWidgetClass);

		if (StaminaWidgetInstance)
		{
			StaminaWidgetInstance->AddToViewport();

			if (UMVVMView* MVVMView = StaminaWidgetInstance->GetExtension<UMVVMView>())
			{
				MVVMView->SetViewModel(FName("VM_Stamina"), StaminaViewModel);
			}	
		}
	}	
}

void ARunnerCharacter::OnTouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
	TouchStartLocation = FVector2D(Location.X, Location.Y);
}

void ARunnerCharacter::OnTouchEnded(ETouchIndex::Type FingerIndex, FVector Location)
{
	if (!CanMove) return;

	FVector2D TouchEndLocation = FVector2D(Location.X, Location.Y);
	FVector2D SwipeDelta = TouchEndLocation - TouchStartLocation;
	
	if (SwipeDelta.Size() < SwipeMinDistance) return;
	
	float SwipeLength = FMath::Min(SwipeDelta.Size(), SwipeMaxDistance);
	FVector2D SwipeNormalized = SwipeDelta.GetSafeNormal();
	FVector2D SwipeClamped = SwipeNormalized * SwipeLength;
	
	float SwipeRatio = SwipeLength / SwipeMaxDistance;	
	float ImpulseY = SwipeClamped.X;   
	float ImpulseZ = -SwipeClamped.Y; 
	
	if (ImpulseY < 0.0f) ImpulseY = 0.0f;

	FVector Impulse = FVector(0.0f, ImpulseY, ImpulseZ) * SwipeImpulseStrength * SwipeRatio;

	if (!Impulse.IsZero())
	{
		FVector CurrentVel = GetCharacterMovement()->Velocity;
		GetCharacterMovement()->Velocity = FVector(CurrentVel.X, CurrentVel.Y, 0.0f);

		if (StaminaViewModel && StaminaViewModel->GetStamina() >= ActionStaminaCost)
		{
			StaminaViewModel->ConsumeStamina(ActionStaminaCost);
			LaunchCharacter(Impulse, false, true);
		}  				
	}
}

void ARunnerCharacter::OnTouchMoved(ETouchIndex::Type FingerIndex, FVector Location)
{
	APlayerController* PC = Cast<APlayerController>(GetController());
	
	if (!PC) return;

	FVector WorldLocation, WorldDirection;
	PC->DeprojectScreenPositionToWorld(Location.X, Location.Y, WorldLocation, WorldDirection);

	FVector SpawnPos = WorldLocation + WorldDirection * 500.0f;

	DrawDebugLine(GetWorld(), LastSwipeWorldPos, SpawnPos, FColor::Red, false, 1.0f, 0, 2.0f);
	LastSwipeWorldPos = SpawnPos;
}

// Called every frame
void ARunnerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	tempPos = GetActorLocation();
	tempPos.X -= 1000.0f;
	tempPos.Y += 500.0f;
	tempPos.Z = zPosition;	
	SideViewCamera->SetWorldLocation(tempPos);
	MoveRight(1.0f);

}

// Called to bind functionality to input
void ARunnerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ARunnerCharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ARunnerCharacter::StopJumping);
	PlayerInputComponent->BindTouch(IE_Pressed, this, &ARunnerCharacter::OnTouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &ARunnerCharacter::OnTouchEnded);
	PlayerInputComponent->BindTouch(IE_Repeat, this, &ARunnerCharacter::OnTouchMoved);
	
}

void ARunnerCharacter::MoveRight(float Value)
{
	if(CanMove)
	{
		AddMovementInput(FVector(0.0f, 1.0f, 0.0f), Value);
	}
}

void ARunnerCharacter::Jump()
{
	if (StaminaViewModel && StaminaViewModel->GetStamina() >= ActionStaminaCost)
	{
		Super::Jump();
		StaminaViewModel->ConsumeStamina(ActionStaminaCost);
	}	
}

void ARunnerCharacter::RespawnCharacter()
{
	UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()));
}

void ARunnerCharacter::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor != nullptr)
	{
		ASpikes* WallSpike = Cast<AWallSpike>(OtherActor);
		ASpikes* Spike = Cast<ASpikes>(OtherActor);

		if(WallSpike || Spike)
		{
			GetMesh()->Deactivate();
			GetMesh()->SetVisibility(false);

			CanMove = false;

			FTimerHandle UnusedHandle;
			GetWorldTimerManager().SetTimer(UnusedHandle, this, &ARunnerCharacter::RespawnCharacter, 2.f, false);
		}
	}
}

