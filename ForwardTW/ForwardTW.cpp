#include "TestRuntime.h"

int main()
{
    setlocale(LC_ALL, "RU");
    float tAmbient;
    cin >> tAmbient;
    list<Line> TorqueFromRotary = list<Line>({
        Line(0, 75, 20, 75),
        Line(75, 150, 75, 100),
        Line(150, 200, 100, 105),
        Line(200, 250, 105, 75),
        Line(250, 300, 75, 0),
    });
    TestEngine *engine = new TestEngine(0.01, 0.0001, 0.1, 10, &TorqueFromRotary);
    int resTest1 = OverHeatTest(engine, tAmbient, 400, 110);
    delete engine;
    engine = new TestEngine(0.01, 0.0001, 0.1, 10, &TorqueFromRotary);
    struct resTest2 result2;
    MaxPowerTest(engine, 400, 25, &result2);
    delete engine;
    printf("Время с момента запуска до перегрева %d секунд\n", resTest1);
    printf("Максимальная мощность %5.1f кВт при %5.1f рад/сек\n", result2.maxPower, result2.maxvRotary);
}
