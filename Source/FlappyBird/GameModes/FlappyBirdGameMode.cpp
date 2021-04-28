// Fill out your copyright notice in the Description page of Project Settings.


#include "FlappyBird/GameModes/FlappyBirdGameMode.h"
#include "FlappyBird/PlatformActor.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "FlappyBird/FlappyBirdCharacter.h"
#include "FlappyBird/SaveGame/FlappyBirdSaveGameBase.h"
#include "Internationalization/Text.h"


void AFlappyBirdGameMode::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("Game start!!!"));
	PlayerCharacterRef = Cast<AFlappyBirdCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	PopulatePool();
	SaveGame = Cast<UFlappyBirdSaveGameBase>(UGameplayStatics::LoadGameFromSlot(SaveGameSlotName, 0));
	if (SaveGame == nullptr)
	{
		SaveGame = Cast<UFlappyBirdSaveGameBase>(UGameplayStatics::CreateSaveGameObject(UFlappyBirdSaveGameBase::StaticClass()));
	}
	else
	{
		HighScore = SaveGame->GetHighScore();
	}
	if (PlayerCharacterRef != nullptr)
	{
		PlayerCharacterRef->StopMovement();
		PlayerCharacterRef->DisableControllerInput();
		FVector PlayerLocation = PlayerCharacterRef->GetActorLocation();
		FVector FirstPlatformLocation = PlayerLocation - DistanceBetweenFirstPlatformAndCharacter;
		PlaceNextPlatform(FirstPlatformLocation);
		GetWorld()->GetTimerManager().SetTimer(CountDownTimerHandle, this, &AFlappyBirdGameMode::ResumeCharacterMovementAndEnableInput, CountDownTime, false);
	}

	
	
}

void AFlappyBirdGameMode::PopulatePool()
{
	if (PlatformType != nullptr)
	{
		for (uint8 PlatformIndex = 0; PlatformIndex < PlatformsPoolSize; PlatformIndex++)
		{
			APlatformActor* const Platform = GetWorld()->SpawnActor<APlatformActor>(PlatformType);
			PlatformsPool.Push(Platform);
			PlatformsPool[PlatformIndex]->HideActor();
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("NO PLATFORM TYPE!!!"));
	}
}

void AFlappyBirdGameMode::PlaceNextPlatform(FVector Location)
{

	UE_LOG(LogTemp, Warning, TEXT("Pipe Pairs Shown: %d"), PipePairsShown);
	APlatformActor* AvailablePlatform=nullptr;
	while (AvailablePlatform == nullptr ) {
		for (uint8 PlatformIndex = 0; PlatformIndex < PlatformsPoolSize; PlatformIndex++)
		{
			if (PlatformsPool[PlatformIndex]->GetIsFree())
			{

				UE_LOG(LogTemp, Warning, TEXT("Platform Index: %d"), PlatformIndex);
				AvailablePlatform = PlatformsPool[PlatformIndex];
				break;
			}
		}
	}
	if (AvailablePlatform != nullptr) {
		AvailablePlatform->SetActorLocation(Location);
		AvailablePlatform->RandomizePipesLocationsAndScales();
		AvailablePlatform->ShowActor();
		
		if (SaveGame!=nullptr && PlatformsPool.Num() > 0 && PlatformsPool[0] != nullptr)
		{   
		
			int16 FirstPipePairOnPlatformScore = PipePairsShown + 1;
			int16 LastPipePairOnPlatFromScore = PipePairsShown + PlatformsPool[0]->GetNumberOfPipePairs() + 1;
			if (FirstPipePairOnPlatformScore <= HighScore+1 && HighScore+1 <= LastPipePairOnPlatFromScore && !bHasFlagPoolBeenShown)
			{
				UE_LOG(LogTemp, Warning, TEXT("Lower Limit: %d Upper Limit:%d HighScore: %d"), FirstPipePairOnPlatformScore, LastPipePairOnPlatFromScore, HighScore);
				AvailablePlatform->SpawnPoleAfterPipePair(HighScore+1 - FirstPipePairOnPlatformScore);
				bHasFlagPoolBeenShown = true;
			}
		}
		PipePairsShown += PlatformsPool[0]->GetNumberOfPipePairs();
	}
		


}

void AFlappyBirdGameMode::IncreaseScore()
{
	PlayerScore++;
	
	if (SaveGame != nullptr && HighScore < PlayerScore)
	{
		SaveGame = Cast<UFlappyBirdSaveGameBase>(UGameplayStatics::CreateSaveGameObject(UFlappyBirdSaveGameBase::StaticClass()));
		if (SaveGame != nullptr)
		{   
			if (!bHasBrokenHighScore)
			{
				bHasBrokenHighScore = true;
				if (HighScoreBrokenSound != nullptr)
				{
					UGameplayStatics::SpawnSoundAtLocation(GetWorld(),HighScoreBrokenSound, PlayerCharacterRef->GetActorLocation());
				}
			}
			HighScore++;
			SaveGame->SetHighScore(HighScore);
			if (!UGameplayStatics::SaveGameToSlot(SaveGame, SaveGameSlotName, 0))
			{
				UE_LOG(LogTemp, Error, TEXT("Didn't save new high score!!!"));
			}
		}
		
	}
	else if (SaveGame == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("SAVE GAME IS NULLPTR !!!!"));
	}


}

FText AFlappyBirdGameMode::GetPlayerScore()
{
	return FText::FromString(FString::FromInt(PlayerScore));
}

int32 AFlappyBirdGameMode::GetPipePairsSpawned()
{
	return PipePairsShown;
}

void AFlappyBirdGameMode::FlagPoleShown()
{
	bHasFlagPoolBeenShown = true;
}

bool AFlappyBirdGameMode::GetHasPoleBeenShown()
{
	return bHasFlagPoolBeenShown;
}


void AFlappyBirdGameMode::ResumeCharacterMovementAndEnableInput()
{
	if (PlayerCharacterRef != nullptr)
	{
		PlayerCharacterRef->ResumeMovement();
		PlayerCharacterRef->EnableControllerInput();
	}

}


FText AFlappyBirdGameMode::GetPlayerHighScore()
{
	if (SaveGame == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("SAVE GAME IS NULLPTR !!!!"));
		return FText::FromString(FString("0"));
	}
	return FText::FromString(FString::FromInt(HighScore));
}