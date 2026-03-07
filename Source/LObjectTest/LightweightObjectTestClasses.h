// LightweightObject Test Classes

#pragma once

#include "CoreMinimal.h"
#include "LightweightObject.h"
#include "LightweightObjectTestClasses.generated.h"

/** Simple test class with basic properties */
UCLASS(Blueprintable)
class UTestLightweightObjectSimple : public ULightweightObjectBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Health = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Mana = 50.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Name = TEXT("DefaultName");
};

/** Complex test class with container and struct properties */
UCLASS(Blueprintable)
class UTestLightweightObjectComplex : public ULightweightObjectBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector Position = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRotator Rotation = FRotator::ZeroRotator;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<int32> Scores;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsActive = true;
};

/** Derived test class for inheritance tests */
UCLASS(Blueprintable)
class UTestLightweightObjectDerived : public UTestLightweightObjectSimple
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Stamina = 75;
};

/** Empty test class for edge case tests */
UCLASS(Blueprintable)
class UTestLightweightObjectEmpty : public ULightweightObjectBase
{
	GENERATED_BODY()
};

/** Test empty class renamed for clarity */
UCLASS(Blueprintable)
class UTestLightweightObjectEmptyClass : public ULightweightObjectBase
{
	GENERATED_BODY()
};

/** Spec test object */
UCLASS(Blueprintable)
class USpecTestObject : public ULightweightObjectBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Value = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Label = TEXT("Default");

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Ratio = 1.0f;
};

/** Spec test child class */
UCLASS(Blueprintable)
class USpecTestObjectChild : public USpecTestObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ChildValue = 100;
};
