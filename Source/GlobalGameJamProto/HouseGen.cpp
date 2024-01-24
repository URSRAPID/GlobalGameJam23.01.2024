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

    GenerateAll();
}

// Called every frame
void AHouseGen::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AHouseGen::FillRoomsArray()
{
    int arraysSize = 15;
    TArray<ROOMS_CELL> emptyRow;
    TArray<WALL_LOCATION> emptyWallsRow;
    emptyRow.Init(ROOMS_CELL::vide, arraysSize);
    emptyWallsRow.Init(WALL_LOCATION::no_wall, arraysSize);
    roomsArray.Init(emptyRow, arraysSize);
    wallsNorthArray.Init(emptyWallsRow, arraysSize);
    wallsEastArray.Init(emptyWallsRow, arraysSize);
    wallsSouthArray.Init(emptyWallsRow, arraysSize);
    wallsWestArray.Init(emptyWallsRow, arraysSize);


    roomsLeft.Add(ROOMS_CELL::chambre);
    roomsLeft.Add(ROOMS_CELL::salle_de_bain);
    roomsLeft.Add(ROOMS_CELL::cuisine);

    UE_LOG(LogTemp, Warning, TEXT("%d"), roomsLeft.Num());

    ROOMS_CELL currentRoom;
    int roomsLeftNum = roomsLeft.Num();
    int xOffset = 0;
    int yOffset = 0;
    int xRoomSize;
    int yRoomSize;
    int yTotalSize = 0;
    int xTotalSize = 0;
    for (int i = 0; i < roomsLeftNum; i++) {
        currentRoom = roomsLeft[FMath::RandRange(0, roomsLeft.Num()-2)];
        roomsLeft.Remove(currentRoom);
        UE_LOG(LogTemp, Warning, TEXT("%d"), roomsLeft.Num());
        switch (i) {
        case 0:xOffset = 0; yOffset = 0; break;
        case 1:yTotalSize += yRoomSize; xOffset = 0; yOffset = yRoomSize; break;
        case 2:xTotalSize += xRoomSize; yTotalSize += yRoomSize; xOffset = xRoomSize; yOffset = 0; break;
        }
        int minRoomSize = roomsSizes[currentRoom].X;
        int maxRoomSize = roomsSizes[currentRoom].Y;
        xRoomSize = FMath::RandRange(minRoomSize, maxRoomSize);
        yRoomSize = FMath::RandRange(minRoomSize, maxRoomSize);
        if (i == 2) { yOffset = yTotalSize - yRoomSize; xTotalSize += xRoomSize; }

        for (int j = 0; j < xRoomSize; j++) { //commencer à gen prochaine salle avec offset 1ère salle, offset que par y au 1er puis par x 2ème (+ offset 1er - maxSize) -- done
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
    }

    //walls salon
    for (int i = 0; i < xTotalSize+2; i++) {
        for (int j = 0; j < yTotalSize; j++) {
            UE_LOG(LogTemp, Warning, TEXT("%d,%d"), i+1, j+1);
            if (i == xTotalSize + 1 || wallsSouthArray[i+1][j] == WALL_LOCATION::south_wall) {
                wallsNorthArray[i][j] = WALL_LOCATION::north_wall;
            }
            if (j == 0 || wallsEastArray[i][FMath::Clamp(j,1,yTotalSize)-1] == WALL_LOCATION::east_wall) {
                wallsWestArray[i][j] = WALL_LOCATION::west_wall;
            }
            if (j == yTotalSize-1 || wallsWestArray[i][j + 1] == WALL_LOCATION::west_wall) {
                wallsEastArray[i][j] = WALL_LOCATION::east_wall;
            }
            if (wallsNorthArray[FMath::Clamp(i, 1, xTotalSize + 2) - 1][j] == WALL_LOCATION::north_wall) {
                wallsSouthArray[i][j] = WALL_LOCATION::south_wall;
            }
        }
    }
}

void AHouseGen::GenWalls()
{
    for (int i = 0; i < wallsNorthArray.Num(); i++) {
        for (int j = 0; j < wallsNorthArray.Num(); j++) {
            if (wallsNorthArray[i][j] != WALL_LOCATION::no_wall) {
                SpawnAWall(FVector(GetActorLocation().X + i * gridOffsetMultiplier, GetActorLocation().Y + j*gridOffsetMultiplier, GetActorLocation().Z), WALL_TYPE::chambre_flat, wallsNorthArray[i][j]);
            }
        }
    }
    for (int i = 0; i < wallsEastArray.Num(); i++) {
        for (int j = 0; j < wallsEastArray.Num(); j++) {
            if (wallsEastArray[i][j] != WALL_LOCATION::no_wall) {
                SpawnAWall(FVector(GetActorLocation().X + i * gridOffsetMultiplier, GetActorLocation().Y + j*gridOffsetMultiplier, GetActorLocation().Z), WALL_TYPE::chambre_flat, wallsEastArray[i][j]);
            }
        }
    }
    for (int i = 0; i < wallsSouthArray.Num(); i++) {
        for (int j = 0; j < wallsSouthArray.Num(); j++) {
            if (wallsSouthArray[i][j] != WALL_LOCATION::no_wall) {
                SpawnAWall(FVector(GetActorLocation().X + i * gridOffsetMultiplier, GetActorLocation().Y + j*gridOffsetMultiplier, GetActorLocation().Z), WALL_TYPE::chambre_flat, wallsSouthArray[i][j]);
            }
        }
    }
    for (int i = 0; i < wallsWestArray.Num(); i++) {
        for (int j = 0; j < wallsWestArray.Num(); j++) {
            if (wallsWestArray[i][j] != WALL_LOCATION::no_wall) {
                SpawnAWall(FVector(GetActorLocation().X + i * gridOffsetMultiplier, GetActorLocation().Y + j*gridOffsetMultiplier, GetActorLocation().Z), WALL_TYPE::chambre_flat, wallsWestArray[i][j]);
            }
        }
    }
}

void AHouseGen::SpawnAWall(FVector pos, WALL_TYPE wallType, WALL_LOCATION wallLocation)
{
    FRotator rot;
    FActorSpawnParameters spawnParams;
    //spawnParams.Owner = this;

    AWallsScript* mySpawn = GetWorld()->SpawnActor<AWallsScript>(bpWallsClass, pos, rot, spawnParams);
    mySpawn->SetWallLocationAndType(wallType, wallLocation);
    mySpawn->SetStaticMesh();
    mySpawn->RotateStaticMeshToLocation();
    spawnedWallsArray.Add(mySpawn);
}

void AHouseGen::GenerateAll()
{
    DestroyWalls();
    FillRoomsArray();
    GenWalls();
}

void AHouseGen::DestroyWalls()
{
    AWallsScript* currentWall;
    if (spawnedWallsArray.Num() <= 0) { return; }
    for (int i = 0; i < spawnedWallsArray.Num();i++) {
        if (spawnedWallsArray[i] != nullptr) {
            UE_LOG(LogTemp, Warning, TEXT("remove"));
            currentWall = spawnedWallsArray[i];
            spawnedWallsArray[i]=nullptr;
            currentWall->Destroy();
        }
    }
    spawnedWallsArray = TArray<AWallsScript*>();
}


