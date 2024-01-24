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
    TArray<WALL_TYPE> emptyWallsTypesRow;
    emptyRow.Init(ROOMS_CELL::vide, arraysSize);
    emptyWallsRow.Init(WALL_LOCATION::no_wall, arraysSize);
    emptyWallsTypesRow.Init(WALL_TYPE::no_type, arraysSize);
    roomsArray.Init(emptyRow, arraysSize);
    wallsNorthArray.Init(emptyWallsRow, arraysSize);
    wallsEastArray.Init(emptyWallsRow, arraysSize);
    wallsSouthArray.Init(emptyWallsRow, arraysSize);
    wallsWestArray.Init(emptyWallsRow, arraysSize);
    wallsTypesNorthArray.Init(emptyWallsTypesRow, arraysSize);
    wallsTypesEastArray.Init(emptyWallsTypesRow, arraysSize);
    wallsTypesSouthArray.Init(emptyWallsTypesRow, arraysSize);
    wallsTypesWestArray.Init(emptyWallsTypesRow, arraysSize);


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
        case 1:yRoomOneSize = yRoomSize; xRoomOneSize = xRoomSize; yTotalSize += yRoomSize; xOffset = 0; yOffset = yRoomSize; break;
        case 2:xTotalSize += xRoomSize; yTotalSize += yRoomSize; xOffset = xRoomSize; yOffset = 0; xRoomTwoSize = xRoomSize; break;
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
                        wallsTypesWestArray[j + xOffset][k + yOffset] = RoomToWallType(currentRoom);
                    }
                    if (j == 0) {
                        wallsSouthArray[j + xOffset][k + yOffset] = WALL_LOCATION::south_wall;
                        wallsTypesSouthArray[j + xOffset][k + yOffset] = RoomToWallType(currentRoom);
                    }
                    if (k == yRoomSize-1) {
                        wallsEastArray[j + xOffset][k + yOffset] = WALL_LOCATION::east_wall;
                        wallsTypesEastArray[j + xOffset][k + yOffset] = RoomToWallType(currentRoom);
                    }
                    if (j == xRoomSize-1) {
                        wallsNorthArray[j + xOffset][k + yOffset] = WALL_LOCATION::north_wall;
                        wallsTypesNorthArray[j + xOffset][k + yOffset] = RoomToWallType(currentRoom);
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
                wallsTypesNorthArray[i][j] = salon_flat;
            }
            if (j == 0 || wallsEastArray[i][FMath::Clamp(j,1,yTotalSize)-1] == WALL_LOCATION::east_wall) {
                wallsWestArray[i][j] = WALL_LOCATION::west_wall;
                wallsTypesWestArray[i][j] = salon_flat;
            }
            if (j == yTotalSize-1 || wallsWestArray[i][j + 1] == WALL_LOCATION::west_wall) {
                wallsEastArray[i][j] = WALL_LOCATION::east_wall;
                wallsTypesEastArray[i][j] = salon_flat;
            }
            if (wallsNorthArray[FMath::Clamp(i, 1, xTotalSize + 2) - 1][j] == WALL_LOCATION::north_wall) {
                wallsSouthArray[i][j] = WALL_LOCATION::south_wall;
                wallsTypesSouthArray[i][j] = salon_flat;
            }
        }
    }

    xHouseSize = xTotalSize + 2;
    yHouseSize = yTotalSize;
}

void AHouseGen::GenWalls()
{
    for (int i = 0; i < wallsNorthArray.Num(); i++) {
        for (int j = 0; j < wallsNorthArray.Num(); j++) {
            if (wallsNorthArray[i][j] != WALL_LOCATION::no_wall) {
                SpawnAWall(FVector(GetActorLocation().X + i * gridOffsetMultiplier, GetActorLocation().Y + j*gridOffsetMultiplier, GetActorLocation().Z), wallsTypesNorthArray[i][j], wallsNorthArray[i][j]);
            }
        }
    }
    for (int i = 0; i < wallsEastArray.Num(); i++) {
        for (int j = 0; j < wallsEastArray.Num(); j++) {
            if (wallsEastArray[i][j] != WALL_LOCATION::no_wall) {
                SpawnAWall(FVector(GetActorLocation().X + i * gridOffsetMultiplier, GetActorLocation().Y + j*gridOffsetMultiplier, GetActorLocation().Z), wallsTypesEastArray[i][j], wallsEastArray[i][j]);
            }
        }
    }
    for (int i = 0; i < wallsSouthArray.Num(); i++) {
        for (int j = 0; j < wallsSouthArray.Num(); j++) {
            if (wallsSouthArray[i][j] != WALL_LOCATION::no_wall) {
                SpawnAWall(FVector(GetActorLocation().X + i * gridOffsetMultiplier, GetActorLocation().Y + j*gridOffsetMultiplier, GetActorLocation().Z), wallsTypesSouthArray[i][j], wallsSouthArray[i][j]);
            }
        }
    }
    for (int i = 0; i < wallsWestArray.Num(); i++) {
        for (int j = 0; j < wallsWestArray.Num(); j++) {
            if (wallsWestArray[i][j] != WALL_LOCATION::no_wall) {
                SpawnAWall(FVector(GetActorLocation().X + i * gridOffsetMultiplier, GetActorLocation().Y + j*gridOffsetMultiplier, GetActorLocation().Z), wallsTypesWestArray[i][j], wallsWestArray[i][j]);
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
    CheckForDoors();
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

void AHouseGen::CheckForDoors()
{
    for (int i = 0; i < roomsArray.Num(); i++) {
        for (int j = 0; j < roomsArray[0].Num(); j++) {
            if (wallsEastArray[i][j] != no_wall && wallsWestArray[i][j] != no_wall && wallsSouthArray[i][j] != no_wall) {
                wallsTypesEastArray[i][j] = ConvertToDoor(wallsTypesEastArray[i][j]);
                wallsTypesEastArray[i][j-1] = ConvertToDoor(wallsTypesEastArray[i][j]);
                wallsTypesWestArray[i][j] = ConvertToDoor(wallsTypesWestArray[i][j]);
                wallsTypesWestArray[i][j+1] = ConvertToDoor(wallsTypesWestArray[i][j]);
                wallsTypesSouthArray[i][j] = ConvertToDoor(wallsTypesSouthArray[i][j]);
                wallsTypesNorthArray[i-1][j] = ConvertToDoor(wallsTypesNorthArray[i][j]);
                UE_LOG(LogTemp, Warning, TEXT("DOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOR"));
                return;
            }
        }
    }

    UE_LOG(LogTemp, Error, TEXT("%d,0"), xRoomOneSize);
    wallsTypesNorthArray[xRoomOneSize-1][0] = ConvertToDoor(wallsTypesNorthArray[xRoomOneSize-1][0]);
    wallsTypesSouthArray[xRoomOneSize][0] = ConvertToDoor(wallsTypesSouthArray[xRoomOneSize][0]);
    wallsTypesWestArray[0][yRoomOneSize] = ConvertToDoor(wallsTypesWestArray[0][yRoomOneSize]);
    wallsTypesEastArray[0][yRoomOneSize-1] = ConvertToDoor(wallsTypesEastArray[0][yRoomOneSize-1]);
    wallsTypesNorthArray[xRoomTwoSize-1][yHouseSize-1] = ConvertToDoor(wallsTypesNorthArray[xRoomTwoSize - 1][yHouseSize - 1]);
    wallsTypesSouthArray[xRoomTwoSize][yHouseSize - 1] = ConvertToDoor(wallsTypesSouthArray[xRoomTwoSize][yHouseSize - 1]);
    wallsTypesNorthArray[xHouseSize-3][yHouseSize-1] = ConvertToDoor(wallsTypesNorthArray[xHouseSize-3][yHouseSize - 1]);
    wallsTypesSouthArray[xHouseSize - 2][yHouseSize - 1] = ConvertToDoor(wallsTypesSouthArray[xHouseSize - 2][yHouseSize - 1]);
}

WALL_TYPE AHouseGen::RoomToWallType(ROOMS_CELL room)
{
    switch (room) {
    case cuisine: return cuisine_flat;
    case salle_de_bain: return salle_de_bain_flat;
    case salon: return salon_flat;
    case chambre: return chambre_flat;
    }
    return no_type;
}

WALL_TYPE AHouseGen::RoomToDoorType(ROOMS_CELL room)
{
    switch (room) {
    case cuisine: return cuisine_door;
    case salle_de_bain: return salle_de_bain_door;
    case salon: return salon_door;
    case chambre: return chambre_door;
    }
    return no_type;
}

WALL_TYPE AHouseGen::ConvertToDoor(WALL_TYPE wallType)
{
    switch (wallType) {
    case cuisine_flat: return cuisine_door;
    case salle_de_bain_flat: return salle_de_bain_door;
    case salon_flat: return salon_door;
    case chambre_flat: return chambre_door;
    }
    return chambre_door; //temp fix
}


// ----------------------------------------------------------------------------------------------------------------------------------
// 
// 
// pour les meubles -> check if mur valide pour amoires/frigo/... -> check toutes les arrays de murs -> si 1 seul mur sur la cell alors emplacement valide
//
// possiblité de tester meubles en angle pour cuisine -> si murs sur 2 layers même cell alors spawn modé spéciale angle (rotate en fonction de orientation mur) 
// 
// pour grands objets (ex: tapis) -> check si celle autour vides pour pouvoir le placer, ex check cases en x,y / x+1,y / x,y+1 / x+1,y+1 -> si toutes vides alors emplacement tapis valide
// 
// penesr à save location milieu pièces pour lampes au plafond ? (sinon check 4 cases autour (pas diagonales) si y'a pas de mur, ni déjà de lampe, peut avoir 2 lampes dans salon
// 
// script différent pour spawn les petits objets directement sur les meubles au OnBegin
// 
// paramètre pour permettre de spawn un objet rare, si déjà présent alors ne peut pas spawn objet rare
// 
// pick objet dans pool différents level
// 
// 
//


