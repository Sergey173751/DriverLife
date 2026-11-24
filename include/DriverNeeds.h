#pragma once
#include <algorithm>

struct DriverNeeds {
    float hunger = 100.0f;
    float thirst = 100.0f;
    float fatigue = 0.0f;
    float hygiene = 100.0f;
    float toilet = 100.0f;

    static float Clamp(float v) { return std::clamp(v, 0.0f, 100.0f); }

    void Update(float gameMinutes) {
        if (gameMinutes <= 0.0f) return;
        hunger = Clamp(hunger - gameMinutes * 0.08f);
        thirst = Clamp(thirst - gameMinutes * 0.15f);
        fatigue = Clamp(fatigue + gameMinutes * 0.10f);
        hygiene = Clamp(hygiene - gameMinutes * 0.03f);
        toilet = Clamp(toilet - gameMinutes * 0.12f);
    }
    void Eat(float amount) { hunger = Clamp(hunger + amount); }
    void Drink(float amount) { thirst = Clamp(thirst + amount); }
    void Sleep(float minutes) { fatigue = Clamp(fatigue - minutes * 0.8f); }
    void Wash() { hygiene = 100.0f; }
    void UseToilet() { toilet = 100.0f; }
};
