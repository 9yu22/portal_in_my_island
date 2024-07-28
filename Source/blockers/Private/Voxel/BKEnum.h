#pragma once

UENUM(BlueprintType)
enum class BKEDirection : uint8
{
	Forward, Right, Back, Left, Up, Down
};

UENUM(BlueprintType)
enum class BKEBlock : uint8
{
	Null, Air, Stone, Dirt, Grass, Amethyst, Stair, Mycelium, Diamond, Leaves, Spruce, Ruby, HellGround, Snow,
	StoneB1, StoneB2, StoneB3,
	AmethystB1, AmethystB2, AmethystB3,
	RubyB1, RubyB2, RubyB3,
	DiamondB1, DiamondB2, DiamondB3,
	IcePillar, HellPillar, JackOLantern
};

UENUM(BlueprintType)
enum class EGenerationType : uint8
{
	GT_3D UMETA(DisplayName = "3D"),
	GT_2D UMETA(DisplayName = "2D"),
};