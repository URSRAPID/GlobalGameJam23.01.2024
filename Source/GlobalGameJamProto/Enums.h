// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Enums.generated.h"

UCLASS()
class GLOBALGAMEJAMPROTO_API AEnums : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnums();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};


enum WALL_LOCATION
{
	north_wall,
	east_wall,
	south_wall,
	west_wall
};

UENUM()
enum WALL_TYPE
{
	salon UMETA(DisplayName = "salon"),
	chambre UMETA(DisplayName = "chambre"),
	cuisine UMETA(DisplayName = "cuisine"),
	salle_de_bain UMETA(DisplayName = "salle de bain")
};

