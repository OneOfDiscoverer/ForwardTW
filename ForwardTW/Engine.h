#include <iostream>
#include <thread>
#include <list>
#include <vector>
#include <chrono>

using namespace std;

class Line {
private:
    float m, b, minx, maxx;
public:
    Line(float x1, float x2, float y1, float y2) {
        m = (y1 - y2) / (x1 - x2);
        b = -(m * x1) + y1;
        minx = x1;
        maxx = x2;
    }
    float getMinX() {
        return minx;
    }
    float getMaxX() {
        return maxx;
    }
    float getPointFromX(float x) {
        return m * x + b;
    }
};

class TorqueFromRotary {
private:
    vector<float> vals;
public:
    TorqueFromRotary(list<Line> *input) {
        for (auto iter = input->begin(); iter != input->end(); iter++) {
            for (float x = iter->getMinX(); x < iter->getMaxX(); x += 0.1) {
                vals.push_back(iter->getPointFromX(x));
            }
        }
    }
    float getTorque(float x) {
        if (x < 0) {
            return vals[0];
        }
        if (x * 10 >= vals.size())
        {
            return vals[vals.size() - 1];
        }
        return vals[(int)(x * 10)];
    }
    float getMaxVRotary() {
        return vals.size() / 10;
    }
};

class Timer {
private:
    int tick;
    thread* trd;
    bool trigger, stopper = true;
public:
    Timer(int tick) {
        this->tick = tick;
        trd = new thread(&Timer::Run, this);
    }
    ~Timer() {
        stopper = false;
        trd->join();
        delete trd;
    }
    void Run() {
        while (stopper) {
            this_thread::sleep_for(std::chrono::milliseconds(tick));
            trigger = true;
        }
    }
    void getTick() {
        trigger = false;
        while (!trigger);
    }
};

class Engine : public TorqueFromRotary {
protected:
    float TAccelFromTorque, TAccelFromRotary, CoolingFromAmbient, Inertion;
    float Temp, tAmbient;
    float vRotary = 0, targetVRotary = 0;
    int Second = 0;
    float ARotary() {
        return getTorque(vRotary) / Inertion;
    }
    float VHeating() {
        if (vRotary < 10) {
            return 0;
        }
        return (getTorque(vRotary) * TAccelFromTorque) + (vRotary * vRotary * TAccelFromRotary);
    }
    float VCooling() {
        return CoolingFromAmbient * (tAmbient - Temp);
    }
    float Power(float torque, float rpm) {
        return (torque * rpm) / 1000;
    }
public:
    Engine(float TAccelFromTorque, float TAccelFromRotary, float CoolingFromAmbient, float Inertion, list<Line>* lineFam) : TorqueFromRotary(lineFam) {
        this->TAccelFromTorque = TAccelFromTorque;
        this->TAccelFromRotary = TAccelFromRotary;
        this->CoolingFromAmbient = CoolingFromAmbient;
        this->Inertion = Inertion;
    }
    //метод Frame при каждом вызове обновляет состояние двигателя до следующей секунды
    void Frame() {
        if (vRotary < targetVRotary && vRotary < getMaxVRotary()) {
            vRotary += ARotary();
        }
        else {
            vRotary -= ARotary();
        }
        Temp += VCooling() + VHeating();
        Second++;
    }
    void getState() {
        printf("Torque: %5.1f vRotary: %5.1f Temp: %5.1f Power: %5.1f\n", getTorque(vRotary), vRotary, Temp, Power(getTorque(vRotary), vRotary));
    }
    int getSecond() {
        return Second;
    }
    void settargetVRotary(float targetvRotary) {
        this->targetVRotary = targetvRotary;
    }
    float getVRotary() {
        return vRotary;
    }
    float getTemp() {
        return Temp;
    }
    float getCurrentPower() {
        return Power(getTorque(vRotary), vRotary);
    }
};