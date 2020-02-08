// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Containers/Array.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CellularTerrain.generated.h"

UCLASS()
class GAMEJAM_API ACellularTerrain : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACellularTerrain();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cave Gen")
	int32 GridSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cave Gen")
	TArray<int32> TileGrid;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cave Gen")
	TArray<int32> FeatherLocations;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cave Gen")
	int32 BirthLimit;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cave Gen")
	int32 DeathLimit;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cave Gen")
	float SpawnAliveRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cave Gen")
	int32 TotalSteps;

	int32 CountAliveNeighbors(int32 x, int32 y);

	UFUNCTION(BlueprintCallable)
	int32 GetY(int32 x);

	UFUNCTION(BlueprintCallable)
	int32 GetX(int32 x);

	UFUNCTION(BlueprintCallable)
	int32 GetIndex(int32 x, int32 y);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void generate();
	void PostEditChangeProperty(FPropertyChangedEvent &PropertyChangedEvent);
};
