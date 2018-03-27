// Fill out your copyright notice in the Description page of Project Settings.
#include "Components/HierarchicalInstancedStaticMeshComponent.h"
#include "Classes/Kismet/KismetMathLibrary.h"
#include "Components/SphereComponent.h"
#include "MinesField.h"


// Sets default values
AMinesField::AMinesField()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	hism = CreateDefaultSubobject<UHierarchicalInstancedStaticMeshComponent>(TEXT("AFieldHISM")); // crear hism
	SetRootComponent(hism); // "setejar" hism com a root

	sphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision")); // crear esfera de col·lisio
	sphereComponent->SetupAttachment(hism); // fer l'esfera filla de hism
	sphereComponent->SetSphereRadius(campExtend); // assignar radi
	sphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AMinesField::BeginOverlap); // subscriure beginoverlap a beginOverlap del component
	
}


void AMinesField::BeginOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	//if (true) // si el jugador entra dins l'esfera
	//{
	//
	//}
}

// Called when the game starts or when spawned
void AMinesField::BeginPlay()
{
	Super::BeginPlay();

	FRandomStream srand;
	srand.GenerateNewSeed();

	for (int i = 0; i < numberOfMines; i++)
	{
		FVector Scale = (FVector(1.f))* mineScale;
		FVector Location = srand.GetUnitVector()*srand.FRandRange(0.f, campExtend);
		FRotator Rotation = UKismetMathLibrary::RandomRotator(); 
		int32 index = hism->AddInstance(FTransform(Rotation, Location, Scale));
		MinesTracker.Add(FMine(index));
		
	}
}

// Called every frame
void AMinesField::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

