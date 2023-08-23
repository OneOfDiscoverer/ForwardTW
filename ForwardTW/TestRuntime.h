#include <iostream>
#include <thread>
#include <list>
#include <vector>
#include <chrono>
#include "Engine.h"

using namespace std;

class TestEngine : public Engine {
private:
    thread* trd;
    bool toStop = true;
public:
    TestEngine(float TAccelFromTorque, float TAccelFromRotary, float CoolingFromAmbient, float Inertion, list<Line> *lineFam) :
        Engine(TAccelFromTorque, TAccelFromRotary, CoolingFromAmbient, Inertion, lineFam) {
    }
    ~TestEngine() {
        toStop = false;
        trd->join();
        delete trd;
    }
    void State() {
        Timer timer = Timer(100);
        while (toStop) {
            timer.getTick();
            Frame();
        }
    }
    void Starter(float tAmbient) {
        this->Temp = this->tAmbient = tAmbient;
        trd = new thread(&TestEngine::State, this);
    }
};

struct resTest2 {
    float maxPower;
    float maxvRotary;
};

int OverHeatTest(TestEngine* engine, float tAmbient, float vRotary, float tOverHeat);
void MaxPowerTest(TestEngine* engine, float setvRotary, float tAmbient, struct resTest2* result);
