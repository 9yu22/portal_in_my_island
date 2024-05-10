#pragma once

UENUM(BlueprintType)
enum class BKEDirection : uint8
{
	Forward, Right, Back, Left, Up, Down
};

UENUM(BlueprintType)
enum class BKEBlock : uint8
{
	Null, Air, Stone, Dirt, Grass, Amethyst
};

UENUM(BlueprintType)
enum class EGenerationType : uint8
{
	GT_3D UMETA(DisplayName = "3D"),
	GT_2D UMETA(DisplayName = "2D"),
};