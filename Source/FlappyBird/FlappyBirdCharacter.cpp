// Fill out your copyright notice in the Description page of Project Settings.


#include "FlappyBirdCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
// Sets default values
AFlappyBirdCharacter::AFlappyBirdCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SpringArm = CreateDefaultSubobject< USpringArmComponent>(TEXT("Spring Arm"));
	SpringArm->SetupAttachment(RootComponent);
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
	
}

// Called when the game starts or when spawned
void AFlappyBirdCharacter::BeginPlay()
{
	Super::BeginPlay();
	CharacterMeshRef = GetMesh();
}

// Called every frame
void AFlappyBirdCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!bIsDead && InputEnabled())
	{
		MoveCharacterForward(DeltaTime);
	}
	RotateCharacterDependingCurrentVelocity(DeltaTime);
}

bool AFlappyBirdCharacter::GetIsDead()
{
	return bIsDead;
}


void AFlappyBirdCharacter::MoveCharacterForward(float DeltaTime)
{
	FVector CurrentCharacterLocation = GetActorLocation();
	FVector TargetCharacterLocation = CurrentCharacterLocation + (CharacterVelocityOnTick * DeltaTime);
	SetActorLocation(TargetCharacterLocation);
}

// Called to bind functionality to input
void AFlappyBirdCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", EInputEvent::IE_Pressed, this, &AFlappyBirdCharacter::BirdJump);
}

/*If current velocity(on the z axis) is 500 then the angle (pitch) of the bird should be 40 and if velocity is 0 then the angle is 0 and if the velocity is -500 then the angle is -40
so from that I found out the formula is F(V)=V*0.04 I made it 0.08 so the RInterp could change the value of the angle before the velocity change */
void AFlappyBirdCharacter::RotateCharacterDependingCurrentVelocity(float DeltaTime)
{

	UCharacterMovementComponent* CharacterMovementComponent = GetCharacterMovement();
	if (CharacterMovementComponent)
	{
		FVector CharacterVelocity = CharacterMovementComponent->Velocity;
		FRotator CurrentRotation = GetMesh()->GetRelativeRotation();
		FRotator TargetRotation = FRotator(CharacterVelocity.Z * 0.08f, CurrentRotation.Yaw, CurrentRotation.Roll);
		FRotator InterpolationResult = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaTime, InterpolationSpeedForRotation);
		GetMesh()->SetRelativeRotation(InterpolationResult);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("NO CHAR MOVMENT!!"));
	}
}


void AFlappyBirdCharacter::NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	if (!GetIsDead())
	{
		UGameplayStatics::SpawnSoundAttached(HitSound, GetMesh());
		bIsDead = true;
		DetachCameraOnDeath();
		APlayerController* PlayerControllerRef = GetPlayerController();
		if (PlayerControllerRef != nullptr )
		{
			DisableControllerInput();
			PlayerControllerRef->SetShowMouseCursor(true);
			GetMesh()->SetSimulatePhysics(true);
			GetMesh()->AddImpulse(-Hit.ImpactPoint * 5.0f);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("NO PLAYER CONTROLLER INPUT DETECTED!!!"));
		}

	}
	
}




void AFlappyBirdCharacter::BirdJump()
{
	if (CharacterMeshRef != nullptr)
	{
		UGameplayStatics::SpawnSoundAttached(JumpSound, CharacterMeshRef);
		LaunchCharacter(FVector(0.0f, 0.0f, JumpHeight), false, bOverrideZAxisOnJump);
		float totalDelay = JumpSound->Duration + PlayingFallSoundDelayInSeconds;
		GetWorld()->GetTimerManager().SetTimer(PlayFallingSoundTimerHandle, this, &AFlappyBirdCharacter::PlayFallingSound, totalDelay, false);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("NO CHARACTER MESH DETECTED!"));
	}

}

void AFlappyBirdCharacter::PlayFallingSound()
{
	if (CharacterMeshRef != nullptr)
	{
		UGameplayStatics::SpawnSoundAttached(FallSound, CharacterMeshRef);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("NO CHARACTER MESH DETECTED!"));
	}
	
}

void AFlappyBirdCharacter::DetachCameraOnDeath()
{
	if (SpringArm != nullptr)
	{
		FVector CurrentSpringArmLocation = SpringArm->GetComponentLocation();
		FRotator CurrentSpringArmRotation = SpringArm->GetComponentRotation();
		SpringArm->DetachFromParent();
		SpringArm->SetWorldLocation(CurrentSpringArmLocation);
		SpringArm->SetWorldRotation(CurrentSpringArmRotation);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("NO SPRING-ARM REF DETECTED!"));
	}
	
}

APlayerController* AFlappyBirdCharacter::GetPlayerController()
{
	return UGameplayStatics::GetPlayerController(GetWorld(), 0);
}

void AFlappyBirdCharacter::StopMovement()
{
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
}

void AFlappyBirdCharacter::ResumeMovement()
{
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
}

void AFlappyBirdCharacter::EnableControllerInput()
{
	APlayerController* PlayerControllerRef = GetPlayerController();
	if (PlayerControllerRef != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("EnableControllerInput"));
		EnableInput(PlayerControllerRef);
    }
	else
	{
		UE_LOG(LogTemp, Error, TEXT("No CONTROLLER! !!"));
	}


}

void AFlappyBirdCharacter::DisableControllerInput()
{
	APlayerController* PlayerControllerRef = GetPlayerController();
	if (PlayerControllerRef != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("DisableControllerInput"));
		DisableInput(PlayerControllerRef);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("No CONTROLLER! !!"));
	}
	
}

