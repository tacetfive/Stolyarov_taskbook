#include <cstdio>
#include <math.h>

class Body {
    double dens;
public:
    Body(double this_dens) { dens = this_dens; }
    virtual double Volume() const = 0;
    double Mass() const
    {
        return (dens * Volume());
    }
};

class Cube : public Body {
    double edge;
public:                                     // | "this_dens" type not needed!
    Cube(double this_edge, double this_dens) : Body(this_dens)
    { edge = this_edge; }
    virtual double Volume() const { return edge*edge*edge; }
};

class Tetrahedron : public Body {
    double edge;
public:
    Tetrahedron(double this_edge, double this_dens) : Body(this_dens)
    { edge = this_edge; }
    virtual double Volume() const { return M_SQRT2 / 12 * edge*edge*edge; }
};

int main()
{
    const Body *p, *q, *r; // if remove "const" Weights and Vols will be 0.000
    Cube a(2, 10), b(5, 0.1);
    Tetrahedron t(6, 2.5);
    p = &a; q = &b; r = &t;
    printf("Volumes: %3.3f %3.3f %3.3f\n",
           p->Volume(), q->Volume(), r->Volume());
    printf("Weights: %3.3f %3.3f %3.3f\n",
           p->Mass(), q->Mass(), r->Mass());
    return 0;
}
