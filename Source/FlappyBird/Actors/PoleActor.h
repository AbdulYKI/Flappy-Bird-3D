// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PoleActor.generated.h"
class USkeletalMeshComponent;
UCLASS()
class FLAPPYBIRD_API APoleActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APoleActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


private:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mesh", meta = (AllowPrivateAccess = "true"))
		USkeletalMeshComponent* PoleMesh;

	

};
