// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlatformActor.generated.h"
class UStaticMeshComponent;
class APipeActor;
class USkeletalMeshComponent;
class UBoxComponent;
class AFlappyBirdGameMode;
class AFlappyBirdCharacter;
class APoleActor;
UCLASS()
class FLAPPYBIRD_API APlatformActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APlatformActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	

	void RandomizePipesLocationsAndScales();

	float GetRandomZScale();

	int8 GetNumberOfPipePairs();

	void HideActor();

	void ShowActor();

	bool GetIsFree();

	void SpawnPoleAfterPipePair(int16 PipePairIndex);

private:
	AFlappyBirdGameMode* GameModeRef;

	AFlappyBirdCharacter* CharacterRef;
	
	bool bIsFree = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "General", meta = (AllowPrivateAccess = "true"))
		USceneComponent* Root;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mesh", meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* FloorMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mesh", meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* CeilingMesh;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn", meta = (AllowPrivateAccess = "true"))
		USceneComponent* FirstFloorSpawnLocation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn", meta = (AllowPrivateAccess = "true"))
		USceneComponent* FirstCeilingSpawnLocation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn", meta = (AllowPrivateAccess = "true"))
		USceneComponent* NextPlatfromSpawnLocation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pipe Actor Class", meta = (AllowPrivateAccess = "true"))
		TSubclassOf<APipeActor> PipeActorType;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pole Actor Class", meta = (AllowPrivateAccess = "true"))
		TSubclassOf<APoleActor> PoleActorType;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn", meta = (AllowPrivateAccess = "true"))
		int32 NumberOfPipePairs = 6;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Scale Limits", meta = (AllowPrivateAccess = "true"))
		float MinZScale = 0.7f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Scale Limits", meta = (AllowPrivateAccess = "true"))
		float MaxZScale = 1.7f;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Distance Between Pipes Limits", meta = (AllowPrivateAccess = "true"))
		float MinYDifference = 300.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Distance Between Pipes Limits", meta = (AllowPrivateAccess = "true"))
		float MaxYDifference = 500.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pool", meta = (AllowPrivateAccess = "true"))
		float TimeBeforeHidingPlatform = 30.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Triggers", meta = (AllowPrivateAccess = "true"))
		UBoxComponent* PlaceNextPlatformBox;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Triggers", meta = (AllowPrivateAccess = "true"))
		UBoxComponent* ScoreBox;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sound", meta = (AllowPrivateAccess = "true"))
		USoundBase* ScoreSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pole", meta = (AllowPrivateAccess = "true"))
		FVector PoleOffsetFromFloorPipe =FVector(-200.0f,-200.0f,0.0f);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pole", meta = (AllowPrivateAccess = "true"))
		FVector PoleScale = FVector(0.05f,0.05f,0.05f);

	TArray<APipeActor*> FloorPipes;

	TArray<APipeActor*> CeilingPipes;


	float RandomYLocation(float OldYLocation);

	void SetActorVisbility(bool Visible);

	void HideActorAfterXSeconds();

	void NotifyGameModeToPlaceNextPlatform();

	void InitializeMeshes();

	void InitializeSpawnLocations();

	void InitializeBoxes();

	UFUNCTION()
		void OnPlaceNextPlatformBoxOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
		void OnScoreBoxOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void SpawnPipes();

	TArray<float> RandomZScales = { 1.3f, 1.63f, 1.96f, 2.5f, 2.1f, 2.2f, 0.9f, 2.7f, 0.9f, 3.1f };

	FTimerHandle HidingActorTimerHandle;

	int8 ScoreBoxPipeIndex = -1;

	void MoveScoreBoxToNextPairOfPipes();
	

	
	

};
