#pragma once

#include "CoreMinimal.h"
#include "Containers/Array.h"
#include "Misc/Optional.h"

template<typename ElementType>
USTRUCT(BlueprintType)
class TInvenArray
{
public:
    // 요소 추가
    void Add(int32 Index, const ElementType& Element)
    {
        //EnsureCapacity(Index);
        Elements[Index] = Element;
    }

    // 요소 접근 (const 버전)
    TOptional<ElementType> Get(int32 Index) const
    {
        if (Index < Elements.Num())
        {
            return Elements[Index];
        }
        return TOptional<ElementType>();
    }

    // 요소 접근 (non-const 버전)
    TOptional<ElementType>& Get(int32 Index)
    {
        if (Index < Elements.Num())
        {
            return Elements[Index];
        }
        static TOptional<ElementType> EmptyValue;
        return EmptyValue;
    }

    // 요소 제거
    void Remove(int32 Index)
    {
        if (Index < Elements.Num())
        {
            Elements[Index].Reset();
        }
    }

    // 배열 크기 반환
    int32 Num() const
    {
        return Elements.Num();
    }

private:
    TArray<TOptional<ElementType>> Elements;

    // 필요한 크기까지 배열을 확장
    void EnsureCapacity(int32 Index)
    {
        if (Index >= Elements.Num())
        {
            Elements.SetNum(Index + 1);
        }
    }
};
