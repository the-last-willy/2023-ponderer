#version 450

struct Particle {
    uint idx;
    uint value;
};

bool isAlive(in Particle p) {
    return true;
}

layout(binding = 0) readonly buffer inputCountBuffer  {
    uint inputCount;
};

layout(binding = 1) writeonly buffer outputCountBuffer  {
    uint outputCount;
};

layout(binding = 2, std430) buffer particleBuffer {
    Particle particles[];
};

int fib(int n) {
    int s = 0;
    for(int i = 0; i < n; ++i) {
        s += i * int(inputCount);
    }
    return s;
}

shared uint localNbAlive;
shared uint localOffset;

layout(local_size_x = 256) in;
void main() {
    uint idx = gl_GlobalInvocationID.x;
    if(idx >= inputCount) return;

    Particle currentParticle = particles[idx];

    if(gl_LocalInvocationID.x == 0) {
        localNbAlive = 0;
    }

    barrier();

    uint localIdx = 0;
    if(isAlive(currentParticle)) {
        localIdx = atomicAdd(localNbAlive, 1);
    }

    barrier();

    if(gl_LocalInvocationID.x == 0) {
        localOffset = atomicAdd(outputCount, localNbAlive);
    }

    barrier();

    if(isAlive(currentParticle)) {
        uint newIdx = localOffset + localIdx;
        particles[newIdx] = currentParticle;
    }
}
