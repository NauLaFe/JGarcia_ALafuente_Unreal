// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MinesField.generated.h"

USTRUCT(BlueprintType)
struct FMine
{
	GENERATED_BODY()

public:
	UPROPERTY()
		int32 index;

	FMine()
	{
	}

	FMine (int Index)
	{
		index = Index;
	}

};

UCLASS()
class FALCONANDOWLST_API AMinesField : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMinesField();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UHierarchicalInstancedStaticMeshComponent *hism;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class USphereComponent *sphereComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Field Properties")
	int32 numberOfMines = 20;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Field Properties")
	float campExtend = 5000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mine Properties")
	float damage = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mine Properties")
	float speed = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mine Properties")
	float mineScale = 100.0f;

	UPROPERTY(EditDefaultsonly)
	class UParticleSystem *ParticleSystemTemplate;

	UFUNCTION(BlueprintImplementableEvent, Category = "Destroy event")
	void DestroyEffect(FVector Location);

	UFUNCTION()
	void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);

	UFUNCTION()
	void EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:
	bool pursue = false;

	void MineHit(int idx);

	void PursuePlayer(int idx); 

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	TArray<FMine> MinesTracker;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
