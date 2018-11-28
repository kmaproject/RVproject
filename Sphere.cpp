#include "Sphere.hpp"

using namespace rt;

Intersection Sphere::getIntersection(const Line& line, double minDist, double maxDist) {
    Intersection in;
    
    Vector dx = line.dx();
    Vector x0 = line.x0();
    Vector center = this->center();
    double s1 = dx*dx;
    double s2 = 2*((x0 - center)*dx);
    double s3 =(x0-center)*(x0-center) - pow(this->radius(),2);
    int delta = pow(s2,2) - 4*s1*s3;
    if(delta<0)
    {
        return in;
    }
    else if(delta ==0)
    {
        double t = (- s2 + sqrt(delta)) / (2*s1);
        Intersection intersection(true, this, &line, t);
        in = intersection;
        return in;
        
    }
    else
    {
        double t1 = (- s2 - sqrt(pow(s2,2) - 4*s1*s3)) / (2*s1);
        double t2 = (- s2 + sqrt(pow(s2,2) - 4*s1*s3)) / (2*s1);
        if(t1 < minDist || t1 > maxDist)
        {
            Intersection intersection(true, this, &line, t2);
            return  intersection;
        }
        else if( t2 < minDist || t2 > maxDist)
        {
            Intersection intersection(true, this, &line, t1);
            return  intersection;
        }
        else if ( t1 < t2)
        {
            Intersection intersection(true, this, &line, t1);
            return  intersection;
        }
        else {
            Intersection intersection(true, this, &line, t2);
            return  intersection;
        }
        
    }
}


const Vector Sphere::normal(const Vector& vec) const {
    Vector n = vec - _center;
    n.normalize();
    return n;
}
