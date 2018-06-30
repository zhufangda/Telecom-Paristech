// ----------------------------------------------
// Informatique Graphique 3D & Réalité Virtuelle.
// Travaux Pratiques
// Shaders
// Copyright (C) 2015 Tamy Boubekeur
// All rights reserved.
// ----------------------------------------------

// Add here all the values you need to describe the lights or the materials. 
// At first used const values, eventually stored in structures.
// Then, use uniform variables and set them from your CPU program using 
// the GLProgram methods.

const vec3 lightPos = vec3 (5.0, 5.0, 5.0);
const vec3 matAlbedo = vec3 (0.6, 0.6, 0.6);

varying vec4 P; // Interpolated fragment-wise position
varying vec3 N; // Interpolated fragment-wise normal
varying vec4 C; // Interpolated fragment-wise color

void main (void) {
    gl_FragColor = vec4 (0.0, 0.0, 0.0, 1.0);
    
    vec3 p = vec3 (gl_ModelViewMatrix * P);
    vec3 n = normalize (gl_NormalMatrix * N);
    vec3 l = normalize (lightPos - p);
    vec3 v = normalize (-p);
    
    // ---------- Code to change -------------
    vec4 color = C;
    // ----------------------------------------
    
    gl_FragColor += color;
}
 
