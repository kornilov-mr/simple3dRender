#include <GL/freeglut.h>
#include <ctime>
#include <chrono>
#include <stdio.h>
#include <math.h>
#include <unistd.h>
#include "vecfun.h"
#include <iostream>

using std::chrono::duration_cast;
using std::chrono::milliseconds;
using std::chrono::seconds;
using std::chrono::system_clock;
int width=400;
int height=400;
float huntforpixelcolor(int i ,int j,int count_rotate,vec3 spherePos,vec3 light){
    vec2 uv = vec2(i, j) / vec2(width, height) * 2.0f - 1.0f;
    vec3 ro = vec3(-6, 0, 0);
    vec3 rd = norm(vec3(2, uv));
    ro = rotateY(ro, 0.2);
    rd = rotateY(rd, 0.2);
    ro = rotateZ(ro, count_rotate * 0.03);
    rd = rotateZ(rd, count_rotate * 0.03);
    float diff = 1;
    for (int k = 0; k < 5; k++) {
        float minIt = 99999;
        vec2 intersection = sphere(ro - spherePos, rd, 1);
        vec3 n = 0;
        float albedo = 1;
        if (intersection.x > 0) {
            vec3 itPoint = ro - spherePos + rd * intersection.x;
            minIt = intersection.x;
            n = norm(itPoint);
        }
        vec3 boxN = 0.5;
        intersection = box(ro, rd, 1, boxN);
        if (intersection.x > 0 && intersection.x < minIt) {
            minIt = intersection.x;
            n = boxN;
        }
                    intersection = plane(ro, rd, vec3(0, 0, -1), 1);
                    if (intersection.x > 0 && intersection.x < minIt) {
                        minIt = intersection.x;
                        n = vec3(0, 0, -1);
                        albedo = 0.5;
                    }
        if (minIt < 99999) {
            diff *= (dot(n, light) * 0.25 + 0.5) * albedo;
            ro = ro + rd * (minIt - 0.01);
            rd = reflect(rd, n);
        }
        else break;
    }
    return diff;

}
[[noreturn]] void paintpoint(){
    glPointSize(1.0);
    int max_fps=60;
    int count_rotate=0;
    HDC hdc = GetDC(GetConsoleWindow());
    int xo=width/2;
    int yo=height/2;
    while(true){
        count_rotate+=1;
        vec3 light = norm(vec3(-0.5, 0.5, -1.0));
        vec3 spherePos = vec3(0, 3, 0);
        auto start = duration_cast<seconds>(system_clock::now().time_since_epoch()).count();
        for(int i =0;i<width;i++){
            for(int j =0;j<height;j++){
                float diff=huntforpixelcolor(i ,j,count_rotate,spherePos,light);
                float color_pix=(float)diff;

                double x=double(i-xo)/double(xo);
                double y=-1*double(j-yo)/double(yo);
//                std::cout<<"i:"<<x<<" j:"<<float(j/height)<<" color:"<<diff<<std::endl;
                glColor3f(color_pix,color_pix,color_pix);
                glBegin(GL_POINTS);
                glVertex2d(x,y);


            }
        }
        glEnd();
        glutSwapBuffers();
        glClear(GL_COLOR_BUFFER_BIT);
        auto end = duration_cast<seconds>(system_clock::now().time_since_epoch()).count();
        if (end-start<=1000/60){
            //usleep(end-start);
        }
    }

}
void display(){
    glClear(GL_COLOR_BUFFER_BIT);
    glPointSize(10.0);
    glColor3f(1.0,0.0,0.0);
    glLineWidth(10);
    glBegin(GL_POINTS);
    glVertex2d(-0.5, -0.5);
    glVertex2d(0.5, -0.5);
    glEnd();
    glutSwapBuffers();
}
int main(int argc, char **argv){
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE);
    glutInitWindowSize(width,height);
    glutInitWindowPosition(200, 200);
    glutCreateWindow("Window");
    glClearColor(1, 1, 1, 0);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-10, 10, -10, 10, 0, 0);
    glutDisplayFunc(paintpoint);
    glutMainLoop();
    return 0;
}