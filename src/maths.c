#include "maths.h"

#define _FL "maths.c"

#define APIC static

float degs_to_rads(float degrees) {
    return degrees * (PI/180);
}

float rads_to_degs(float rads) {
    return rads * (180/PI);
}

void mapi_Vec2Add(float* v1, float* v2, float* result) {
    if (!v1 || !v2 || !*v1 || !*v2)
        return;
    
    result[0] = v1[0] + v2[0];
    result[1] = v1[1] + v2[1];
}

void mapi_Vec3Add(float* v1, float* v2, float* result) {
    if (!v1 || !v2 || !*v1 || !*v2)
        return;
    
    result[0] = v1[0] + v2[0];
    result[1] = v1[1] + v2[1];
    result[2] = v1[2] + v2[2];
}

void mapi_Vec4Add(float* v1, float* v2, float* result) {
    if (!v1 || !v2 || !*v1 || !*v2)
        return;
    
    result[0] = v1[0] + v2[0];
    result[1] = v1[1] + v2[1];
    result[2] = v1[2] + v2[2];
    result[3] = v1[3] + v2[3];
}

void mapi_Matrix3x3Fill(float* matrix3x3, float val) {
    if (!matrix3x3)
        return;

    for(int i = 0; i < (3 * 3); i++) {
        matrix3x3[i] = val;
    }
}

void mapi_Matrix4x4Fill(float* matrix4x4, float val) {
    if (!matrix4x4)
        return;

    for(int i = 0; i < (4 * 4); i++) {
        matrix4x4[i] = val;
    }
}

void mapi_Matrix5x5Fill(float* matrix5x5, float val) {
    if (!matrix5x5)
        return;

    for(int i = 0; i < (5 * 5); i++) {
        matrix5x5[i] = val;
    }
}

// transform 4x4 matrices for graphics rendering purposes...

void mapi_TransformMatrix4x4(float* matrix4x4, float* position, float* rotation, float* scale) {
    if (!matrix4x4 || !position || !rotation || !scale)
        return;
    
}

void mapi_ViewMatrix4x4(float* matrix4x4, float* position, float* rotation) {
    if (!matrix4x4 || !position || !rotation)
        return;
    
}

void mapi_ProjectionMatrix4x4(float* matrix4x4, float fovy, float width, float height) {
    if (!matrix4x4)
        return;
}