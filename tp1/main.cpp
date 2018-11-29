#include <cmath>
#include <iostream>
#include <string>

#include "Vector.hpp"
#include "Line.hpp"
#include "Geometry.hpp"
#include "Sphere.hpp"
#include "Image.hpp"
#include "Color.hpp"
#include "Intersection.hpp"
#include "Material.hpp"

#include "Scene.hpp"

using namespace std;
using namespace rt;

double imageToViewPlane(int n, int imgSize, double viewPlaneSize) {
    double u = (double)n*viewPlaneSize / (double)imgSize;
    u -= viewPlaneSize / 2;
    return u;
} 

const Intersection findFirstIntersection(const Line& ray,
                                         double minDist, double maxDist) {
    Intersection intersection;
    
    for (int i = 0; i < geometryCount; i++) {
        Intersection in = scene[i]->getIntersection(ray, minDist, maxDist);
        if (in.valid()) {
            if (!intersection.valid()) {
                intersection = in;
            }
            else if (in.t() < intersection.t()) {
                intersection = in;
            }
        }
    }
    
    return intersection;
}

int main() {
    Vector viewPoint(0, 0, 0);
    Vector viewDirection(0, 0, 1);
    Vector viewUp(0, -1, 0);
    
    double frontPlaneDist = 0;
    double backPlaneDist = 1000;
    
    double viewPlaneDist = 512;
    double viewPlaneWidth = 1024;
    double viewPlaneHeight = 768;
    
    int imageWidth = 1024;
    int imageHeight = 768;
    
    Vector viewParallel = viewUp^viewDirection;
    
    viewDirection.normalize();
    viewUp.normalize();
    viewParallel.normalize();
    
    Image image(imageWidth, imageHeight);
    
    for (int i = 0; i < imageWidth; i++) {
        for (int j = 0; j < imageHeight; j++) {
            Vector xOne = viewPoint + viewDirection * viewPlaneDist + viewUp * imageToViewPlane(j, imageHeight, viewPlaneHeight) + viewParallel * imageToViewPlane(i, imageWidth,  viewPlaneWidth);
            
            Line xray(viewPoint, xOne, false);
            Intersection intersection = findFirstIntersection(xray, frontPlaneDist, backPlaneDist);
            
            if(intersection.valid())
            {
                //double radius = ((Sphere *)intersection.geometry())->radius();
                
                Vector L = lights[1]->position();
                Vector C = viewPoint;
                Vector V = intersection.vec();
                Vector E = (C - V);
                // A Vérifier
                Vector N = (V - ((Sphere *)intersection.geometry())->center()) + V;
                Vector T = L - V;
                Vector R = N * (N * T) * 2 - T;
                
                E.normalize();
                N.normalize();
                T.normalize();
                R.normalize();
                
                Color colorSphere = intersection.geometry()->material().ambient() * lights[0]->ambient();
                
                if (N * T > 0) {
                    colorSphere += intersection.geometry()->material().diffuse() * lights[0]->diffuse() * (N * T);
                }
                if (E * R > 0) {
                    colorSphere += intersection.geometry()->material().specular() * lights[0]->specular() * pow(E*R, intersection.geometry()->material().shininess());
                }
                
                colorSphere *= lights[0]->intensity();
                
                /********************************TODO********************************/
                // E : vector from the intersection point to the camera
                // T : vector from the intersection point to the light
                // N : normal to the surface at the intersection point
                // R : reflection vector R = N * (N*T) * 2 - T
                
                // if (normal * T) > 0 ---> Color += diffuseM * diffuseL * (N*T)
                // if (E * R) > 0 ---> Color += specularM * specularL * pow(E*R, shininessM)
                
                //newColorSphere*lights[0]->intensity();
                //newColorSphere*lights[1]->intensity();
                /********************************************************************/
                
                /********************************TEST********************************/
                //cout<<"Light + ColorMat R : "<<lights[0]->ambient().red()*lights[1]->ambient().red()*intersection.geometry()->material().ambient().red()<<endl;
                //cout<<"Light + ColorMat G : "<<lights[0]->ambient().green()*lights[1]->ambient().green()*intersection.geometry()->material().ambient().green()<<endl;
                //cout<<"Light + ColorMat B : "<<lights[0]->ambient().blue()*lights[1]->ambient().blue()*intersection.geometry()->material().ambient().blue()<<endl;
                
                //cout<<"Pixel : "<<i<<" "<<j<<" Couleur R: "<<intersection.geometry()->material().ambient().red()<<endl;
                /********************************************************************/
                
                image.setPixel(i,j,colorSphere);
            }
        }
    }
    
    image.store("newScene.png");
    
    
    /*for (int i = 0; i < geometryCount; i++) {
        delete scene[i];
    }
    */
    return 0;
}

