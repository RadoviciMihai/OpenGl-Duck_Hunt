#include "lab_m1/tema1/duck.h"

#include <vector>
#include <iostream>

#include "lab_m1/tema1/transform2D.h"
#include "lab_m1/tema1/object2D.h"

using namespace std;
using namespace m1;



Duck::Duck(float size, float time)
{	
	this->size = size*5;
	this->beak = object2D::CreateTriangle("beak", glm::vec3(size * 5, size * 2.5, 0), glm::vec3(size * 5, size * 3, 0), glm::vec3(size * 4.75, size * 2.75, 0), glm::vec3((float)212/255, (float)129/255, (float)12/255), true);
	this->head = object2D::CreateTriangle("head", glm::vec3(size * 4, size * 2, 0), glm::vec3(size * 3.5, size * 2.5, 0), glm::vec3(size * 5, size * 3, 0), glm::vec3((float)9/255, (float)133/255, (float)119/255), true);
	this->body = object2D::CreateTriangle("body", glm::vec3(size * 1, size * 1, 0), glm::vec3(size * 4, size * 2, 0), glm::vec3(size * 3, size * 3, 0), glm::vec3((float)110 / 255, (float)90 / 255, (float)40 / 255), true);
	this->supWing = object2D::CreateTriangle("supWing", glm::vec3(size * 2.3, size * 2.3, 0), glm::vec3(size * 3, size * 3, 0), glm::vec3(size * 2, size * 5, 0), glm::vec3((float)200 / 255, (float)180 / 255, (float)100 / 255), true);
	this->infWing = object2D::CreateTriangle("infWing", glm::vec3(size * 2.3, size * 2.3, 0), glm::vec3(size * 2, size * 5, 0), glm::vec3(size * 2, size * 3, 0), glm::vec3((float)170 / 255, (float)150 / 255, (float)100 / 255), true);
	this->supTail = object2D::CreateTriangle("supTail", glm::vec3(size * 0.5, size * 1.3, 0), glm::vec3(size * 1.5, size * 1.5, 0), glm::vec3(size * 0, size * 2, 0), glm::vec3((float)110 / 255, (float)80 / 255, (float)35 / 255), true);
	this->infTail = object2D::CreateTriangle("infTail", glm::vec3(size * 0, size * 0, 0), glm::vec3(size * 1.5, size * 1.5, 0), glm::vec3(size * 0.5, size * 1.3, 0), glm::vec3((float)95 / 255, (float)56 / 255, (float)30 / 255), true);

	this->duckPosition = glm::mat3(1);
	this->duckRotation = glm::mat3(1);
	this->wingRotation = glm::mat3(1);

	this->flightTime = time;
	this->isAlive = true;
}


Duck::~Duck()
{
}

Mesh* m1::Duck::getBeak()
{
	return this->beak;
}

Mesh* m1::Duck::getHead()
{
	return this->head;
}

Mesh* m1::Duck::getBody()
{
	return this->body;
}

Mesh* m1::Duck::getSupWing()
{
	return this->supWing;
}

Mesh* m1::Duck::getInfWing()
{
	return this->infWing;
}

Mesh* m1::Duck::getSupTail()
{
	return this->supTail;
}

Mesh* m1::Duck::getInfTail()
{
	return this->infTail;
}

float m1::Duck::getDuckX()
{
	return this->duckPosition[2][0];
}

float m1::Duck::getDuckY()
{
	return this->duckPosition[2][1];
}

float m1::Duck::getDuckSpeedX()
{
	return this->duckSpeedX;
}

float m1::Duck::getDuckSpeedY()
{
	return this->duckSpeedY;
}

float m1::Duck::getDuckSpeed()
{
	return sqrt(this->duckSpeedX * this->duckSpeedX + this->duckSpeedY * this->duckSpeedY);
}

float m1::Duck::getFlightTime()
{
	return this->flightTime;
}

float m1::Duck::getSize()
{
	return this->size;
}

void m1::Duck::setDuckSpeedX(float x)
{
	this->duckSpeedX = x;
}

void m1::Duck::setDuckSpeedY(float y)
{
	this->duckSpeedY = y;
}

void m1::Duck::kill()
{
	this->isAlive = false;
	this->duckRotation = transform2D::Rotate(-M_PI / 2);
	if (this->flightTime > 0) {
		this->moveDuck(0, this->size *0.5);
		if (this->duckSpeedX > 0)
			this->moveDuck(this->size / 2,0);
		else
			this->moveDuck(-this->size / 2, 0);
	}
	else {
		this->moveDuck(-this->size / 2, this->size);
	}
}

bool m1::Duck::getAlive()
{
	return this->isAlive;
}

glm::mat3 m1::Duck::getDuckTranslation()
{
	return this->duckPosition*this->duckRotation;
}

glm::mat3 m1::Duck::getWingTranslation()
{
	return this->duckPosition*this->duckRotation*this->wingRotation;
}

void m1::Duck::moveDuck(float x, float y)
{
	this->duckPosition *= transform2D::Translate(x, y);
}

void m1::Duck::flapWings(float val) {
	static float wingFlap = 0;
	static float flapDirection = 1;
	static bool orient = false;
	if (wingFlap <= 0) {
		flapDirection = 1;
		orient = !orient;
		if (orient)
			this->wingRotation = glm::mat3(1);
	}
	if (wingFlap >= 1) {
		flapDirection = -1;
		this->wingRotation *= transform2D::Invert();
	}
	wingFlap += val * flapDirection;
	this->wingRotation *= transform2D::ProjectStep(val * flapDirection);
}

void m1::Duck::fly(float dt)
{
	this->moveDuck(dt* this->duckSpeedX, dt * this->duckSpeedY);
	this->flapWings(0.05);
	this->flightTime -= dt;
	if (flightTime <= 0) {
		this->duckRotation = transform2D::Rotate(M_PI / 2);
	}
	if (this->getDuckX() + this->size > maxX && this->duckSpeedX>0) {
		this->duckSpeedX *= -1;
		this->moveDuck(this->size,0);
		this->duckRotation = transform2D::Mirror();
	}

	if (this->getDuckX()  < this->size && this->duckSpeedX < 0) {
		this->duckSpeedX *= -1;
		this->moveDuck(-this->size, 0);
		this->duckRotation = glm::mat3(1);
	}

	if ((this->getDuckY() + this->size > maxY && this->duckSpeedY > 0) || (this->getDuckY() < 0 && this->duckSpeedY < 0))  this->duckSpeedY *= -1;

}

bool m1::Duck::escape(float dt)
{
	if (this->getDuckY() - this->size > maxY)
		return true;
	float speed = sqrt(this->duckSpeedX * this->duckSpeedX + this->duckSpeedY * this->duckSpeedY);
	this->moveDuck(0, dt * speed);
	this->flapWings(0.05);
	return false;
}

bool m1::Duck::die(float dt)
{
	if (this->getDuckY() + this->size < 0)
		return true;
	this->moveDuck(0, dt * (- 250));
	return false;
}

void m1::Duck::flapWings2(float dt)
{	
	static float flapDirection = 1;
	static float wingFlap = 0;

	if (wingFlap <= 0)
		flapDirection = 1;
	if (wingFlap >= M_PI) {
		flapDirection = -1;
	}
	wingFlap += flapDirection *dt *1.7;
	this->wingRotation *= transform2D::Rotate(M_PI / 4) * transform2D::RotateOX(dt * flapDirection * M_PI/2) * transform2D::Rotate(-M_PI/4);
}

void m1::Duck::mirror()
{

	this->duckSpeedX *= -1;
	this->moveDuck(this->size, 0);
	this->duckRotation = transform2D::Mirror();

}

void m1::Duck::setBounds(int x, int y)
{
	maxX = x;
	maxY = y;
}

