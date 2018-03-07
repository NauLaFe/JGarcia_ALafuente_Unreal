// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "TargetFinderComponent.generated.h"

UENUM(BlueprintType)
enum class ETargetFinderMode : uint8
{
	Inactive UMETA(DisplayName="No action"), TargetSweeping UMETA(DisplayName ="Sweeping of several Targets"), SingleTarget UMETA("Single Target"), CameraSlave UMETA("Follow Player Camera")
};


USTRUCT(BlueprintType)
struct FTargetArea
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float Radius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector Direction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Offset;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnTargetFixed, AActor*, OtherActor, UPrimitiveComponent*, OtherComponent);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FALCONANDOWLST_API UTargetFinderComponent : public USceneComponent
{
	GENERATED_BODY()

public:	

	// Sets default values for this component's properties
	UTargetFinderComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Working Mode")
	ETargetFinderMode WorkingMode = ETargetFinderMode::Inactive;
	UPROPERTY(EditDefaultsonly, BlueprintReadOnly, Category ="Working Mode")
	TSet<TSubclassOf<AActor>> TargetTypes;
	UPROPERTY(EditDefaultsonly, BlueprintReadOnly, Category = "Working Mode")
	float TimeInTarget;
	UPROPERTY(EditDefaultsonly, BlueprintReadOnly, Category = "Working Mode")
	float CameraScope;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Working Mode")
	bool bIsWorking = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Managing Target Finder")
	FTargetArea TargetArea;
	UFUNCTION(BlueprintCallable, Category="Managing Target Finder")
	void Start();
	UFUNCTION(BlueprintCallable, Category = "Managing Target Finder")
	void Stop();
	UPROPERTY(BlueprintReadOnly, Category = "Target Result")
	AActor * CurrentTarget = nullptr;
	UPROPERTY(BlueprintReadOnly, Category = "Target Result")
	UPrimitiveComponent * CurrentTargetComponent = nullptr;
	UPROPERTY(BlueprintReadOnly, Category = "Target Result")
	float DistanceWhenCameraSlave = 0.f;
	UPROPERTY(BlueprintReadOnly, Category = "Target Result")
	FString TargetTypeWhenCameraSlave;
	UPROPERTY(BlueprintReadOnly, Category = "Target Result")
	FVector currentTargetLocation;

	UPROPERTY(BlueprintAssignable)
	FOnTargetFixed OnTargetFixed;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void FindTargets();
	FHitResult FindPlayerCameraHit();
	class APlayerCameraManager * pcm;
	FTimerHandle TimerHandle;
	bool IsTargetFixedNotified;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
	
};
