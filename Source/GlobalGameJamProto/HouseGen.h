// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WallsScript.h"
#include "HouseGen.generated.h"


UCLASS()
class GLOBALGAMEJAMPROTO_API AHouseGen : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHouseGen();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void FillRoomsArray();
	void GenWalls();
	void SpawnAWall(FVector pos, WALL_TYPE wallType, WALL_LOCATION wallLocation);
	UFUNCTION(BlueprintCallable)
	void GenerateAll();
	UFUNCTION(BlueprintCallable)
	void DestroyWalls();

private:
	UPROPERTY(EditAnywhere)
	float gridOffsetMultiplier;
	UPROPERTY(EditAnywhere)
	TSubclassOf<class AWallsScript> bpWallsClass;
	TArray<TArray<ROOMS_CELL>> roomsArray;
	TArray<TArray<WALL_LOCATION>> wallsNorthArray;
	TArray<TArray<WALL_LOCATION>> wallsEastArray;
	TArray<TArray<WALL_LOCATION>> wallsSouthArray;
	TArray<TArray<WALL_LOCATION>> wallsWestArray;
	TArray<ROOMS_CELL> roomsLeft;

	UPROPERTY(EditAnywhere)
	TMap<TEnumAsByte<ROOMS_CELL>, FVector2D> roomsSizes;
	TArray<AWallsScript*> spawnedWallsArray;
};
