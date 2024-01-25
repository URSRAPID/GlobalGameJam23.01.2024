// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Enums.h"
#include "Components/ArrowComponent.h"
#include "WallsScript.generated.h"

UCLASS()
class GLOBALGAMEJAMPROTO_API AWallsScript : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWallsScript();


	void SetStaticMesh();
	void RotateStaticMeshToLocation();
	void SetWallLocationAndType(WALL_TYPE wallType, WALL_LOCATION wallLocation);

	void SpawnMeuble();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	TEnumAsByte<WALL_TYPE> currentWallType;
	TEnumAsByte<WALL_LOCATION> currentWallLocation;
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* staticMeshComponent = nullptr;
	UArrowComponent* arrowComponent = nullptr;

	UPROPERTY(EditAnywhere)
	TMap<TEnumAsByte<WALL_TYPE>,UStaticMesh*> wallTypeMap;
	UPROPERTY(EditAnywhere)
	TMap<TEnumAsByte<ROOMS_CELL>, UStaticMesh*> floorTypeMap;
	float WallLocationToRotation();
	UPROPERTY(EditAnywhere)
	AActor* meubleActor;

};
