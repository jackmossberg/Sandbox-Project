#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define API extern

#define PI 3.14f
#define MAT4_INIT { 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f};

typedef float vec2[2];
typedef float vec3[3];
typedef float vec4[4];

typedef float matrix3x3[9];
typedef float matrix4x4[16];

API float degs_to_rads(float degrees);
API float rads_to_degs(float rads);

API void mapi_Vec2Add(float* v1, float* v2, float* result);
API void mapi_Vec3Add(float* v1, float* v2, float* result);
API void mapi_Vec4Add(float* v1, float* v2, float* result);

API void mapi_Matrix3x3Fill(float* matrix3x3, float val);
API void mapi_Matrix4x4Fill(float* matrix4x4, float val);
API void mapi_Matrix5x5Fill(float* matrix5x5, float val);

API void mapi_TransformMatrix4x4(float* matrix4x4, float* position, float* rotation, float* scale);
API void mapi_ViewMatrix4x4(float* matrix4x4, float* position, float* rotation);
API void mapi_ProjectionMatrix4x4(float* matrix4x4, float fovy, float width, float height);