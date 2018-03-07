// Fill out your copyright notice in the Description page of Project Settings.

#include "AsteroidsField.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"
#include "Classes/Kismet/KismetMathLibrary.h"
#include "MyDefs.h"

// Sets default values
AAsteroidsField::AAsteroidsField()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	hism = CreateDefaultSubobject<UHierarchicalInstancedStaticMeshComponent>(TEXT("AsteroidsHISM"));
	SetRootComponent(hism);
}

void AAsteroidsField::ItemTakeDamage(float DamageAmount, int32 index)
{
	FAsteroid * a = &AsteroidsTracker.GetData()[index];
	MYLOG(FString::FormatAsNumber(a->TakeDamage(DamageAmount)))
	if (a->Health < 0.f)
	{
		FTransform t;
		hism->GetInstanceTransform(index, t, true);

		//Destruir asteroide
		hism->RemoveInstance(index);
		if (index < AsteroidsTracker.Num() - 1)
		{
			AsteroidsTracker.GetData()[index] = AsteroidsTracker.Last();
		}
		AsteroidsTracker.Pop();

		hism->MarkRenderStateDirty();

		//Damos la posibilidad de añadir efectos a nivel de blueprint
		DestroyEffect(t.GetLocation());
	}

}

// Called when the game starts or when spawned
void AAsteroidsField::BeginPlay()
{
	Super::BeginPlay();
	
	FRandomStream srand;
	srand.GenerateNewSeed();

	for (int i = 0; i < NumberOfBigAsteroids; i++)
	{

		FVector Scale = (srand.GetUnitVector() + FVector(1.f))*0.5f*ScaleMaxBigAsteroids;
		FVector Location = srand.GetUnitVector()*srand.FRandRange(0.f, FieldRadius);
		FRotator Rotation = UKismetMathLibrary::RandomRotator();
		int32 index = hism->AddInstance(FTransform(Rotation, Location, Scale));
		AsteroidsTracker.Add(FAsteroid(index, Scale.Size()*10.f));
	}
	for (int i = 0; i < NumberOfLittleAsteroids; i++)
	{

		FVector Scale = (srand.GetUnitVector() + FVector(1.f))*0.5f*ScaleMaxLittleAsteroids;
		FVector Location = srand.GetUnitVector()*srand.FRandRange(0.f, FieldRadius);
		FRotator Rotation = UKismetMathLibrary::RandomRotator();
		int32 index = hism->AddInstance(FTransform(Rotation, Location, Scale));
		AsteroidsTracker.Add(FAsteroid(index, Scale.Size()*10.f));
		//MYLOG(FString::FormatAsNumber(Scale.Size()*100.f))
	}
}

// Called every frame
void AAsteroidsField::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

