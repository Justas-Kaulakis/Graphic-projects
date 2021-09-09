#ifndef FLUID_H_INCLUDED
#define FLUID_H_INCLUDED

#include <iostream>
#include <SFML/Graphics.hpp>

#define IX(i, j) std::max(0, std::min(i, N)) + (N+2) * std::max(0, std::min(j, N))

enum DrawProp {VELOCITY = 0, DENSITY = 1};

class Fluid : public sf::Drawable {
private:
    int N;
    int scale;
    float drawCap;
    float diff;
    float visc;
    float *s;
    float *density;

    float *Vx;
    float *Vy;

    float *Vx0;
    float *Vy0;

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        for(int i = 1; i < N-1; i++) {
            for(int j = 1; j < N-1; j++) {
                int x = (i-1) * scale;
                int y = (j-1) * scale;

                sf::RectangleShape r(sf::Vector2f((float)scale, (float)scale));

                if(drawProp == DrawProp::DENSITY) {
                    float d = density[IX(i, j)];
                    if(d < drawCap) continue;
                    r.setPosition((float)x, (float)y);
                    r.setFillColor(sf::Color(d, d, d));
                } else {
                    float vx = Vx[IX(i, j)];
                    float vy = Vy[IX(i, j)];
                    //if(vx + vy < 0.5f) continue;
                    r.setPosition((float)x, (float)y);
                    r.setFillColor(sf::Color(vx, 0, vy));
                }

                target.draw(r);
            }
        }
    }

    void set_bnd(int b, float *x)
    {
        for(int i = 1; i < N - 1; i++) {
            x[IX(i, 0  )] = b == 2 ? -x[IX(i, 1  )] : x[IX(i, 1  )];
            x[IX(i, N-1)] = b == 2 ? -x[IX(i, N-2)] : x[IX(i, N-2)];
        }

        for(int j = 1; j < N - 1; j++) {
            x[IX(0  , j)] = b == 1 ? -x[IX(1  , j)] : x[IX(1  , j)];
            x[IX(N-1, j)] = b == 1 ? -x[IX(N-2, j)] : x[IX(N-2, j)];
        }


        float amount = 0.33f;

        x[IX(0, 0)]       = amount * (x[IX(1, 0)] + x[IX(0, 1)]);
        x[IX(0, N-1)]     = amount * (x[IX(1, N-1)] + x[IX(0, N-2)]);
        x[IX(N-1, 0)]     = amount * (x[IX(N-2, 0)] + x[IX(N-1, 1)]);
        x[IX(N-1, N-1)]   = amount * (x[IX(N-2, N-1)] + x[IX(N-1, N-2)]);
    }


    void lin_solve(int b, float *x, float *x0, float a, float c)
    {
        float cRecip = 1.0 / c;
            for (int j = 1; j < N - 1; j++) {
                for (int i = 1; i < N - 1; i++) {
                    x[IX(i, j)] =
                        (x0[IX(i, j)]
                            + a*(    x[IX(i+1, j  )]
                                    +x[IX(i-1, j  )]
                                    +x[IX(i  , j+1)]
                                    +x[IX(i  , j-1)]
                            )) * cRecip;
                }
            }

            set_bnd(b, x);

    }

    void diffuse(int b, float *x, float *x0, float diff, float dt)
    {
        float a = dt * diff * (N - 2) * (N - 2);
        lin_solve(b, x, x0, a, 1 + 6 * a);
    }

    void project(float *velocX, float *velocY, float *p, float *div)
    {
            for (int j = 1; j < N - 1; j++) {
                for (int i = 1; i < N - 1; i++) {
                    div[IX(i, j)] = -0.5f*(
                             velocX[IX(i+1, j  )]
                            -velocX[IX(i-1, j  )]
                            +velocY[IX(i  , j+1)]
                            -velocY[IX(i  , j-1)]
                        )/N;
                    p[IX(i, j)] = 0;
                }
            }

        set_bnd(0, div);
        set_bnd(0, p);
        lin_solve(0, p, div, 1, 6);

            for (int j = 1; j < N - 1; j++) {
                for (int i = 1; i < N - 1; i++) {
                    velocX[IX(i, j)] -= 0.5f * (  p[IX(i+1, j)]
                                                    -p[IX(i-1, j)]) * N;
                    velocY[IX(i, j)] -= 0.5f * (  p[IX(i, j+1)]
                                                    -p[IX(i, j-1)]) * N;
                }
            }

        set_bnd(1, velocX);
        set_bnd(2, velocY);
    }

    void advect(int b, float *d, float *d0,  float *velocX, float *velocY, float dt)
    {
        float i0, i1, j0, j1;

        float dtx = dt * (N - 2);
        float dty = dt * (N - 2);

        float s0, s1, t0, t1;
        float tmp1, tmp2, x, y;

        float Nfloat = N;
        float ifloat, jfloat;
        int i, j;

            for(j = 1, jfloat = 1; j < N - 1; j++, jfloat++) {
                for(i = 1, ifloat = 1; i < N - 1; i++, ifloat++) {
                    tmp1 = dtx * velocX[IX(i, j)];
                    tmp2 = dty * velocY[IX(i, j)];
                    x    = ifloat - tmp1;
                    y    = jfloat - tmp2;

                    if(x < 0.5f) x = 0.5f;
                    if(x > Nfloat + 0.5f) x = Nfloat + 0.5f;
                    i0 = floorf(x);
                    i1 = i0 + 1.0f;
                    if(y < 0.5f) y = 0.5f;
                    if(y > Nfloat + 0.5f) y = Nfloat + 0.5f;
                    j0 = floorf(y);
                    j1 = j0 + 1.0f;

                    s1 = x - i0;
                    s0 = 1.0f - s1;
                    t1 = y - j0;
                    t0 = 1.0f - t1;

                    int i0i = i0;
                    int i1i = i1;
                    int j0i = j0;
                    int j1i = j1;

                    d[IX(i, j)] =

                        s0 * ( t0 * d0[IX(i0i, j0i)] + t1 * d0[IX(i0i, j1i)])
                       +s1 * ( t0 * d0[IX(i1i, j0i)] + t1 * d0[IX(i1i, j1i)]);
                }
            }

        set_bnd(b, d);
    }

public:
    DrawProp drawProp;
    Fluid(int size, int scale, float drawCap, int diffusion, int viscosity, DrawProp prop = DrawProp::DENSITY) :
        N(size + 2), scale(scale), drawCap(drawCap), diff(diffusion), visc(viscosity),
        drawProp(prop)
    {
        int myN = N + 2;

        this->s = new float[myN*myN]();

        this->density = new float[myN*myN]();

        this->Vx = new float[myN*myN]();
        this->Vy = new float[myN*myN]();

        this->Vx0 = new float[myN*myN]();
        this->Vy0 = new float[myN*myN]();
    }

    ~Fluid() {
        delete[] this->s;
        delete[] this->density;
        delete[] this->Vx;
        delete[] this->Vy;
        delete[] this->Vx0;
        delete[] this->Vy0;
    }

    void addDensity(int x, int y, float amount) {
        this->density[IX(x, y)] += amount;
        this->density[IX(x, y)] = std::min(255.f, this->density[IX(x, y)]);
    }
    void addDensity(sf::Vector2i pos, float amount) {
        addDensity(pos.x, pos.y, amount);
    }

    void addVelocity(int x, int y, float amountX, float amountY) {
        this->Vx[IX(x, y)] += amountX;
        this->Vy[IX(x, y)] += amountY;
        //this->Vx[IX(x, y)] = std::min(255, this->Vx[IX(x, y)])
    }
    void addVelocity(sf::Vector2i pos, float amountX, float amountY) {
        addVelocity(pos.x, pos.y, amountX, amountY);
    }

    void fade(float amount) {
        for(int i = 0; i < N; i++) {
            for(int j = 0; j < N; j++) {
                density[IX(i, j)] = std::max(density[IX(i, j)] - amount, 0.0f);
            }
        }
    }

    void step(float dt)
    {
        diffuse(1, Vx0, Vx, visc, dt);
        diffuse(2, Vy0, Vy, visc, dt);

        project(Vx0, Vy0, Vx, Vy);

        advect(1, Vx, Vx0, Vx0, Vy0, dt);
        advect(2, Vy, Vy0, Vx0, Vy0, dt);

        project(Vx, Vy, Vx0, Vy0);

        diffuse(0, s, density, diff, dt);
        advect(0, density, s, Vx, Vy, dt);
    }
};



#endif // FLUID_H_INCLUDED
