// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StructUtils/PropertyBag.h"
#include "UObject/Object.h"
#include "LightweightObject.generated.h"

class ULightweightObjectBase;

USTRUCT(BlueprintType)
struct FLightweightObject
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowAbstract = "false"))
	TSubclassOf<ULightweightObjectBase> Class;
	
	UPROPERTY(EditAnywhere)
	FInstancedPropertyBag Data;
};

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType, Abstract)
class LIGHTWEIGHTOBJECT_API ULightweightObjectBase : public UObject
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	FLightweightObject CreateLightweightObject() const;

	UFUNCTION(BlueprintCallable)
	bool PushLightweightObject(const FLightweightObject& InLightweightObject);
	
	UFUNCTION(BlueprintCallable)
	bool PopLightweightObject(FLightweightObject& OutLightweightObject);
	
	UFUNCTION(BlueprintCallable, CustomThunk, meta=(CustomStructureParam = "OutObject"))
	static void ConvertToULightweightObject(UPARAM(Ref) const FLightweightObject& InObject, int32& OutObject);
	
	UFUNCTION(BlueprintCallable)
	bool ConvertToFLightweightObject(FLightweightObject& OutLightweightObject);
	
	virtual void PostLoad() override;
	void MigrateData(FLightweightObject& InLightweightObject) const;
private:
	DECLARE_FUNCTION(execConvertToULightweightObject);	
	
	UPROPERTY(Transient)
	FInstancedPropertyBag DefaultData;
};
