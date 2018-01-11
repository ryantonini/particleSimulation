//
//  particle_emitter.cpp
//
//  References: https://learnopengl.com/#!In-Practice/2D-Game/Particles
//
//  TODO:
//  1. Finish implementing smoke effect
//  2. Use instancing for better performance
//
//  Created by Ryan Tonini on 2017-11-14.
//

#include "particle_emitter.hpp"
#include "texture.hpp"

ParticleEmitter::ParticleEmitter(Shader shader, Texture2D texture, GLuint maxParticles)
:  maxParticles(maxParticles), shader(shader), texture(texture)
{
    this->startSystem();
}

void ParticleEmitter::update(GLfloat dt, GLuint newParticles, bool isRed) {
    // Add new particles
    for (GLuint i = 0; i < newParticles; ++i) {
        int unusedParticle = this->findUnusedParticle();
        this->createParticle(particles[unusedParticle]);
    }
    // Update all particles
    for (GLuint i = 0; i < this->maxParticles; ++i) {
        Particle &p = this->particles[i];
        p.age += dt; // increment age
        
        p.position[0] = p.position[0] + p.velocity[0] + p.accel[0];
        p.position[1] = p.position[1] + p.velocity[1] + p.accel[1];
        p.position[2] = p.position[2] + p.velocity[2] + p.accel[2];
        
        p.accel[0] = p.accel[0] + 0.0f;
        p.accel[1] = p.accel[1] + 0.0005; // acceleration (only in y direction)
        p.accel[2] = p.accel[2] + 0.0f;
        
        // vary color depending on age of particle
        float diff = p.lifespan/p.age;
        
        // color assignment
        if (p.type == 0) {
            if(diff < 4.10)
            { // red
                if (isRed)
                    p.color = glm::vec4(1.0f, 0.25f, 0.0f, p.color.a);
                else
                    p.color = glm::vec4(0.0f, 0.5f, 0.0f, p.color.a);
            }
            else if(diff < 7.20)
            { // gold
                if (isRed)
                    p.color = glm::vec4(1.0f, 0.5f, 0.0f, p.color.a);
                else
                    p.color = glm::vec4(0.5f, 1.0f, 0.0f, p.color.a);
            }
            else if(diff < 12.0)
            { // yellow
                if (isRed)
                    p.color = glm::vec4(1.0f, 1.0f, 0.0f, p.color.a);
                else
                    p.color = glm::vec4(0.5f, 1.0f, 0.0f, p.color.a);
            }
            else
            { // initial light
                p.color = glm::vec4(1.0f, 0.95f, 0.8f, p.color.a);
            }
        } else if (p.type == 1) { // for smoke (not fully implemmented)
            if (diff < 3.0) {
                p.color = glm::vec4(0.7f, 0.7f, 0.7f, p.color.a);
            }
        }
        
        p.color.a -= dt * 0.25;  // reduce opacity of particle on each iteration
        
        // bounding conditions
        if (p.type == 0 && (p.age > p.lifespan || p.position[1] > -10 || p.position[1] < -90 || p.position[0] > 25 || p.position[0] < -25 || p.position[2] > 10 || p.position[2] < -15)) {
             p.age += p.lifespan;
     
            // smoke effect (not fully implemmented)
     
    /*       int temp = rand()%100;
             if(temp < 0 && p.status != 1) {  // not using smoke currently
                this->fireToSmoke(p);
                //lastUsedParticle = i;
             } else {
                p.status = 1;
             }
        } else if (p.type == 1 && (p.position[1] > -10|| p.position[1] < -120|| p.position[0] > 40 || p.position[0] < -40 || p.position[2] > 20 || p.position[2] < -20 )) {
            p.age += p.lifespan;
        }
    */
        }
    }
}

void ParticleEmitter::startSystem() {
    // Set up mesh and attribute properties
    GLuint VBO, EBO;
    GLfloat particle_quad[] = {
        0.0f, 0.0f, -3.0f, 0.0f, 0.0f,
        0.0f, 0.5f, -3.0f, 0.0f, 1.0f,
        0.5f, 0.5f, -4.0f, 1.0f, 1.0f,
        0.5f, 0.0f, -4.0f, 1.0f, 0.0f,
    };
    unsigned int indices[] = {
        0, 1, 3,
        1, 2, 3
    };
    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glBindVertexArray(this->VAO);
    // Fill mesh buffer
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(particle_quad), particle_quad, GL_STATIC_DRAW);
    // Fill element buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    // Set mesh attributes
    // Position coords
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    // Tex coords
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3* sizeof(float)));
    glEnableVertexAttribArray(1);
    // Unbind buffers
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    
    // Create particles equal to maxParticles
    for (GLuint i = 0; i < this->maxParticles; ++i)
        this->particles.push_back(Particle());
}

void ParticleEmitter::fireToSmoke(Particle &p) {
    p.lifespan = 6.0f;
    p.age = 0.0f;
    p.type = 1;
    
    // adjust velocity settings
    // TODO
    
    p.color = glm::vec4(0.7f, 0.7f, 0.7f, 1.0f);
}

GLuint lastUsedParticle = 0;
GLuint ParticleEmitter::findUnusedParticle() {
    // search from last index onwards
    for (GLuint i = lastUsedParticle; i < this->maxParticles; ++i){
        if (this->particles[i].age >= particles[i].lifespan){
            lastUsedParticle = i;
            return i;
        }
    }
    
   // scan from beginning to last index
    for (GLuint i = 0; i < lastUsedParticle; ++i){
        if (this->particles[i].age >= particles[i].lifespan){
            lastUsedParticle = i;
            return i;
        }
    }
    
    // all particles are alive, override the first
    lastUsedParticle = 0;
    return 0;
}

void ParticleEmitter::createParticle(Particle &particle) {
    particle.color = glm::vec4(1.0f, 0.95f, 0.8f, 1.0f);
    particle.type = 0;
    particle.status = 0;
    particle.lifespan = 5.0f;
    particle.age = 0.0f;
    particle.velocity[0] = (((((((2) * rand()%11) + 1)) * rand()%11) + 1) * 0.004) ;
    particle.velocity[1] =  ((((((5) * rand()%11) + 5)) * rand()%11) + 1) * 0.04;
    particle.velocity[2] = (((((((2) * rand()%11) + 1)) * rand()%11) + 1) * 0.004);
    particle.position[0] = ((rand()%7)-(rand()%7));
    particle.position[1] = -17;
    particle.position[2] = 0.0;
    particle.accel = glm::vec3(0.0f, 0.0f,0.0f);
}

// Render all particles
void ParticleEmitter::draw(const glm::mat4 &view, const glm::mat4 &projection)
{
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    this->shader.use();
    for (Particle particle : this->particles)
    {
        if (particle.age < particle.lifespan)
        {
            this->shader.setVec3("offset", particle.position);
            this->shader.setVec4("color", particle.color);
            this->shader.setMat4("projection", projection);
            this->shader.setMat4("view", view);
            this->texture.Bind();
            glBindVertexArray(this->VAO);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);
        }
    }
    // return to default blending
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

// Delete shader
void ParticleEmitter::clear() {
    glDeleteProgram(this->shader.ID);
}
