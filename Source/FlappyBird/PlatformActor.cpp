// Fill out your copyright notice in the Description page of Project Settings.


#include "PlatformActor.h"
#include "PipeActor.h"
#include "Math/UnrealMathUtility.h"
#include "Components/CapsuleComponent.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "FlappyBird/GameModes/FlappyBirdGameMode.h"
#include "Components/BoxComponent.h"
#include "FlappyBirdCharacter.h"
#include "FlappyBird/Actors/PoleActor.h"

// Sets default values
APlatformActor::APlatformActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;
	GameModeRef = Cast<AFlappyBirdGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	InitializeMeshes();
	InitializeSpawnLocations();
	InitializeBoxes();
}

// Called when the game starts or when spawned
void APlatformActor::BeginPlay()
{
	Super::BeginPlay();
	CharacterRef = Cast<AFlappyBirdCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	PlaceNextPlatformBox->OnComponentEndOverlap.AddDynamic(this,&APlatformActor::OnPlaceNextPlatformBoxOverlapEnd);
	ScoreBox->OnComponentEndOverlap.AddDynamic(this, &APlatformActor::OnScoreBoxOverlapEnd);
	if (PipeActorType != nullptr)
	{
		SpawnPipes();
		RandomizePipesLocationsAndScales();
		MoveScoreBoxToNextPairOfPipes();

	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("PIPE TYPE NOT CHOSEN!"));
	}
	
	
}

// Called every frame
void APlatformActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APlatformActor::InitializeMeshes()
{
	FloorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Floor Mesh"));
	FloorMesh->SetupAttachment(RootComponent);
	CeilingMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Ceiling Mesh"));
	CeilingMesh->SetupAttachment(RootComponent);	
}

void APlatformActor::InitializeSpawnLocations()
{
	FirstFloorSpawnLocation = CreateDefaultSubobject<USceneComponent>(TEXT("Floor First Spawn Point"));
	FirstFloorSpawnLocation->SetupAttachment(FloorMesh);
	FirstCeilingSpawnLocation = CreateDefaultSubobject<USceneComponent>(TEXT("Ceiling First Spawn Point"));
	FirstCeilingSpawnLocation->SetupAttachment(CeilingMesh);
	NextPlatfromSpawnLocation = CreateDefaultSubobject<USceneComponent>(TEXT("Next Platform Spawn Point"));
	NextPlatfromSpawnLocation->SetupAttachment(FloorMesh);
}

void APlatformActor::InitializeBoxes()
{
	PlaceNextPlatformBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Place Next Platform Box"));
	PlaceNextPlatformBox->SetupAttachment(RootComponent);
	ScoreBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Score Box"));
	ScoreBox->SetupAttachment(RootComponent);
}


void APlatformActor::RandomizePipesLocationsAndScales()
{
	for (int8 PipeIndex = 0; PipeIndex < NumberOfPipePairs; PipeIndex++)
	{
		APipeActor* const FloorPipe = FloorPipes[PipeIndex];
		float FloorPipeZScale = GetRandomZScale();
		FloorPipe->SetActorScale3D(FVector(1.0f, 0.75f, FloorPipeZScale));

		APipeActor* const CeilingPipe = CeilingPipes[PipeIndex];
		CeilingPipe->SetActorScale3D(FVector(1.0f, 0.75f,3.8f - FloorPipeZScale));

		if (PipeIndex == 0)
		{
			CeilingPipe->SetActorLocation(FirstCeilingSpawnLocation->GetComponentLocation());
			FloorPipe->SetActorLocation(FirstFloorSpawnLocation->GetComponentLocation());
		}
		else
		{
			FVector PreviousFloorPipeLocation = FloorPipes[PipeIndex - 1]->GetActorLocation();
			float DistanceFromPreviousPipeOnY = RandomYLocation(PreviousFloorPipeLocation.Y);
			FVector CurrentFloorPipeLocation = FVector(PreviousFloorPipeLocation.X, DistanceFromPreviousPipeOnY, PreviousFloorPipeLocation.Z);
			FloorPipe->SetActorLocation(CurrentFloorPipeLocation);

			FVector PreviousCeilingPipeLocation = CeilingPipes[PipeIndex - 1]->GetActorLocation();
			FVector CurrentCeilingPipeLocation = FVector(PreviousCeilingPipeLocation.X, DistanceFromPreviousPipeOnY, PreviousCeilingPipeLocation.Z);
			CeilingPipe->SetActorLocation(CurrentCeilingPipeLocation);

		}
		FloorPipe->SetActorRotation(FRotator(0.0f, 0.0f, 0.0f));
		CeilingPipe->SetActorRotation(FRotator(180.0f, 0.0f, 0.0f));
	
	}

}



void APlatformActor::OnScoreBoxOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AFlappyBirdCharacter* PlayerCharacter =Cast<AFlappyBirdCharacter>(OtherActor);
	if (PlayerCharacter!=nullptr && !PlayerCharacter->GetIsDead())
	{
		UGameplayStatics::SpawnSoundAttached(ScoreSound, ScoreBox);
		MoveScoreBoxToNextPairOfPipes();
		GameModeRef->IncreaseScore();
		UE_LOG(LogTemp, Warning, TEXT("Score!"));
	}
	

}

void APlatformActor::SpawnPipes()
{

	for (int8 PipeIndex = 0; PipeIndex < NumberOfPipePairs; PipeIndex++)
	{
		APipeActor* const FloorPipe= GetWorld()->SpawnActor<APipeActor>(PipeActorType);
		APipeActor* const CeilingPipe = GetWorld()->SpawnActor<APipeActor>(PipeActorType);
		FloorPipe->SetOwner(this);
		FloorPipes.Push(FloorPipe);
		CeilingPipe->SetOwner(this);
		CeilingPipes.Push(CeilingPipe);
	}
}

void APlatformActor::MoveScoreBoxToNextPairOfPipes()
{
	ScoreBoxPipeIndex = (ScoreBoxPipeIndex + 1) % NumberOfPipePairs;
	FVector FloorPipeLocation = FloorPipes[ScoreBoxPipeIndex]->GetActorLocation();
	FVector CurrentScoreBoxLocation = ScoreBox->GetComponentLocation();
	FVector TargetLocationForScoreBox = FVector(CurrentScoreBoxLocation.X, FloorPipeLocation.Y, CurrentScoreBoxLocation.Z);
	ScoreBox->SetWorldLocation(TargetLocationForScoreBox);
}

float APlatformActor::GetRandomZScale()
{
    int8 index = FMath::RandRange(0, RandomZScales.Num()-1);
	return RandomZScales[index];
}

int8 APlatformActor::GetNumberOfPipePairs()
{
	return NumberOfPipePairs;
}

float APlatformActor::RandomYLocation(float OldYLocation)
{
	return  OldYLocation - FMath::FRandRange(MinYDifference, MaxYDifference);
}

void APlatformActor::SetActorVisbility(bool Visible)
{
	SetActorHiddenInGame(!Visible);
	bIsFree = !Visible;
	for (int PipeIndex = 0; PipeIndex < NumberOfPipePairs; PipeIndex++)
	{
		FloorPipes[PipeIndex]->SetActorHiddenInGame(!Visible);
		CeilingPipes[PipeIndex]->SetActorHiddenInGame(!Visible);
	}
}

void APlatformActor::HideActor() {
	if (CharacterRef != nullptr && !CharacterRef->GetIsDead())
	{
		SetActorVisbility(false);
	}
	

}

void APlatformActor::ShowActor()
{
	SetActorVisbility(true);
	HideActorAfterXSeconds();
}
void APlatformActor::HideActorAfterXSeconds()
{
	GetWorld()->GetTimerManager().SetTimer(HidingActorTimerHandle,this,&APlatformActor::HideActor, TimeBeforeHidingPlatform, false);
}

bool APlatformActor::GetIsFree()
{
	return bIsFree;
}

void APlatformActor::SpawnPoleAfterPipePair(int16 PipePairIndex)
{
	if (PipePairIndex < FloorPipes.Num() && PoleActorType != nullptr)
	{
		FVector FloorPipeLocation = FloorPipes[PipePairIndex]->GetActorLocation();
		APoleActor* Pole = GetWorld()->SpawnActor<APoleActor>(PoleActorType);
		Pole->SetActorLocation(FloorPipeLocation + PoleOffsetFromFloorPipe);
		Pole->SetActorScale3D(PoleScale);
		Pole->SetActorRotation(FQuat(FRotator(0.0f, 90.0f, 0.0f)));
	}
	else if(PipePairIndex >= FloorPipes.Num())
	{
		UE_LOG(LogTemp, Error, TEXT("OUT OF RANGE INDEX!!!"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("POLE TYPE NOT CHOSEN!!!"));
	}

   
}

void APlatformActor::NotifyGameModeToPlaceNextPlatform()
{
	if (GameModeRef)
	{
		GameModeRef->PlaceNextPlatform(NextPlatfromSpawnLocation->GetComponentLocation());
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("NO FLAPPY BIRD GAME MODE SELECTED !!!"));
	}
}

void APlatformActor::OnPlaceNextPlatformBoxOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (Cast<AFlappyBirdCharacter>(OtherActor))
	{
		NotifyGameModeToPlaceNextPlatform();
	}
}

