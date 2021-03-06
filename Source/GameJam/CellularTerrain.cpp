// Fill out your copyright notice in the Description page of Project Settings.

#include "CellularTerrain.h"

int32 ACellularTerrain::GetIndex(int32 x, int32 y)
{
	return (y * GridSize) + x;
}

int32 ACellularTerrain::GetX(int32 x)
{
	return x - ((x / GridSize) * GridSize);
}

int32 ACellularTerrain::GetY(int32 x)
{
	return x / GridSize;
}

int32 ACellularTerrain::CountAliveNeighbors(int32 x, int32 y)
{
	int32 count = 0;
	for (int32 i = -1; i < 2; i++)
	{
		for (int32 j = -1; j < 2; j++)
		{
			int32 newX = x + i;
			int32 newY = y + j;

			// Identity Check
			if (i == 0 && j == 0) {}
			else if (newX < 0 || newY < 0 || newX >= GridSize || newY >= GridSize)
			{
				// Out of Bounds Check
				// Makes it harder to randomly have an edge cave
				count += 2;
			}
			else if (TileGrid[GetIndex(newX, newY)])
			{
				// Standard Case
				count += 1;
			}
		}
	}

	return count;
}

// Fills TileGrid and FeatherLocations
void ACellularTerrain::generate()
{
	// Generating initial random grid to grow on
	for (int32 i = 0; i < GridSize; i++)
	{
		for (int32 j = 0; j < GridSize; j++)
		{
			if (FMath::FRandRange(0, 1) < SpawnAliveRate)
			{
				TileGrid[GetIndex(i, j)] = 1;
			}
			else
			{
				TileGrid[GetIndex(i, j)] = 0;
			}
		}
	}

	// Growing the caves using cellular automata
	for (int32 i = 0; i < TotalSteps; i++)
	{
		// Don't want to overwrite grid as we're calculating
		TArray<int32> NewTileGrid;
		NewTileGrid.Init(0, GridSize * GridSize);

		for (int32 x = 0; x < GridSize; x++)
		{
			for (int32 y = 0; y < GridSize; y++)
			{
				int32 neighbors = CountAliveNeighbors(x, y);

				// If cell is alive but too lonely, it dies
				if (TileGrid[GetIndex(x, y)] == 1)
				{
					if (neighbors < DeathLimit)
					{
						NewTileGrid[GetIndex(x, y)] = 0;
					}
					else
					{
						NewTileGrid[GetIndex(x, y)] = 1;
					}
				}
				// If cell is dead but has friends, it lives
				else if (TileGrid[GetIndex(x, y)] == 0)
				{
					if (neighbors > BirthLimit)
					{
						NewTileGrid[GetIndex(x, y)] = 1;
					}
					else
					{
						NewTileGrid[GetIndex(x, y)] = 0;
					}
				}
			}
		}

		for (int32 k = 0; k < GridSize * GridSize; k++)
		{
			TileGrid[k] = NewTileGrid[k];
		}
	}

	// Clear Out a space in the center
	for (int32 ii = -5; ii < 5; ii++)
	{
		for (int32 jj = -5; jj < 5; jj++)
		{
			TileGrid[GetIndex(ii + GridSize / 2, jj + GridSize / 2)] = 0;
		}
	}

	// Time to add feathers to the map
	FeatherLocations.Empty();
	while (FeatherLocations.Num() < 10)
	{
		int32 random = FMath::RandRange(0, GridSize * GridSize - 1);
		if (TileGrid[random] == 0)
		{
			FeatherLocations.AddUnique(random);
		}
	}

	// Clearing Out All Inside Rocks (Only Border)
	TArray<int32> FinalTileGrid;
	FinalTileGrid.Init(0, GridSize * GridSize);

	for (int32 xx = 0; xx < GridSize; xx++)
	{
		for (int32 yy = 0; yy < GridSize; yy++)
		{
			if (CountAliveNeighbors(xx, yy) == 8)
			{
				FinalTileGrid[GetIndex(xx, yy)] = 1;
			}
		}
	}

	for (int32 k = 0; k < GridSize * GridSize; k++)
	{
		if (TileGrid[k] == 1 && FinalTileGrid[k] == 1)
		{
			TileGrid[k] = FinalTileGrid[k];
		}
	}
}

// Sets default values
ACellularTerrain::ACellularTerrain()
{
	PrimaryActorTick.bCanEverTick = true;

	int32 seed = (int32)(FDateTime::Now().GetTicks() % INT_MAX);
	FMath::RandInit(seed);

	GridSize = 50;
	BirthLimit = 4;
	DeathLimit = 4;
	SpawnAliveRate = 0.47f;
	TotalSteps = 8;
	TileGrid.Init(0, GridSize * GridSize);

	generate();
}

// Called when the game starts or when spawned
void ACellularTerrain::BeginPlay()
{
	Super::BeginPlay();

	int32 seed = (int32)(FDateTime::Now().GetTicks() % INT_MAX);
	FMath::RandInit(seed);

	generate();
}

// Called every frame
void ACellularTerrain::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

#if WITH_EDITOR
void ACellularTerrain::PostEditChangeProperty(FPropertyChangedEvent &PropertyChangedEvent)
{
	generate();

	Super::PostEditChangeProperty(PropertyChangedEvent);
}
#endif
