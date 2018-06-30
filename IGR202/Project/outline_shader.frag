#version 120
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

uniform vec3 lightPos = vec3 (5.0, 5.0, 5.0);
const vec3 matAlbedo = vec3 (0.6, 0.6, 0.6);

varying vec4 P; // Interpolated fragment-wise position
varying vec3 N; // Interpolated fragment-wise normal
varying vec4 C; // Interpolated fragment-wise color

void main (void) {
    gl_FragColor = vec4 (0.0, 0.0, 0.0, 1.0);
    
}
 
