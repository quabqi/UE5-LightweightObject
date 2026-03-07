# LightweightObject

A lightweight object system for Unreal Engine 5 using `FInstancedPropertyBag` for data storage with CDO reuse pattern.

## Design

[Detailed Design Article](https://zhuanlan.zhihu.com/p/1978493760200536584)

## Quick Start

### 1. Define a Lightweight Object Class

```cpp
UCLASS(Blueprintable)
class UMyData : public ULightweightObjectBase
{
    GENERATED_BODY()
public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Health = 100;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString Name;
};
```

### 2. Usage

```cpp
UMyData* CDO = UMyData::StaticClass()->GetDefaultObject<UMyData>();
CDO->PostLoad();

// Capture current state
FLightweightObject Obj;
CDO->ConvertToFLightweightObject(Obj);

// Modify CDO
CDO->Health = 200;

// Restore state
CDO->PushLightweightObject(Obj);  // Health = 100
```

## API

| Method | Description |
|--------|-------------|
| `CreateLightweightObject()` | Create with default values |
| `ConvertToFLightweightObject()` | Create and capture current values |
| `PushLightweightObject()` | Restore values to CDO |
| `PopLightweightObject()` | Copy CDO values into object |

## Running Tests

```batch
RunTests.bat
```
