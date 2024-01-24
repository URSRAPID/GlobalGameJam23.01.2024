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
	west_wall,
	no_wall
};

UENUM()
enum WALL_TYPE
{
	no_type UMETA(DisplayName = "no type"),

	salon_flat UMETA(DisplayName = "salon flat"),
	chambre_flat UMETA(DisplayName = "chambre flat"),
	cuisine_flat UMETA(DisplayName = "cuisine flat"),
	salle_de_bain_flat UMETA(DisplayName = "salle de bain flat"),

	salon_window UMETA(DisplayName = "salon window"),
	chambre_window UMETA(DisplayName = "chambre window"),
	cuisine_window UMETA(DisplayName = "cuisine window"),
	salle_de_bain_window UMETA(DisplayName = "salle de bain window"),

	salon_door UMETA(DisplayName = "salon door"),
	chambre_door UMETA(DisplayName = "chambre door"),
	cuisine_door UMETA(DisplayName = "cuisine door"),
	salle_de_bain_door UMETA(DisplayName = "salle de bain door")
};

UENUM()
enum ROOMS_CELL
{
	cuisine UMETA(DisplayName = "cuisine"),
	chambre UMETA(DisplayName = "chambre"),
	salon UMETA(DisplayName = "salon"),
	salle_de_bain UMETA(DisplayName = "salle de bain"),
	vide UMETA(DisplayName = "vide"),
};
