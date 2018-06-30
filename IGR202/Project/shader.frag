// ----------------------------------------------
// Informatique Graphique 3D & Réalité Virtuelle.
// Travaux Pratiques
// Shaders Normal
// Copyright (C) 2015 Tamy Boubekeur
// All rights reserved.
// ----------------------------------------------

// Add here all the values you need to describe the lights or the materials. 
// At first used const values, eventually stored in structures.
// Then, use uniform variables and set them from your CPU program using 
// the GLProgram methods.
const float PI = 3.1415926535897932384626433832795;
uniform vec3 lightPos;
uniform  bool showNPR;

uniform float alpha; // rugosité du materiaux 
uniform float f0; //indice de réfraction de fresnel
uniform float kd; // Coefficient Diffus
uniform bool show_shadow ,show_AO;
uniform bool simplify;

const vec3 matAlbedo = vec3 (0.7, 0.6, 0.6);
varying vec4 P; // Interpolated fragment-wise position
varying vec3 N; // Interpolated fragment-wise normal
varying vec4 C; // Interpolated fragment-wise color


void main (void) {
    gl_FragColor = vec4 (0.0, 0.0, 0.0, 1.0);
    
    vec3 p = vec3 (gl_ModelViewMatrix * P);
    vec3 n = normalize (gl_NormalMatrix * N);
    //vec3 l = normalize (lightPos - p);
    vec3 v = normalize (-p); // wo

    vec3 lightPosInCamSpace = vec3 (gl_ModelViewMatrix * vec4(lightPos,1));
    vec3 l = normalize (lightPosInCamSpace - p); // wi
    //vec3 l = normalize (lightPos - p);
    // ---------- Code to change -------------
    /***********************  Le modèle GGX *************************/
    vec3 wh = normalize(l + v);
    float dist = length(lightPosInCamSpace - p);   
 	float fd = kd / PI;
    float D_ggx = alpha * alpha / 
    			(PI * pow(1.0 + (alpha * alpha-1.0 ) * dot(n,wh) * dot(n,wh), 2.0));

    float F = f0 + (1.0-f0)*pow(1.0 - max(0.0, dot(l,wh)),5.0);
    float G_smith_in = 2.0 * max(dot(n,l),0.0)/(( max(dot(n,l),0.0) + sqrt(alpha*alpha + (1.0 - alpha*alpha)*max(dot(n,l),0.0)*max(dot(n,l),0.0))));
    float G_smith_out =  2.0 * max(dot(n,v),0.0)/(( max(dot(n,v),0.0) + sqrt(alpha*alpha + (1.0 - alpha*alpha)*max(dot(n,v),0.0)*max(dot(n,v),0.0))));
    float G_ggx = G_smith_in * G_smith_out;

    vec3 colorReponse = max( dot(l, n),0.0) * matAlbedo * (D_ggx * F * G_ggx + fd);


    if(show_shadow){
    	colorReponse *= C.w;
    }

    if(show_AO){
    	colorReponse *= C.z;
    }

    vec4 color = vec4(colorReponse, 1.0);
    // ----------------------------------------
    
    gl_FragColor += color;
   // gl_FragColor = C.w * vec4(1.0,1.0,1.0,1.0);

}
