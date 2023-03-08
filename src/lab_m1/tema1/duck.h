#pragma once
#include "components/simple_scene.h"

namespace m1
{
    class Duck
    {
        float size;
        Mesh* body;
        Mesh* head;
        Mesh* beak;
        Mesh* supWing;
        Mesh* infWing;
        Mesh* supTail;
        Mesh* infTail;
        glm::mat3 duckPosition;
        glm::mat3 duckRotation;
        glm::mat3 wingRotation;
        float duckSpeedX, duckSpeedY;
        float flightTime;
        bool isAlive;

     public:
        Duck(float, float);
        ~Duck();

        Mesh* getBeak();
        Mesh* getHead();
        Mesh* getBody();
        Mesh* getSupWing();
        Mesh* getInfWing();
        Mesh* getSupTail();
        Mesh* getInfTail();
        float getDuckX();
        float getDuckY();
        float getDuckSpeedX();
        float getDuckSpeedY();
        float getDuckSpeed();
        float getFlightTime();
        float getSize();
        void setDuckSpeedX(float);
        void setDuckSpeedY(float);
        void kill();
        bool getAlive();



        glm::mat3 getDuckTranslation();
        glm::mat3 getWingTranslation();

        void moveDuck(float, float);
        void flapWings(float);
        void fly(float);
        bool escape(float);
        bool die(float);

        void flapWings2(float);
        void mirror();
        void setBounds(int, int);

     private:

     protected:
         int maxX, maxY;

    };
}  
