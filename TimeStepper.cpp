#include "TimeStepper.hpp"
#include <iostream>

///TODO: implement Explicit Euler time integrator here
void ForwardEuler::takeStep(ParticleSystem* particleSystem, float stepSize) {
    vector<Vector3f>& state = particleSystem->getState(); // X(t)
    vector<Vector3f> f = particleSystem->evalF(state); // f(X, t)

    vector<Vector3f> newState(state.size()); // X(t + h)
    for (size_t i = 0; i < newState.size(); i++) {
        newState[i] = state[i] + stepSize*f[i]; // X(t + h) = X(t) + h*f(X, t)
    }

    particleSystem->setState(newState);
}

///TODO: implement Trapzoidal rule here
void Trapzoidal::takeStep(ParticleSystem* particleSystem, float stepSize) {
    vector<Vector3f>& state = particleSystem->getState(); // X(t)
    vector<Vector3f> f0 = particleSystem->evalF(state); // f(X, t), f0

    vector<Vector3f> state1(state.size());
    for (size_t i = 0; i < state1.size(); i++) {
        state1[i] = state[i] + stepSize*f0[i]; // X + h*f0
    }
    vector<Vector3f> f1 = particleSystem->evalF(state1); // f1 = f(X + h*f0, t + h)
    
    vector<Vector3f> newState(state.size()); // X(t + h)
    for (size_t i = 0; i < newState.size(); i++) {
        newState[i] = state[i] + stepSize*(f0[i] + f1[i])/2.f; // X + h*(f0 + f1)/2
    }

    particleSystem->setState(newState);
}


void RungeKutta4::takeStep(ParticleSystem* particleSystem, float stepSize) {
    vector<Vector3f>& state = particleSystem->getState(); // X(t)
    vector<Vector3f> f0 = particleSystem->evalF(state); // f(X, t), f0

    vector<Vector3f> state1(state.size());
    for (size_t i = 0; i < state1.size(); i++) {
        state1[i] = state[i] + stepSize/2.f*f0[i]; // X + h/2*f0
    }
    vector<Vector3f> f1 = particleSystem->evalF(state1); // f(X + h/2*f0, t + h/2), f1

    vector<Vector3f> state2(state.size());
    for (size_t i = 0; i < state2.size(); i++) {
        state2[i] = state[i] + stepSize/2.f*f1[i]; // X + h/2*f1
    }
    vector<Vector3f> f2 = particleSystem->evalF(state2); // f(X + h/2*f0, t + h/2), f2

    vector<Vector3f> state3(state.size());
    for (size_t i = 0; i < state3.size(); i++) {
        state3[i] = state[i] + stepSize*f2[i]; // X + h*f2
    }
    vector<Vector3f> f3 = particleSystem->evalF(state3); // f(X + h*f2, t + h), f3

    vector<Vector3f> newState(state.size()); // X(t + h)
    for (size_t i = 0; i < newState.size(); i++) {
         // X + h*(f0 + f1 + f2 + f3)/6
        newState[i] = state[i] + stepSize*(f0[i] + f1[i] + f2[i] + f3[i])/6.f;
    }

    particleSystem->setState(newState);
}