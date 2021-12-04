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
