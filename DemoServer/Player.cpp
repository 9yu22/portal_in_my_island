#include "Player.h"

Player::Player()
{
	m_id = -1;
	m_hp = 100;

	m_rotation.pitch = 0.f;
	m_rotation.yaw = 0.f;
	m_rotation.roll = 0.f;
}

Player::~Player()
{
}

WorldRotation Player::GetWorldRotation()
{
	return m_rotation;
}

void Player::SetWorldRotation(WorldRotation new_rotation)
{
	m_rotation.pitch = new_rotation.pitch;
	m_rotation.yaw = new_rotation.yaw;
	m_rotation.roll = new_rotation.roll;
}

void Player::SetWorldRotation(float new_pitch, float new_yaw, float new_roll)
{
	m_rotation.pitch = new_pitch;
	m_rotation.yaw = new_yaw;
	m_rotation.roll = new_roll;
}

