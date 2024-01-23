// Fill out your copyright notice in the Description page of Project Settings.


#include "WallsScript.h"

// Sets default values
AWallsScript::AWallsScript()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AWallsScript::BeginPlay()
{
	Super::BeginPlay();
	staticMeshComponent = FindComponentByClass<UStaticMeshComponent>();
	arrowComponent = FindComponentByClass<UArrowComponent>();
}

// Called every frame
void AWallsScript::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWallsScript::SetStaticMesh()
{
	if (staticMeshComponent == nullptr) { return; }
	staticMeshComponent->SetStaticMesh(wallTypeMap[currentWallType]);
}

void AWallsScript::RotateStaticMeshToLocation()
{
	if (arrowComponent == nullptr) { return; }
	arrowComponent->SetRelativeRotation(FRotator(0,WallLocationToRotation(), 0));
}

void AWallsScript::SetWallLocationAndType(WALL_TYPE wallType, WALL_LOCATION wallLocation)
{
	currentWallLocation = wallLocation;
	currentWallType = wallType;
}

float AWallsScript::WallLocationToRotation()
{
	switch (currentWallLocation) {
	case WALL_LOCATION::north_wall:
		return 0.0f;
	case WALL_LOCATION::east_wall:
		return 90.0f;
	case WALL_LOCATION::south_wall:
		return 180.0f;
	case WALL_LOCATION::west_wall:
		return 270.0f;
	}
	return 0.0f;
}





