#include <iostream>
#include <vector>

#include "CGL/vector2D.h"

#include "mass.h"
#include "rope.h"
#include "spring.h"

namespace CGL {

    Rope::Rope(Vector2D start, Vector2D end, int num_nodes, float node_mass, float k, vector<int> pinned_nodes)
    {
        // TODO (Part 1): Create a rope starting at `start`, ending at `end`, and containing `num_nodes` nodes.

        for (size_t i = 0; i < num_nodes; i++) {
            Vector2D position = start + i * (end - start) / (num_nodes - 1);
            Mass *mass = new Mass(position, node_mass, false); 
            masses.emplace_back(mass);
            if (i > 0) {
                Spring *spring = new Spring(masses[i], masses[i-1], k);
                springs.emplace_back(spring);
            }
        }
//        Comment-in this part when you implement the constructor
       for (auto &i : pinned_nodes) {
           masses[i]->pinned = true;
       }
    }

    void Rope::simulateEuler(float delta_t, Vector2D gravity)
    {
        for (auto &s : springs)
        {
            // TODO (Part 2): Use Hooke's law to calculate the force on a node
            float clength = (s->m1->position - s->m2->position).norm();
            Vector2D distance = s->m1->position - s->m2->position;
            Vector2D force = -s->k * distance / clength * (clength - s->rest_length);
            s->m1->forces += force;
            s->m2->forces -= force;
            
             // damping
            Vector2D reve = s->m2->velocity - s->m1->velocity;
            Vector2D force1 = 0.05 * (reve.x * dis.x + reve.y * dis.y) * dis / length;
            s->m1->forces += force1;
            s->m2->forces -= force1;
            // air damping
            s->m1->forces -= 0.005 * s->m1->velocity;
            s->m2->forces -= 0.005 * s->m2->velocity;
        }

        for (auto &m : masses)
        {
            if (!m->pinned)
            {
                // TODO (Part 2): Add the force due to gravity, then compute the new velocity and position

                m->forces += gravity * m->forces;
                Vector2D a = m->forces / m->mass; 
                m->velocity += delta_t * a;
                m->position += delta_t * m->velocity; 
                // TODO (Part 2): Add global damping

            }

            // Reset all forces on each mass
            m->forces = Vector2D(0, 0);
        }
    }

       void Rope::simulateVerlet(float delta_t, Vector2D gravity)
    {
        for (auto &s : springs)
        {
            // TODO (Part 3): Simulate one timestep of the rope using explicit Verlet （solving constraints)
            float length = (s->m2->position - s->m1->position).norm();
            Vector2D dis = s->m2->position - s->m1->position;
            Vector2D force = s->k * (length - s->rest_length) * dis / length;
            s->m1->forces += force;
            s->m2->forces -= force;
        }

        for (auto &m : masses)
        {
            if (!m->pinned)
            {
                Vector2D temp_position = m->position;
                // TODO (Part 3.1): Set the new position of the rope mass
                m->forces += gravity;
                Vector2D pos = m->position;
                
                //m->position += (m->position - m->last_position) + m->forces / m->mass * delta_t * delta_t;
                // TODO (Part 4): Add global Verlet damping
                m->position += (1 - 0.00005) * (m->position - m->last_position) + m->forces / m->mass * delta_t * delta_t;
                m->last_position = pos;
            }
            // Reset all forces on each mass
            m->forces = Vector2D(0, 0);
        }
    }

}
