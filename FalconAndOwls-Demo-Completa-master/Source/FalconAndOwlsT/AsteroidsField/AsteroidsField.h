// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AsteroidsField.generated.h"

USTRUCT(BlueprintType)
struct FAsteroid
{
	GENERATED_BODY()

public:
	UPROPERTY()
	int32 index;

	UPROPERTY()
	float Health;

	FAsteroid()
	{
	}

	FAsteroid(int Index, float Health)
	{
		index = Index;
		Health = Health * 100;
	}

	float TakeDamage(float Damage)
	{
		return (Health -= Damage);
	}
};

UCLASS()
class FALCONANDOWLST_API AAsteroidsField : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAsteroidsField();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UHierarchicalInstancedStaticMeshComponent *hism;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Field Size")
	float NumberOfLittleAsteroids;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Field Size")
	float NumberOfBigAsteroids;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Field Size")
	float FieldRadius;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Asteroids configuration")
	float ScaleMaxLittleAsteroids;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Asteroids configuration")
	float ScaleMaxBigAsteroids;

	void ItemTakeDamage
	(
		float DamageAmount,
		int32 index
	);

	UFUNCTION(BlueprintImplementableEvent, Category="Destroy event")
	void DestroyEffect(FVector Location);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	TArray<FAsteroid> AsteroidsTracker;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
