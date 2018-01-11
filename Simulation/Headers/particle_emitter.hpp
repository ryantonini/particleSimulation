//
//  particle_emitter.hpp
//
//
//  Created by Ryan Tonini on 2017-11-14.
//

#ifndef PARTICLE_EMITTER_HPP
#define PARTICLE_EMITTER_HPP

#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "texture.hpp"
#include "shader.hpp"
#include "particle_emitter.hpp"

// single particle representation
struct Particle {
    glm::vec3 position, velocity, accel;
    glm::vec4 color;
    GLfloat lifespan, age;
    GLint type; //  0 for fire, 1 for smoke
    GLint status;
    
    Particle() : position(0.0f), velocity(0.0f), accel(0.0f), color(1.0f), lifespan(0.0f), age(0.0f), type(0), status(0) { }
};

// particle emitter -- represents a system of particles
class ParticleEmitter {
    public:
    // constructor
        ParticleEmitter(Shader shader, Texture2D texture, GLuint maxParticles);
        // render all particles
        void draw(const glm::mat4 &view, const glm::mat4 &projection);
        // updates particles
        void update(GLfloat dt, GLuint newParticles, bool isRed);
        // convert fire particles to smoke
        void fireToSmoke(Particle &p);
        // delete shaders
        void clear();
    private:
        // initalizes vector of type Particle
        std::vector<Particle> particles;
        GLuint maxParticles;
        Shader shader;
        Texture2D texture;
        GLuint VAO;
        // initializes buffer and vertex attributes
        void startSystem();
        // returns the first particle index that's currently unused - or overrides the next one if all are active
        GLuint findUnusedParticle();
        // create new particle
        void createParticle(Particle &particle);
    
};

#endif /* PARTICLE_EMITTER_HPP */


