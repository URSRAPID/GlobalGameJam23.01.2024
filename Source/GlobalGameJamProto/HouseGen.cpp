// Fill out your copyright notice in the Description page of Project Settings.


#include "HouseGen.h"

// Sets default values
AHouseGen::AHouseGen()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AHouseGen::BeginPlay()
{
	Super::BeginPlay();	


    FillRoomsArray();
    GenWalls();
}

// Called every frame
void AHouseGen::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AHouseGen::FillRoomsArray()
{
    TArray<ROOMS_CELL> emptyRow;
    TArray<WALL_LOCATION> emptyWallsRow;
    emptyRow.Init(ROOMS_CELL::vide, 10);
    emptyWallsRow.Init(WALL_LOCATION::no_wall, 10);
    roomsArray.Init(emptyRow, 10);
    wallsNorthArray.Init(emptyWallsRow, 10);
    wallsEastArray.Init(emptyWallsRow, 10);
    wallsSouthArray.Init(emptyWallsRow, 10);
    wallsWestArray.Init(emptyWallsRow, 10);


    roomsLeft.Add(ROOMS_CELL::chambre);
    roomsLeft.Add(ROOMS_CELL::cuisine);
    roomsLeft.Add(ROOMS_CELL::salle_de_bain);

    UE_LOG(LogTemp, Warning, TEXT("%d"), roomsLeft.Num());

    ROOMS_CELL currentRoom;
    int roomsLeftNum = roomsLeft.Num();
    int xOffset = 0;
    int yOffset = 0;
    for (int i = 0; i < roomsLeftNum; i++) {
        currentRoom = roomsLeft[FMath::RandRange(0, roomsLeft.Num()-1)];
        roomsLeft.Remove(currentRoom);
        UE_LOG(LogTemp, Warning, TEXT("%d"), roomsLeft.Num());
        int minRoomSize = roomsSizes[currentRoom].X;
        int maxRoomSize = roomsSizes[currentRoom].Y;
        int xRoomSize = FMath::RandRange(minRoomSize, maxRoomSize);
        int yRoomSize = FMath::RandRange(minRoomSize, maxRoomSize);
        for (int j = 0; j < xRoomSize; j++) { //commencer à gen prochaine salle avec offset 1ère salle, offset que par y au 1er puis par x 2ème (+ offset 1er - maxSize)
            for (int k = 0; k < yRoomSize; k++) {
                if (j + xOffset < roomsArray.Num() && k + yOffset < roomsArray[0].Num() && roomsArray[j + xOffset][k + yOffset] == ROOMS_CELL::vide) {
                    roomsArray[j + xOffset][k + yOffset] = currentRoom;
                    UE_LOG(LogTemp, Warning, TEXT("%d,%d"), j,k);
                    if (k == 0) {
                        wallsWestArray[j + xOffset][k + yOffset] = WALL_LOCATION::west_wall;
                    }
                    if (j == 0) {
                        wallsSouthArray[j + xOffset][k + yOffset] = WALL_LOCATION::south_wall;
                    }
                    if (k == yRoomSize-1) {
                        wallsEastArray[j + xOffset][k + yOffset] = WALL_LOCATION::east_wall;
                    }
                    if (j == xRoomSize-1) {
                        wallsNorthArray[j + xOffset][k + yOffset] = WALL_LOCATION::north_wall;
                    }
                }
            }
        }
        xOffset = xRoomSize;
        yOffset = yRoomSize;
    }
}

void AHouseGen::GenWalls()
{
    for (int i = 0; i < wallsNorthArray.Num(); i++) {
        for (int j = 0; j < wallsNorthArray.Num(); j++) {
            if (wallsNorthArray[i][j] != WALL_LOCATION::no_wall) {
                SpawnAWall(FVector(i * gridOffsetMultiplier, j*gridOffsetMultiplier, GetActorLocation().Z), WALL_TYPE::chambre_flat, wallsNorthArray[i][j]);
            }
        }
    }
    for (int i = 0; i < wallsEastArray.Num(); i++) {
        for (int j = 0; j < wallsEastArray.Num(); j++) {
            if (wallsEastArray[i][j] != WALL_LOCATION::no_wall) {
                SpawnAWall(FVector(i * gridOffsetMultiplier, j*gridOffsetMultiplier, GetActorLocation().Z), WALL_TYPE::chambre_flat, wallsEastArray[i][j]);
            }
        }
    }
    for (int i = 0; i < wallsSouthArray.Num(); i++) {
        for (int j = 0; j < wallsSouthArray.Num(); j++) {
            if (wallsSouthArray[i][j] != WALL_LOCATION::no_wall) {
                SpawnAWall(FVector(i * gridOffsetMultiplier, j*gridOffsetMultiplier, GetActorLocation().Z), WALL_TYPE::chambre_flat, wallsSouthArray[i][j]);
            }
        }
    }
    for (int i = 0; i < wallsWestArray.Num(); i++) {
        for (int j = 0; j < wallsWestArray.Num(); j++) {
            if (wallsWestArray[i][j] != WALL_LOCATION::no_wall) {
                SpawnAWall(FVector(i * gridOffsetMultiplier, j*gridOffsetMultiplier, GetActorLocation().Z), WALL_TYPE::chambre_flat, wallsWestArray[i][j]);
            }
        }
    }
}

void AHouseGen::SpawnAWall(FVector pos, WALL_TYPE wallType, WALL_LOCATION wallLocation)
{
    FRotator rot;
    FActorSpawnParameters spawnParams;
    spawnParams.Owner = this;

    AWallsScript* mySpawn = GetWorld()->SpawnActor<AWallsScript>(bpWallsClass, pos, rot, spawnParams);
    mySpawn->SetWallLocationAndType(wallType, wallLocation);
    mySpawn->SetStaticMesh();
    mySpawn->RotateStaticMeshToLocation();
}


