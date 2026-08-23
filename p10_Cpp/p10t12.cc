#include <cstdio>

class Prism {
    double height;
public:
    Prism(double this_height) { height = this_height; }
    virtual double Square() const = 0;
    double Volume() const
    {
        return (height * Square());
    }
    virtual ~Prism() {}
};

class Box : public Prism {
    double edge;
public:
    Box(double this_height, double this_edge) : Prism(this_height)
    {
        edge = this_edge; 
    }
    Box(double this_edge) : Prism(this_edge)
    {
        edge = this_edge; 
    }
    virtual double Square() const { return edge*edge; }
    virtual ~Box() {}
};

class Cube : public Box { 
    double edge;
public:
    Cube(double this_edge) : Box(this_edge) 
    {
        edge = this_edge;
    }
    ~Cube() {}
};

int main()
{
    const Prism *p, *q, *r;
    Box a(0.5, 2), b(5, 0.2);
    Cube c(0.5);
    p = &a; q = &b; r = &c;
    printf("Squares: %3.3f %3.3f %3.3f\n",
           p->Square(), q->Square(), r->Square());
    printf("Volumes: %3.3f %3.3f %3.3f\n",
           p->Volume(), q->Volume(), r->Volume());
    return 0;
}

/* 
 * Squares: 4.000 0.040 0.250
 * Volumes: 2.000 0.200 0.125
 * */
