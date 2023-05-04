#pragma once
#include "ecm.h"
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

using namespace sf;
using namespace std;

// Create animation component from class Component
class AnimationComponent : public Component
{
protected:
	float animationTimer;
	float frameTime;
	int frameCount;
	Vector2f set_size;
	Sprite sprite;
	Texture spritesheet;
	IntRect currentFrame;
	Vector2u currentTexture;
	bool end;

public:
	explicit AnimationComponent(Entity* p, Vector2f size);
	const Vector2f& getSize() const;
	const Sprite& getSprite() const;
	int getFrameCount() const;
	float getFrameTime() const;
	bool finished() const;
	void update(double dt) override;
	void render() override;
	void setSpritesheet(const Texture& setSpritesheet);
	void setFrameTime(float time);
	void setFrameCount(int count);
	void setSize(const Vector2f& size);
	void setScale(const Vector2f& size);
	void setRotation(float rot);
	void rotate(float rot);
	float getRotation();
	AnimationComponent() = delete;
};

class EnemyAnimationComp : public AnimationComponent
{
protected:
	Sprite turretSprite;
	Texture turretImage;

public:
	explicit EnemyAnimationComp(Entity* p, Vector2f size);
	void setTurretRotation(float rot);
	void rotateTurret(float rot);
	float getTurretRotation();
	void setRotation(float rot);
	void rotate(float rot);
	void update(double dt) override;
	void render() override;
	void setScale(const Vector2f& scale);
	Sprite& getTurretSprite();

};