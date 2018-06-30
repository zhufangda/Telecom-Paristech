// --------------------------------------------------------------------------
// Copyright(C) 2009-2016
// Tamy Boubekeur
// 
// Permission granted to use this code only for teaching projects and 
// private practice.
//
// Do not distribute this code outside the teaching assignements.                                                                           
// All rights reserved.                                                       
// --------------------------------------------------------------------------
#define _USE_MATH_DEFINES
#include <GL/glew.h>
#include <GL/glut.h>
#include <iostream>
#include <vector>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <algorithm>
#include <cmath>
#include <ctime>
#include "LightSource.h"
#include "Vec3.h"
#include "Camera.h"
#include "Mesh.h"
#include "GLProgram.h"
#include "Exception.h"
#include <algorithm>
#include "Ray.h"
#include <cstdlib>
#include <ctime>
#include "BVH.h"
#include "Mat4.h"
#include "AxisAlignedBoundingBox.h"



static float make_light = 0;
using namespace std;

static const unsigned int DEFAULT_SCREENWIDTH = 1024;
static const unsigned int DEFAULT_SCREENHEIGHT = 768;
static const string DEFAULT_MESH_FILE ("mesh_collection/double-torus.off");
static const char * modelFilename;

static const string appTitle ("Informatique Graphique & Realite Virtuelle - Travaux Pratiques - Algorithmes de Rendu");
static const string myName ("ZHU Fangda");
static GLint window;
static unsigned int FPS = 0;
static bool fullScreen = false;

static Camera camera;
static Mesh mesh;
static Mesh contoursMesh;
GLProgram * normalProgram;
GLProgram * aabbProgram;
GLProgram * nprProgram;
GLProgram * glProgram;
GLProgram * outlineProgram;


BVH* bvh;
static unsigned deep = 3;
static std::vector<float> colorResponses; // Cached per-vertex color response, updated at each frame
/** Add a plan in the model**/
static void createPlan(float, float, float,float, float, float);
static int transIndice(int i, int j, int row);
void key(unsigned char keyPressed, int x, int y);
void initShaderValue();
static double ds_ggx(float alpha, Vec3f normal, Vec3f wh);
static double term_fresnel(double f0, Vec3f wi, Vec3f wh);
static double term_geo_ct(Vec3f normal, Vec3f wh, Vec3f wi, Vec3f wo);
static double g_smith(double alpha, Vec3f normal, Vec3f w);
static double term_geo_ggx(double alpha, Vec3f normal, Vec3f wh, Vec3f wi, Vec3f wo);
static void computePerVertexShadow();
static bool rayTest(const Ray& ray, const Mesh& mesh);
static void createBVH();
static void drawBox(float width, int deep);
void init(const char * modelFilename);
std::vector<string> meshFileList;
void initMeshVextex();
int meshIndex = 0;


static int nbOfRayForAO = 20;
static float testRadius = 0.1f;
static float testAngle = M_PI / 4.0f;
static float alpha = 0.08f; // rugosité du materiaux [0,1]
static float f0 = 0.17f; //indice de réfraction de fresnel
static float kd = 3.7f; //
static bool showAO = false, showShadow = false, showNPR = false;
static LightSource lightCamera;

//LightSource light2 = LightSource(0.0f, 1.5f, 0.0f, 0.8f, 0.8f, 0.8f);
//LightSource light3 = LightSource(0.0f, 0.0f, -1.0f, 0.8f, 0.8f, 0.8f);
//LightSource light4 = LightSource(0.0f, 0.0f, 1.0f, 0.6f, 0.6f, 0.8f);
//LightSource light5 = LightSource(0, 0, 1.0f, 0.5f, 0.5f, 1.0f);

/*
static void createContoursMesh(float scale_x, float scale_y, float scale_z){
	Mat4f scale_matrix = Mat4f::scale(Vec3f(scale_x, scale_y, scale_z));
	contoursMesh = mesh;
	cout << contoursMesh.positions()[0] << std::endl;

	vector<Vec3f>& positions = contoursMesh.positions();

	for (int i = 0; i < positions.size(); i++) {
		positions[i] = scale_matrix * positions[i];
	}

	cout << contoursMesh.positions()[0] << std::endl;
	
}*/



static void createBVH() {
	std::vector<Triangle*> triangles_ptr;
	vector<Triangle>& triangles = mesh.triangles();


	cout << "Create BVH......." << endl;
	for (int i = 0; i < mesh.triangles().size(); i++) {
		triangles_ptr.push_back(&triangles[i]);
	}

	time_t start, end;
	start = time(nullptr);
	bvh = BVH::buildBVH2(mesh, triangles_ptr,0);
	end = time(nullptr);
	cout << "\tTriangles number:" << bvh->total_nbr_tri << std::endl;
	cout << "\tFinished with " << end - start << " sec" << std::endl;

}


static void drawBox(float width, int deep) {
	aabbProgram->use();
	bvh->drawBVH(2.5, deep);
}


static void initUniformeValue(GLProgram* InputglProgram) {
	lightCamera.setPoisiton(0.0f, 5.0f, -5.0f);
	cout << lightCamera.getPosition() << std::endl;
	InputglProgram->use();
	InputglProgram->setUniform1f("kd", kd);
	InputglProgram->setUniform1f("f0", f0);
	InputglProgram->setUniform1f("alpha", alpha);
	InputglProgram->setUniform1f("kd", kd);
		InputglProgram->setUniform1f("f0", f0);
		InputglProgram->setUniform1f("alpha", alpha);
		InputglProgram->setUniform1i("show_shadow", showShadow);
		InputglProgram->setUniform1i("show_AO", showAO);
		InputglProgram->setUniform3f("lightPos", lightCamera.getPosition()[0],
			lightCamera.getPosition()[1],
			lightCamera.getPosition()[2]);
}
void printUsage() {
	std::cerr << std::endl
		<< appTitle << std::endl
		<< "Author: " << myName << std::endl << std::endl
		<< "Usage: ./main [<file.off>]" << std::endl
		<< "Commands:" << std::endl
		<< "------------------" << std::endl
		<< " ?: Print help" << std::endl
		<< " w: Toggle wireframe mode" << std::endl
		<< " <drag>+<left button>: rotate model" << std::endl
		<< " <drag>+<right button>: move model" << std::endl
		<< " <drag>+<middle button>: zoom" << std::endl
		<< " s: switch for the shadow" << std::endl
		<< " a: switch for the AO" << std::endl
		<< " d,D: change kd(Coefficient Diffus)" << std::endl
		<< " r,R: change F0(indice de réfraction de fresnel)" << std::endl
		<< " m,M: change alpha(rugosité du materiaux )" << std::endl
		<< " z,Z: change the number of ray tested for AO" << std::endl
		<< " e,E: change the ray radius for AO test" << std::endl
		<< " t,T: change the angle of ray for AO test" << std::endl
		<< " n: switch shader between normal shader and NPR shader" << std::endl
		<< " 0: show original mesh" << std::endl
		<< " 1: show topologicial filter with alpha = 0.1 " << std::endl
		<< " 2: show topologicial filter with alpha = 0.5 " << std::endl
		<< " 3: show topologicial filter with alpha = 1.0 " << std::endl
		<< " 4: show geometrical filter" << std::endl
		<< " 5: show simplify mesh with resolution 16*16*16" << std::endl
		<< " 6: show simplify mesh with resolution 32*32*32" << std::endl
		<< " 7: show simplify mesh with resolution 64*64*64" << std::endl
		<< " 8: show subdivision mesh" << std::endl
		<< " 9: show simplify mesh with octree" << std::endl
		<< " ->, <-: switch model" << std::endl
		<< " q, <esc>: Quit" << std::endl << std::endl;
}


void init(const char * modelFilename) {
	glewExperimental = GL_TRUE;
	glewInit(); // init glew, which takes in charges the modern OpenGL calls (v>1.2, shaders, etc)
	glCullFace(GL_BACK);     // Specifies the faces to cull (here the ones pointing away from the camera)
	glEnable(GL_CULL_FACE); // Enables face culling (based on the orientation defined by the CW/CCW enumeration).
	glDepthFunc(GL_LESS); // Specify the depth test for the z-buffer
	glEnable(GL_DEPTH_TEST); // Enable the z-buffer in the rasterization
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glEnable(GL_NORMALIZE);
	glLineWidth(2.0); // Set the width of edges in GL_LINE polygon mode
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Background color

	
	try {
		mesh.loadOFF(modelFilename);
	}
	catch (std::runtime_error const& e) {
		std::cout << e.what() << std::endl;
		exit(1);
	}
	meshFileList.push_back(modelFilename);
	meshIndex = meshFileList.size()-1;
	
	//createPlan(0.0f, -0.5, 0.f, 5.0f, 5.0f, 0.1f);
	createBVH();
	//createContoursMesh(1.1, 1.1, 1.1);

	
	camera.resize(DEFAULT_SCREENWIDTH, DEFAULT_SCREENHEIGHT);

	mesh.exportOFF("after_filter.off");


	try {
		normalProgram = GLProgram::genVFProgram("Simple GL Program", "shader.vert", "shader.frag"); // Load and compile pair of shaders
		aabbProgram = GLProgram::genVFProgram("AABB", "shader_aabb.vert", "shader_aabb.frag");
		nprProgram = GLProgram::genVFProgram("npr", "npr_shader.vert", "npr_shader.frag");
		outlineProgram = GLProgram::genVFProgram("Outline program", "outline_shader.vert", "outline_shader.frag");

		initUniformeValue(normalProgram);
		initUniformeValue(nprProgram);

		glProgram = normalProgram;
		glProgram->use();
		initMeshVextex();
	}
	catch (Exception & e) {
		cerr << e.msg() << endl;
	}

	colorResponses.resize(4 * mesh.positions().size());

}

// EXERCISE : the following color response shall be replaced with a proper reflectance evaluation/shadow test/etc.
void updatePerVertexColorResponse() {



	Vec3f wo, wi, wh, normal, position;
	float ds = 0;//distance between the light and the surface
	float attenuation = 0; //attenuation parametre
	float kd = 0.5;  // coefficient de diffu

	/*** model de blinn-Phong ***/
	float ks = 0.5; // Coefficient de specularite
	float s = 0.5; // brillance
	float fs = 0.0f; // modele de Phong modifié
	Vec3f camera_pos;



	std::vector<LightSource*> lights;
	//lights.push_back(&lightCamera);
	/*
	lights.push_back(&light2);
	lights.push_back(&light3);
	lights.push_back(&light4);
	lights.push_back(&light5);
*/
	//for (unsigned int i = 0; i < colorResponses.size(); i++) {
		//	colorResponses[i].init(0, 0, 0);
			//for (unsigned int j = 0; j < lights.size(); j++) {
				/*** model de blinn-Phong ***/
				//camera.getPos(camera_pos);
		//		wi = normalize(lights.at(j)->getPosition() - mesh.positions().at(i));
			//	wo = normalize(camera_pos - mesh.positions().at(i));
			//	wh = (wi + wo) / (wi + wo).length();

			//	normal = mesh.normals().at(i);
	//			fs = ks * pow(std::max(0.0f,dot(normal, wh)),s); 

		//		ds = (lights.at(j)->getPosition() - mesh.positions().at(i)).length();
			//	attenuation = 1.0f / (0.01f + 0.5f*ds + 0.5f * ds * ds);
				/*** model de blinn-Phong ***/
				//colorResponses[i] +=std::max(0.0f, dot(wi,normal))*lights.at(j)->getColor()*attenuation * fs;

				/** Model de micro-facettes **/
				//float face = ds_ggx(0.3, normal, wh) * term_fresnel(0.03, wi, wh) * term_geo_ggx(0.3,normal, wh, wi, wo);
				//printf("%f %f %f\n", ds_ggx(0.6, normal, wh) , term_fresnel(0.03, wi, wh) , term_geo_ct(normal, wh, wi, wo));
				//colorResponses[i] += std::max(0.0f, dot(wi, normal))*lights.at(j)->getColor() * (face+1) * attenuation;
}

bool rayTest(const Ray& ray, const Mesh& mesh) {
	bool flag = false;
	for (int j = 0; j < mesh.triangles().size(); j++) {
		flag = ray.intersectionRayonTriangle(mesh, mesh.triangles()[j]);
		if (flag)  return flag;
	}
	return flag;

}


/** Generer un rayon dans  angle indiqué de cône avec un radius specifique **/
Ray* genRay(const Vec3f& position, const Vec3f& norme, const float& radius, const float& angle){

	float x, y, z, cos_val;
	Vec3f vec;
	do {
		x = rand() % 1000/ 500.0f - 1.0f;
		y = rand() % 1000/ 500.0f - 1.0f;
		z = rand() % 1000/ 500.0f - 1.0f;
		vec.init(x, y, z);
		cos_val = dot(vec, norme) / (vec.length() * norme.length());
	} while (cos_val < 0.0  || cos_val < cos(angle));
	vec = radius * vec;
	return new Ray(position + 0.0001f*vec, vec);
}

void computePerVertexAO(unsigned int numOfSamples, float radius, float angle) {
	srand((unsigned)time(NULL));
	Ray* sample;
	int colorIndex = 2;
	float hasAO = 0.0f;
	for (unsigned int i = 0; i < mesh.positions().size(); i++) {
		float sum = 0.0f;
		for (unsigned int j = 0; j < numOfSamples; j++) {
			sample = genRay(mesh.positions().at(i),
				mesh.normals().at(i),
				radius,
				angle);
			//hasAO = rayTest(*sample, mesh) ? 0.0f : 1.0f;
			hasAO = bvh->rayCollision(mesh,*sample) ? 0.0f : 1.0f;

			float poid = dot(sample->direction, mesh.normals().at(i)) / (sample->direction.length() * mesh.normals().at(i).length());
			sum += hasAO;
			delete sample;
		}
		colorResponses[colorIndex] = sum / numOfSamples;
		
		
		// std::cout << "Index" << colorIndex << "Velue" << colorResponses[colorIndex] <<std::endl;
		colorIndex += 4;
	}
}

void computePerVertexShadow() {


	Ray ray;
	int colorIndice = 3;
	for (int i = 0; i < mesh.positions().size(); i++) {
		float shadow = 1.0f;
		ray.direction = -mesh.positions()[i] + lightCamera.getPosition();
		ray.setOrigin(mesh.positions()[i] + 0.0001f * ray.direction);//
		
		//shadow = rayTest(ray, mesh) ? 0.0f : 1.0f;

		shadow = bvh->rayCollision(mesh, ray) ? 0.0f : 1.0f;
		colorResponses[colorIndice] = shadow;
		colorIndice += 4;

	}

	
}

void renderScene () {
	colorResponses.resize(4 * mesh.positions().size());

	if (showNPR) {
		glCullFace(GL_FRONT);
		outlineProgram->use();
		glVertexPointer(3, GL_FLOAT, sizeof(Vec3f), (GLvoid*)(&(mesh.positions()[0])));
		glNormalPointer(GL_FLOAT, 3 * sizeof(float), (GLvoid*)&(mesh.normals()[0]));
		glColorPointer(4, GL_FLOAT, 0, (GLvoid*)(&(colorResponses[0])));
		glDrawElements(GL_TRIANGLES, 3 * mesh.triangles().size(), GL_UNSIGNED_INT, (GLvoid*)((&mesh.triangles()[0])));
		
	}


	glCullFace(GL_BACK);
	glProgram->use();
	
	glVertexPointer(3, GL_FLOAT, sizeof(Vec3f), (GLvoid*)(mesh.positions().data()));
	glNormalPointer(GL_FLOAT, 3 * sizeof(float), (GLvoid*)(mesh.normals().data()));
	glColorPointer(4, GL_FLOAT, 0, (GLvoid*)(&(colorResponses[0])));
	glDrawElements(GL_TRIANGLES, 3 * mesh.triangles().size(), GL_UNSIGNED_INT, (GLvoid*)(mesh.triangles().data()));


}

void createPlan(float x_offset, float y_offset, float z_offset, float width_x = 20, float width_z = 20, float width_unit = 0.1) {
	int line = (int)(width_x / (2 * width_unit)), row = (int)(width_z/(2*width_unit));
	float width = width_unit;
	int size= mesh.positions().size();
	int indice1, indice2, indice3;
	for (int i = -line; i <= line; i++) {
		for (int j = -row; j <= row; j++) {
			mesh.positions().push_back(Vec3f(j*width+x_offset, y_offset, i*width + z_offset));
		}
	}

	for (int i = 0; i < 2 * line; i++) {
		for (int j = 0; j < 2 * row; j++) {
			indice1 = transIndice(i + 1, j, 2 * row + 1);
			indice2 = transIndice(i + 1, j + 1, 2 * row + 1);
			indice3 = transIndice(i, j + 1, 2 * row + 1);
			mesh.triangles().push_back(Triangle(indice1 + size, indice2 + size, indice3 + size));


		    indice1 = transIndice(i, j, 2 * row + 1);
			indice2 = transIndice(i + 1, j, 2 * row + 1);
			indice3 = transIndice(i, j + 1, 2 * row + 1);
			mesh.triangles().push_back(Triangle(indice1 + size, indice2 + size, indice3 + size));
		}
	}
	mesh.recomputeNormals();


	mesh.exportOFF("exportincr.off");

}

int transIndice(int i, int j, int row) {
	return i*row + j;
}

void reshape(int w, int h) {
    camera.resize (w, h);
}

void display () {
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_ACCUM_BUFFER_BIT);
    camera.apply (); 
    renderScene ();
	drawBox(2.5, deep);
    glFlush ();
    glutSwapBuffers (); 
}

void specialKey(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_LEFT:
		if (meshIndex == 0) {
			meshIndex = meshFileList.size() - 1;
		}
		else {
			meshIndex--;
		}
		mesh.loadOFF(meshFileList[meshIndex]);

		std::cout << "Switch Model to " << meshFileList[meshIndex] << std::endl;
		renderScene();
		break;
	case GLUT_KEY_RIGHT:
		if (meshIndex == meshFileList.size()-1) {
			meshIndex = 0;
		}
		else {
			meshIndex++;
		}
		mesh.loadOFF(meshFileList[meshIndex]);
		std::cout << "Switch Model to " << meshFileList[meshIndex] << std::endl;
		renderScene();
		break;
	default:
		printUsage();
		break;
	}

}


void key (unsigned char keyPressed, int x, int y) {
    switch (keyPressed) {
	case '0':
		mesh.loadOFF(meshFileList[meshIndex]);
		renderScene();
		std::cout << "Reloding mesh " << meshFileList[meshIndex] << std::endl;
		break;
	case '1':
		mesh.geoFilter(0.1f);
		std::cout << "Geometry Filter with alpha = 0.1" << std::endl;
		renderScene();
		break;
	case '2':
		mesh.geoFilter(0.5f);
		std::cout << "Geometry Filter with alpha = 0.5" << std::endl;
		renderScene();
		break;
	case '3':
		mesh.geoFilter(1.0f);
		std::cout << "Geometry Filter with alpha = 1.0" << std::endl;
		renderScene();
		break;
	case '4':
		mesh.topoFilter();
		std::cout << "Topology filter mesh" << std::endl;
		renderScene();
		break;
	case '5':
		mesh.simplify(16);
		std::cout << "Simplify mesh by uniform grill 16*16*16" << std::endl;
		renderScene();
		break;
	case '6':
		mesh.simplify(32);
		std::cout << "Simplify mesh by uniform grill 32*32*32" << std::endl;
		renderScene();
		break;
	case '7':
		mesh.simplify(64);
		std::cout << "Simplify mesh by uniform grill 64*64*64" << std::endl;
		renderScene();
		break;
	case '8':
		mesh.subdivide();
		std::cout << "Subdivision of mesh" << std::endl;
		renderScene();
		break;
	case '9':
		mesh.simplifyAdaptiveMesh(25);
		std::cout << "Simplify mesh by octree" << std::endl;
		renderScene();
		break;

    case 'f':
        if (fullScreen) {
            glutReshapeWindow (camera.getScreenWidth (), camera.getScreenHeight ());
            fullScreen = false;
        } else {
            glutFullScreen ();
            fullScreen = true;
        }      
        break;
    case 'q':
    case 27:
        exit (0);
        break;
    case 'w':
        GLint mode[2];
		glGetIntegerv (GL_POLYGON_MODE, mode);
		glPolygonMode (GL_FRONT_AND_BACK, mode[1] ==  GL_FILL ? GL_LINE : GL_FILL);
        break;
        break;
	case 'D':
		kd += 0.1f;
		glProgram->setUniform1f("kd", kd);
		break;
	case 'R':
		f0 += 0.01f;
		glProgram->setUniform1f("f0", f0);
		break;
	case 'M':
		alpha = (alpha + 0.01f)>1.0f ? alpha : alpha + 0.01f;
		glProgram->setUniform1f("alpha", alpha);
		break;
	case 'd':
		kd -= 0.1f;
		glProgram->setUniform1f("kd", kd);
		break;
	case 'r':
		f0 -= 0.01f;
		glProgram->setUniform1f("f0", f0);
		break;
	case 'm':
		alpha = (alpha-0.01f)<0.0f? alpha: alpha - 0.01f;
		glProgram->setUniform1f("alpha", alpha);
		break;
	case '+':
		deep += 1;
		break;
	case '-':
		deep -= 1;
		break;
	case 's':
		showShadow = !showShadow;
		glProgram->setUniform1i("show_shadow", showShadow);
		break;
	case 'a':
		showAO  = !showAO;
		glProgram->setUniform1i("show_AO", showAO);
		break;
	case 'n':
		showNPR = !showNPR;
		if (showNPR == true) glProgram = nprProgram;
		else glProgram = normalProgram;
		break;
	case 'z':
		nbOfRayForAO -= 1;
		computePerVertexAO(nbOfRayForAO, testRadius, testAngle);
		break;
	case 'Z':
		nbOfRayForAO += 1;
		computePerVertexAO(nbOfRayForAO, testRadius, testAngle);
		break;
	case 'e':
		testRadius -= 0.01f;
		computePerVertexAO(nbOfRayForAO, testRadius, testAngle);
		break;
	case 'E':
		testRadius += 0.01f;
		computePerVertexAO(nbOfRayForAO, testRadius, testAngle);
		break;
	case 't':
		testAngle += 0.01f;
		computePerVertexAO(nbOfRayForAO, testRadius, testAngle);
		break;
    default:
		cout << keyPressed << std::endl;
        printUsage ();
        break;
    }
}

void initShaderValue() {
	glProgram->setUniform1f("kd", kd);
	glProgram->setUniform1f("f0", f0);
	glProgram->setUniform1f("alpha", alpha);
	glProgram->setUniform1f("f0", f0);
	glProgram->setUniform1i("show_shadow", showShadow);
	glProgram->setUniform1i("show_AO", showAO);
}


void mouse (int button, int state, int x, int y) {
    camera.handleMouseClickEvent (button, state, x, y);
}

void motion (int x, int y) {
    camera.handleMouseMoveEvent (x, y);
}

void idle () {
    static float lastTime = glutGet ((GLenum)GLUT_ELAPSED_TIME);
    static unsigned int counter = 0;
    counter++;
    float currentTime = glutGet ((GLenum)GLUT_ELAPSED_TIME);
    if (currentTime - lastTime >= 1000.0f) {
        FPS = counter;
        counter = 0;
        static char winTitle [128];
        unsigned int numOfTriangles = mesh.triangles ().size ();
        sprintf (winTitle, "Number Of Triangles: %d - FPS: %d - alpha: %2.2f - F0:%2.2f - kd:%2.2f - radius:%2.3f - angle:%2.3f -nbRayForAO:%d", numOfTriangles, FPS,alpha, f0, kd, testRadius, testAngle, nbOfRayForAO);
        string title = appTitle + " - By " + myName  + " - " + winTitle;
        glutSetWindowTitle (title.c_str ());
        lastTime = currentTime;
    }
    glutPostRedisplay (); 
}


int main (int argc, char ** argv) {
    if (argc > 2) {
        printUsage ();
        exit (1);
    }
    glutInit (&argc, argv);
    glutInitDisplayMode (GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
    glutInitWindowSize (DEFAULT_SCREENWIDTH, DEFAULT_SCREENHEIGHT);
    window = glutCreateWindow (appTitle.c_str ());

    
	modelFilename = (argc == 2 ? argv[1] : DEFAULT_MESH_FILE.c_str());
	init(modelFilename);

	computePerVertexAO(nbOfRayForAO, testRadius, testAngle);
	computePerVertexShadow();

    glutIdleFunc (idle);
    glutReshapeFunc (reshape);
    glutDisplayFunc (display);
    glutKeyboardFunc (key);
	glutSpecialFunc(specialKey);
    glutMotionFunc (motion);
    glutMouseFunc (mouse);
    printUsage ();  
    glutMainLoop ();
    return 0;
}


static double ds_ggx(float alpha, Vec3f normal, Vec3f wh) {
	double nor_dot_wh = dot(normal,wh);
	return alpha*alpha / (M_PI * (1 + (alpha*alpha - 1)*nor_dot_wh*nor_dot_wh));
}

static double term_fresnel(double f0, Vec3f wi, Vec3f wh){
	return f0 + (1 - f0)*pow(1 - std::max(0.0f, dot(wi, wh)),5);
}

/** Terme Géometrique Cook-Torrance*/
static double term_geo_ct(Vec3f normal, Vec3f wh, Vec3f wi, Vec3f wo) {
	double n_d_wh = dot(normal, wh);
	double n_d_wi = dot(normal, wi);
	double n_d_wo = dot(normal, wo);
	double wo_d_wh = dot(wo, wh);
	double m1 = std::min(1.0, 2 * n_d_wh*n_d_wi / wo_d_wh);
	return std::min(m1, 2 * n_d_wh*n_d_wo / wo_d_wh);
}

/** Terme Géometrique ggx*/
static double term_geo_ggx(double alpha, Vec3f normal, Vec3f wh, Vec3f wi, Vec3f wo) {
	return g_smith(alpha,normal, wi) 
		* g_smith(alpha, normal, wi) 
		* g_smith(alpha,normal, wo);
}

static double g_smith(double alpha, Vec3f normal, Vec3f w) {
	double n_d_w = dot(normal, w);
	double terme = sqrt(alpha*alpha + (1 - alpha*alpha)*n_d_w*n_d_w);
	return 2 * n_d_w / (n_d_w + terme);
}

void initMeshVextex() {
	meshFileList.push_back("mesh_collection/max_50K.off");
	meshFileList.push_back("models/bridge.off");
	meshFileList.push_back("models/double-torus.off");
	meshFileList.push_back("models/killeroo.off");
	meshFileList.push_back("models/man.off");
	meshFileList.push_back("models/monkey.off");
	meshFileList.push_back("models/rhino.off");
	meshFileList.push_back("models/sphere.off");
	meshFileList.push_back("mesh_collection/african_face.off");
	meshFileList.push_back("mesh_collection/alien.off");
	meshFileList.push_back("mesh_collection/armadillo.off");
	meshFileList.push_back("mesh_collection/beast.off");
	meshFileList.push_back("mesh_collection/beetle_tri.off");
	meshFileList.push_back("mesh_collection/big_buck_bunny.off");
	meshFileList.push_back("mesh_collection/bozbezbozzel.off");
	meshFileList.push_back("mesh_collection/camel_head.off");
	meshFileList.push_back("mesh_collection/camel_mc.off");
	meshFileList.push_back("mesh_collection/capsule.off");
	meshFileList.push_back("mesh_collection/chair.off.off");
	meshFileList.push_back("mesh_collection/chinese_dragon.off");
	meshFileList.push_back("mesh_collection/cyberware_face.off");
	meshFileList.push_back("mesh_collection/dancer.off");
	meshFileList.push_back("mesh_collection/dancer2.off");
	meshFileList.push_back("mesh_collection/dino.off");
	meshFileList.push_back("mesh_collection/double-torus.off");
	meshFileList.push_back("mesh_collection/duck.off");
	meshFileList.push_back("mesh_collection/fandisk.off");
	meshFileList.push_back("mesh_collection/feline.off");
	meshFileList.push_back("mesh_collection/fertility_tri.off");
	meshFileList.push_back("mesh_collection/foot.off");
	meshFileList.push_back("mesh_collection/gargoyle.off");
	meshFileList.push_back("mesh_collection/genus4.off");
	meshFileList.push_back("mesh_collection/goro-lowres.off");
	meshFileList.push_back("mesh_collection/grog.off");
	meshFileList.push_back("mesh_collection/hand_bones.off");
	meshFileList.push_back("mesh_collection/hand_pierre.off");
	meshFileList.push_back("mesh_collection/homer.off");
	meshFileList.push_back("mesh_collection/horse.off");
	meshFileList.push_back("mesh_collection/icosphere.off");
	meshFileList.push_back("mesh_collection/julius-80k.off");
	meshFileList.push_back("mesh_collection/knot.off");
	meshFileList.push_back("mesh_collection/max.off");
	meshFileList.push_back("mesh_collection/meta_4.off");
	meshFileList.push_back("mesh_collection/moai.off");
	meshFileList.push_back("mesh_collection/neptune.off");
	meshFileList.push_back("mesh_collection/ninja-lowres.off");
	meshFileList.push_back("mesh_collection/ninjaHead_High.off");
	meshFileList.push_back("mesh_collection/octahedron.off");
	meshFileList.push_back("mesh_collection/octopus.off");
	meshFileList.push_back("mesh_collection/octopus_toon.off");
	meshFileList.push_back("mesh_collection/ogre.off");
	meshFileList.push_back("mesh_collection/oil_pump.off");
	meshFileList.push_back("mesh_collection/pegaso.off");
	meshFileList.push_back("mesh_collection/phong_tessellation_face.off");
	meshFileList.push_back("mesh_collection/polygirl.off");
	meshFileList.push_back("mesh_collection/ram.off");
	meshFileList.push_back("mesh_collection/raptor.off");
	meshFileList.push_back("mesh_collection/rhino.off");
	meshFileList.push_back("mesh_collection/robot.off");
	meshFileList.push_back("mesh_collection/rockerarm_tri.off");
	meshFileList.push_back("mesh_collection/screw_diver.off");
	meshFileList.push_back("mesh_collection/sphere.off");
	meshFileList.push_back("mesh_collection/squirrel.off");
	meshFileList.push_back("mesh_collection/star_knot.off");
	meshFileList.push_back("mesh_collection/temple.off");
	meshFileList.push_back("mesh_collection/torus.off");
	meshFileList.push_back("mesh_collection/tree.off");
	meshFileList.push_back("mesh_collection/tweety.off");
	meshFileList.push_back("mesh_collection/zbrush_head.off");
	meshFileList.push_back("mesh_collection/zbrush_man.off");

}