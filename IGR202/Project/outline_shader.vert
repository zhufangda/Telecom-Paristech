// ----------------------------------------------
// Informatique Graphique 3D & Réalité Virtuelle.
// Travaux Pratiques
// Outline Shaders
// Copyright (C) 2015 Tamy Boubekeur
// All rights reserved.
// ----------------------------------------------

varying vec4 P; // Per-vertex position
varying vec3 N; // Per-vertex normal
varying vec4 C; // Per-vertex color


const float outline_width = 0.003;
void main(void) {
    P = gl_Vertex;
    N = gl_Normal;
    C = gl_Color;
    gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * vec4(gl_Vertex.xyz + gl_Normal.xyz * outline_width, 1.0);

}
