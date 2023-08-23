#include "TestRuntime.h"

int OverHeatTest(TestEngine* engine, float tAmbient, float vRotary, float tOverHeat) {
    cout << "Запуск теста на перегрев" << endl;
    this_thread::sleep_for(std::chrono::milliseconds(1000));
    engine->settargetVRotary(vRotary);
    engine->Starter(tAmbient);
    while (true) {
        this_thread::sleep_for(std::chrono::milliseconds(10));
        engine->getState();
        if (engine->getTemp() > tOverHeat) {
            return engine->getSecond();
        }
    }
}

void MaxPowerTest(TestEngine* engine, float setvRotary, float tAmbient, struct resTest2* result) {
    float maxPower = 0, maxvRotary = 0;
    cout << "Запуск теста на максимальную мощность" << endl;
    this_thread::sleep_for(std::chrono::milliseconds(1000));
    engine->settargetVRotary(setvRotary);
    engine->Starter(tAmbient);
    while (true) {
        this_thread::sleep_for(std::chrono::milliseconds(10));
        engine->getState();
        float tmpPower = engine->getCurrentPower();
        if (tmpPower > maxPower) {
            maxPower = tmpPower;
            maxvRotary = engine->getVRotary();
        }
        if (round(engine->getMaxVRotary()) == round(engine->getVRotary())) {
            result->maxPower = maxPower;
            result->maxvRotary = maxvRotary;
            return;
        }
    }
}