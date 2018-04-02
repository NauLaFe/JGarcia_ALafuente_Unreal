// Fill out your copyright notice in the Description page of Project Settings.
#include "Components/HierarchicalInstancedStaticMeshComponent.h"
#include "Classes/Kismet/KismetMathLibrary.h"
#include "Components/SphereComponent.h"
#include "MinesField.h"
#include "MyDefs.h"
#include "Pawn/FalconPlayer.h"
#include "AsteroidsField/AsteroidsField.h"
#include "Classes/Kismet/GameplayStatics.h"

// Sets default values
AMinesField::AMinesField()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	hism = CreateDefaultSubobject<UHierarchicalInstancedStaticMeshComponent>(TEXT("AFieldHISM")); // crear hism
	SetRootComponent(hism); // "setejar" hism com a root

	sphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision")); // crear esfera de col·lisio
	sphereComponent->SetupAttachment(hism); // fer l'esfera filla de hism
	sphereComponent->bGenerateOverlapEvents = true;
}


void AMinesField::BeginOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (!Cast<AFalconPlayer>(OtherActor)) return;
	
	pursue = true;
	MYLOG("Player inside collision")
}
void AMinesField::EndOverlap (UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!Cast<AFalconPlayer>(OtherActor)) return;

	pursue = false;
	MYLOG("Player outside collision")
}


// Called when the game starts or when spawned
void AMinesField::BeginPlay()
{
	Super::BeginPlay();

	// al constructor no funciona, segons un forum d'internet si el poses al beeginplay funciona, i aixi a resultat ser ¯\_(")_/¯ 
	sphereComponent->SetSphereRadius(campExtend); // assignar radi
	sphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AMinesField::BeginOverlap); // subscriure beginoverlap a beginOverlap del component
	sphereComponent->OnComponentEndOverlap.AddDynamic(this, &AMinesField::EndOverlap);	// subscriure endoverlap a endOverlap del component

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

	for (int i = 0; i < numberOfMines; i++)
	{
		PursuePlayer(i);
		MineHit(i);
	}
	
}

void AMinesField::MineHit(int idx)
{
	FTransform t;
	hism->GetInstanceTransform(idx, t, true);
	FVector position = t.GetLocation();

	// HIT
	FHitResult Hit;
	FVector Start = position;
	FVector End = position + FVector(10, 10, 10);

	FCollisionQueryParams qp;
	qp.AddIgnoredActor(this);
	qp.bTraceComplex = false;

	qp.bReturnPhysicalMaterial = false;

	if (!GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_WorldStatic, qp, FCollisionResponseParams()))
		return;

	// PLAYER HIT
	AFalconPlayer* player = Cast<AFalconPlayer>(Hit.GetActor());
	if (player)
	{
		// DESTROY
		hism->RemoveInstance(idx);
		FDamageEvent de;

		Hit.GetActor()->TakeDamage(damage, de, UGameplayStatics::GetPlayerController(GetWorld(), 0), this);

		if (ParticleSystemTemplate != nullptr)
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ParticleSystemTemplate, FTransform(GetActorRotation(), Start, 10*FVector(1.f, 1.f, 1.f)));
	}

	// ASTEROID HIT
	AAsteroidsField *asteroids = Cast<AAsteroidsField>(Hit.GetActor());
	if (asteroids)
	{
		UPrimitiveComponent* CurrentTargetComponent = Hit.GetComponent();

		if (CurrentTargetComponent)
		{
			hism->RemoveInstance(idx);
			asteroids->ItemTakeDamage(damage, Hit.Item);
			if (ParticleSystemTemplate != nullptr)
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ParticleSystemTemplate, FTransform(GetActorRotation(), Start, 10 * FVector(1.f, 1.f, 1.f)));

		}
	}
}

void AMinesField::PursuePlayer(int idx)
{
	if (!pursue) return;

	FVector playerPosition = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();

	FTransform t;
	hism->GetInstanceTransform(idx, t, true);
	FVector currentPos = t.GetLocation();

	FQuat currentRot = t.GetRotation();
	FVector currentScale = t.GetScale3D();

	FVector moveDirection = playerPosition - currentPos;
	moveDirection.Normalize();

	FVector newPos = currentPos + moveDirection * speed;
	FTransform newTransfrom = FTransform(currentRot, newPos, currentScale);
	hism->UpdateInstanceTransform(idx, newTransfrom, true);
}





