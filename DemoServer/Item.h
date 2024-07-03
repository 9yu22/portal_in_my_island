#pragma once
#include "GameObject.h"
class Item : public GameObject
{
public:
	char item_type = 0;

public:
	Item();
	~Item();
};

