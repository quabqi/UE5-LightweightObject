// LightweightObject Unit Tests
#include "LightweightObjectTest.h"
#include "Misc/AutomationTest.h"

#if WITH_DEV_AUTOMATION_TESTS

// Test 1: CreateLightweightObject
IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FLightweightObjectCreateTest,
	"LightweightObject.Core.CreateLightweightObject",
	EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::ProductFilter)

bool FLightweightObjectCreateTest::RunTest(const FString& Parameters)
{
	UTestLightweightObjectSimple* CDO = UTestLightweightObjectSimple::StaticClass()->GetDefaultObject<UTestLightweightObjectSimple>();
	TestNotNull(TEXT("CDO should exist"), CDO);

	CDO->PostLoad();

	FLightweightObject LightweightObj = CDO->CreateLightweightObject();

	TestEqual(TEXT("Class should match"), LightweightObj.Class.Get(), UTestLightweightObjectSimple::StaticClass());
	TestTrue(TEXT("Data should be valid"), LightweightObj.Data.IsValid());
	TestNotNull(TEXT("PropertyBag should exist"), LightweightObj.Data.GetPropertyBagStruct());

	return true;
}

// Test 2: Push/Pop
IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FLightweightObjectPushPopTest,
	"LightweightObject.Core.PushPop",
	EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::ProductFilter)

bool FLightweightObjectPushPopTest::RunTest(const FString& Parameters)
{
	UTestLightweightObjectSimple* CDO = UTestLightweightObjectSimple::StaticClass()->GetDefaultObject<UTestLightweightObjectSimple>();
	CDO->PostLoad();

	CDO->Health = 200;
	CDO->Mana = 100.0f;
	CDO->Name = TEXT("ModifiedName");

	// Create lightweight object and capture current values
	FLightweightObject LightweightObj;
	CDO->ConvertToFLightweightObject(LightweightObj);

	CDO->Health = 300;
	CDO->Mana = 150.0f;
	CDO->Name = TEXT("AnotherName");

	TestTrue(TEXT("Push should succeed"), CDO->PushLightweightObject(LightweightObj));
	TestEqual(TEXT("Health should be restored"), CDO->Health, 200);
	TestEqual(TEXT("Mana should be restored"), CDO->Mana, 100.0f);
	TestEqual(TEXT("Name should be restored"), CDO->Name, TEXT("ModifiedName"));

	CDO->Health = 400;
	CDO->Mana = 200.0f;

	TestTrue(TEXT("Pop should succeed"), CDO->PopLightweightObject(LightweightObj));

	CDO->Health = 0;
	CDO->PushLightweightObject(LightweightObj);
	TestEqual(TEXT("Health should be 400 after Pop->Push"), CDO->Health, 400);

	return true;
}

// Test 3: Type Mismatch
IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FLightweightObjectTypeMismatchTest,
	"LightweightObject.Core.TypeMismatch",
	EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::ProductFilter)

bool FLightweightObjectTypeMismatchTest::RunTest(const FString& Parameters)
{
	UTestLightweightObjectSimple* CDOSimple = UTestLightweightObjectSimple::StaticClass()->GetDefaultObject<UTestLightweightObjectSimple>();
	CDOSimple->PostLoad();

	UTestLightweightObjectComplex* CDOComplex = UTestLightweightObjectComplex::StaticClass()->GetDefaultObject<UTestLightweightObjectComplex>();
	CDOComplex->PostLoad();

	FLightweightObject SimpleObj = CDOSimple->CreateLightweightObject();

	TestFalse(TEXT("Push with wrong type should fail"), CDOComplex->PushLightweightObject(SimpleObj));
	TestFalse(TEXT("Pop with wrong type should fail"), CDOComplex->PopLightweightObject(SimpleObj));

	return true;
}

// Test 4: Empty Class
IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FLightweightObjectEmptyClassTest,
	"LightweightObject.Core.EmptyClass",
	EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::ProductFilter)

bool FLightweightObjectEmptyClassTest::RunTest(const FString& Parameters)
{
	UTestLightweightObjectEmpty* CDO = UTestLightweightObjectEmpty::StaticClass()->GetDefaultObject<UTestLightweightObjectEmpty>();
	CDO->PostLoad();

	FLightweightObject EmptyObj = CDO->CreateLightweightObject();

	TestEqual(TEXT("Class should match"), EmptyObj.Class.Get(), UTestLightweightObjectEmpty::StaticClass());
	TestTrue(TEXT("Push on empty object should succeed"), CDO->PushLightweightObject(EmptyObj));
	TestTrue(TEXT("Pop on empty object should succeed"), CDO->PopLightweightObject(EmptyObj));

	return true;
}

// Test 5: Inheritance
IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FLightweightObjectInheritanceTest,
	"LightweightObject.Core.Inheritance",
	EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::ProductFilter)

bool FLightweightObjectInheritanceTest::RunTest(const FString& Parameters)
{
	UTestLightweightObjectDerived* CDO = UTestLightweightObjectDerived::StaticClass()->GetDefaultObject<UTestLightweightObjectDerived>();
	CDO->PostLoad();

	CDO->Health = 150;
	CDO->Mana = 75.0f;
	CDO->Name = TEXT("Derived");
	CDO->Stamina = 50;

	// Create lightweight object and capture current values
	FLightweightObject DerivedObj;
	CDO->ConvertToFLightweightObject(DerivedObj);

	CDO->Health = 0;
	CDO->Stamina = 0;

	CDO->PushLightweightObject(DerivedObj);

	TestEqual(TEXT("Base class Health should be restored"), CDO->Health, 150);
	TestEqual(TEXT("Base class Mana should be restored"), CDO->Mana, 75.0f);
	TestEqual(TEXT("Base class Name should be restored"), CDO->Name, TEXT("Derived"));
	TestEqual(TEXT("Derived class Stamina should be restored"), CDO->Stamina, 50);

	return true;
}

// Test 6: Complex Properties
IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FLightweightObjectComplexPropertyTest,
	"LightweightObject.Core.ComplexProperties",
	EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::ProductFilter)

bool FLightweightObjectComplexPropertyTest::RunTest(const FString& Parameters)
{
	UTestLightweightObjectComplex* CDO = UTestLightweightObjectComplex::StaticClass()->GetDefaultObject<UTestLightweightObjectComplex>();
	CDO->PostLoad();

	CDO->Position = FVector(100.0, 200.0, 300.0);
	CDO->Rotation = FRotator(45.0, 90.0, 0.0);
	CDO->Scores = {10, 20, 30, 40, 50};
	CDO->bIsActive = false;

	FLightweightObject ComplexObj;
	CDO->ConvertToFLightweightObject(ComplexObj);

	CDO->Position = FVector::ZeroVector;
	CDO->Rotation = FRotator::ZeroRotator;
	CDO->Scores.Empty();
	CDO->bIsActive = true;

	TestTrue(TEXT("Push should succeed"), CDO->PushLightweightObject(ComplexObj));

	TestEqual(TEXT("Position X should match"), CDO->Position.X, 100.0);
	TestEqual(TEXT("Position Y should match"), CDO->Position.Y, 200.0);
	TestEqual(TEXT("Position Z should match"), CDO->Position.Z, 300.0);
	TestEqual(TEXT("Rotation Pitch should match"), CDO->Rotation.Pitch, 45.0);
	TestEqual(TEXT("Rotation Yaw should match"), CDO->Rotation.Yaw, 90.0);
	TestEqual(TEXT("Scores count should match"), CDO->Scores.Num(), 5);
	TestEqual(TEXT("Scores[0] should match"), CDO->Scores[0], 10);
	TestEqual(TEXT("Scores[4] should match"), CDO->Scores[4], 50);
	TestFalse(TEXT("bIsActive should be false"), CDO->bIsActive);

	return true;
}

// Test 7: ConvertToFLightweightObject
IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FLightweightObjectConvertToTest,
	"LightweightObject.Core.ConvertToFLightweightObject",
	EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::ProductFilter)

bool FLightweightObjectConvertToTest::RunTest(const FString& Parameters)
{
	UTestLightweightObjectSimple* CDO = UTestLightweightObjectSimple::StaticClass()->GetDefaultObject<UTestLightweightObjectSimple>();
	CDO->PostLoad();

	CDO->Health = 999;
	CDO->Mana = 888.0f;
	CDO->Name = TEXT("ConvertTest");

	FLightweightObject ConvertedObj;
	TestTrue(TEXT("ConvertToFLightweightObject should succeed"), CDO->ConvertToFLightweightObject(ConvertedObj));
	TestEqual(TEXT("Converted class should match"), ConvertedObj.Class.Get(), UTestLightweightObjectSimple::StaticClass());

	CDO->Health = 0;
	CDO->PushLightweightObject(ConvertedObj);
	TestEqual(TEXT("Health should be 999"), CDO->Health, 999);

	return true;
}

// Test 8: MigrateData
IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FLightweightObjectMigrateDataTest,
	"LightweightObject.Core.MigrateData",
	EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::ProductFilter)

bool FLightweightObjectMigrateDataTest::RunTest(const FString& Parameters)
{
	UTestLightweightObjectSimple* CDO = UTestLightweightObjectSimple::StaticClass()->GetDefaultObject<UTestLightweightObjectSimple>();
	CDO->PostLoad();

	FLightweightObject EmptyObj;
	EmptyObj.Class = UTestLightweightObjectSimple::StaticClass();

	CDO->MigrateData(EmptyObj);
	TestTrue(TEXT("After migration, data should be valid"), EmptyObj.Data.IsValid());

	return true;
}

// Test 9: Multiple Push/Pop
IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FLightweightObjectMultiplePushPopTest,
	"LightweightObject.Core.MultiplePushPop",
	EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::ProductFilter)

bool FLightweightObjectMultiplePushPopTest::RunTest(const FString& Parameters)
{
	UTestLightweightObjectSimple* CDO = UTestLightweightObjectSimple::StaticClass()->GetDefaultObject<UTestLightweightObjectSimple>();
	CDO->PostLoad();

	FLightweightObject Obj1 = CDO->CreateLightweightObject();
	FLightweightObject Obj2 = CDO->CreateLightweightObject();
	FLightweightObject Obj3 = CDO->CreateLightweightObject();

	CDO->Health = 100;
	CDO->PopLightweightObject(Obj1);

	CDO->Health = 200;
	CDO->PopLightweightObject(Obj2);

	CDO->Health = 300;
	CDO->PopLightweightObject(Obj3);

	CDO->PushLightweightObject(Obj1);
	TestEqual(TEXT("Obj1 Health should be 100"), CDO->Health, 100);

	CDO->PushLightweightObject(Obj2);
	TestEqual(TEXT("Obj2 Health should be 200"), CDO->Health, 200);

	CDO->PushLightweightObject(Obj3);
	TestEqual(TEXT("Obj3 Health should be 300"), CDO->Health, 300);

	CDO->PushLightweightObject(Obj1);
	TestEqual(TEXT("Obj1 Health should still be 100"), CDO->Health, 100);

	return true;
}

// Test 10: Boundary Values
IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FLightweightObjectBoundaryTest,
	"LightweightObject.Core.BoundaryValues",
	EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::ProductFilter)

bool FLightweightObjectBoundaryTest::RunTest(const FString& Parameters)
{
	UTestLightweightObjectSimple* CDO = UTestLightweightObjectSimple::StaticClass()->GetDefaultObject<UTestLightweightObjectSimple>();
	CDO->PostLoad();

	// Zero values - use ConvertToFLightweightObject to capture current state
	CDO->Health = 0;
	CDO->Mana = 0.0f;
	CDO->Name = TEXT("");

	FLightweightObject Obj;
	CDO->ConvertToFLightweightObject(Obj);

	CDO->Health = 100;
	CDO->Mana = 100.0f;
	CDO->Name = TEXT("NonEmpty");

	CDO->PushLightweightObject(Obj);

	TestEqual(TEXT("Health should be 0"), CDO->Health, 0);
	TestEqual(TEXT("Mana should be 0.0"), CDO->Mana, 0.0f);
	TestEqual(TEXT("Name should be empty"), CDO->Name, TEXT(""));

	// Max values
	CDO->Health = MAX_int32;
	CDO->Mana = MAX_flt;
	CDO->Name = FString::ChrN(1024, TEXT('A'));

	CDO->PopLightweightObject(Obj);

	CDO->Health = 0;
	CDO->PushLightweightObject(Obj);

	TestEqual(TEXT("Health should be MAX_int32"), CDO->Health, MAX_int32);
	TestEqual(TEXT("Mana should be MAX_flt"), CDO->Mana, MAX_flt);

	return true;
}

// Test 11: PropertyBag Validation
IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FLightweightObjectPropertyBagValidationTest,
	"LightweightObject.Core.PropertyBagValidation",
	EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::ProductFilter)

bool FLightweightObjectPropertyBagValidationTest::RunTest(const FString& Parameters)
{
	UTestLightweightObjectSimple* CDO = UTestLightweightObjectSimple::StaticClass()->GetDefaultObject<UTestLightweightObjectSimple>();
	CDO->PostLoad();

	FLightweightObject ValidObj = CDO->CreateLightweightObject();

	FLightweightObject DifferentObj;
	DifferentObj.Class = UTestLightweightObjectSimple::StaticClass();
	TArray<FPropertyBagPropertyDesc> Descs;
	Descs.Add(FPropertyBagPropertyDesc(TEXT("DifferentProperty"), EPropertyBagPropertyType::Int32));
	DifferentObj.Data.AddProperties(Descs);

	TestFalse(TEXT("Push with different PropertyBag structure should fail"), CDO->PushLightweightObject(DifferentObj));
	TestFalse(TEXT("Pop with different PropertyBag structure should fail"), CDO->PopLightweightObject(DifferentObj));
	TestTrue(TEXT("Push with matching PropertyBag structure should succeed"), CDO->PushLightweightObject(ValidObj));

	return true;
}

// Test 12: Null Class
IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FLightweightObjectNullClassTest,
	"LightweightObject.Core.NullClass",
	EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::ProductFilter)

bool FLightweightObjectNullClassTest::RunTest(const FString& Parameters)
{
	FLightweightObject NullClassObj;
	NullClassObj.Class = nullptr;

	UTestLightweightObjectSimple* CDO = UTestLightweightObjectSimple::StaticClass()->GetDefaultObject<UTestLightweightObjectSimple>();
	CDO->PostLoad();

	TestFalse(TEXT("Push with null class should fail"), CDO->PushLightweightObject(NullClassObj));
	TestFalse(TEXT("Pop with null class should fail"), CDO->PopLightweightObject(NullClassObj));

	return true;
}

#endif // WITH_DEV_AUTOMATION_TESTS
