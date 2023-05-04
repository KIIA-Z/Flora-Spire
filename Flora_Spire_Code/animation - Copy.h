#include "animation.h"
#include "system_renderer.h"
#include <cmath>
#include <system_resources.h>

using namespace sf;
using namespace std;

// Create animation component
AnimationComponent::AnimationComponent(Entity* p, Vector2f size) : Component(p), set_size(size)
{
	animationTimer = 0.0f;
	currentFrame.width = set_size.x;
	currentFrame.height = set_size.y;
	currentTexture = Vector2u(0, 0);
	frameTime = 0.2f;
	frameCount = 1;
	end = false;
	auto origin = set_size / 2.f;
	origin.x = floor(origin.x);
	origin.y = floor(origin.y);
	sprite.setOrigin(origin);
}

void AnimationComponent::update(double dt)
{
	end = false;
	animationTimer += dt;
	if (animationTimer > frameTime)
	{
		animationTimer -= frameTime;
		currentTexture.x++;
		if (currentTexture.x >= frameCount)
		{
			end = true;
			currentTexture.x = 0;
		}
	}

	currentFrame.left = currentTexture.x * set_size.x;
	currentFrame.top = currentTexture.y * set_size.y;
	sprite.setTextureRect(currentFrame);
	sprite.setPosition(_parent->getPosition());
}

void AnimationComponent::render()
{
	Renderer::queueAnimation(sprite);
}

void AnimationComponent::setSpritesheet(const Texture& setSpritesheet)
{
	spritesheet = setSpritesheet;
	sprite.setTexture(spritesheet);
}

void AnimationComponent::setFrameTime(float time)
{
	frameTime = time;
}

void AnimationComponent::setFrameCount(int count)
{
	frameCount = count;
}

void AnimationComponent::setSize(const Vector2f& size)
{
	set_size = size;
	currentFrame.left = set_size.x;
	currentFrame.top = set_size.y;

	auto origin = set_size / 2.f;
	origin.x = floor(origin.x);
	origin.y = floor(origin.y);
	sprite.setOrigin(origin);
}

void AnimationComponent::setScale(const Vector2f& scale)
{
	//set_size = size;
	sprite.setScale(scale);
}


const Vector2f& AnimationComponent::getSize() const
{
	return set_size;
}

int AnimationComponent::getFrameCount() const
{
	return frameCount;
}

float AnimationComponent::getFrameTime() const
{
	return frameTime;
}

bool AnimationComponent::finished() const
{
	return end;
}

const Sprite& AnimationComponent::getSprite() const
{
	return sprite;
}



// -- Enemy animation components
EnemyAnimationComp::EnemyAnimationComp(Entity* p, Vector2f size) : AnimationComponent(p, size)
{
	turretImage = *Resources::load<Texture>("enemyTurret.png");
	turretSprite.setTexture(turretImage);
	auto bounds = turretSprite.getGlobalBounds();
	//turretSprite.setOrigin(bounds.getSize().x / 2, bounds.getSize().y - 5.f);
}



void EnemyAnimationComp::setRotation(float rot)
{
	AnimationComponent::setRotation(rot);
	setTurretRotation(rot);
}

void EnemyAnimationComp::rotate(float rot)
{
	AnimationComponent::rotate(rot);
	rotateTurret(rot);
}

void EnemyAnimationComp::setScale(const Vector2f& scale)
{
	AnimationComponent::setScale(scale);
	turretSprite.setScale(scale);
}

void EnemyAnimationComp::update(double dt)
{
	AnimationComponent::update(dt);
	turretSprite.setPosition(_parent->getPosition());
}

void EnemyAnimationComp::render()
{
	AnimationComponent::render();
	Renderer::queueAnimation(turretSprite);
	//Renderer::queue(&sprite);
	//Renderer::queue(&turretSprite);
}


Sprite& EnemyAnimationComp::getTurretSprite() {
	return turretSprite;
}