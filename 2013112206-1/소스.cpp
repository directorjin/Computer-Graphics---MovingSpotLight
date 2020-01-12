#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <gl/glut.h>


GLfloat light0_positon[] = { 5.0, 0.0, 10.0, 1.0 };
GLfloat light0_direction[] = { 0.0f, 0.0f, -1.0f, 0.0f };
GLfloat light0_diffuse[] = { 0.7f, 0.7f, 0.7f };
GLfloat light0_ambient[] = { 0.2f, 0.2f, 0.2f };
GLfloat light0_specular[] = { 1.0f, 1.0f, 1.0f };

//white
GLfloat light1_positon[] = { 0.0, 5.0, 10.0, 1.0 };
GLfloat light1_direction[] = { 0.0f, 0.0f, -1.0f, 0.0f };
GLfloat light1_diffuse[] = { 0.7f, 0.0f, 0.0f };
GLfloat light1_ambient[] = { 0.2f, 0.2f, 0.2f };
GLfloat light1_specular[] = { 1.0f, 0.0f, 0.0f };
//red
GLfloat light2_positon[] = { 0.0, -5.0, 10.0, 1.0 };
GLfloat light2_direction[] = { 0.0f, 0.0f, -1.0f, 0.0f };
GLfloat light2_diffuse[] = { 0.0f, 0.0f, 7.0f };
GLfloat light2_ambient[] = { 0.2f, 0.2f, 0.2f };
GLfloat light2_specular[] = { 0.0f, 0.0f, 1.0f };
//blue
GLfloat light3_positon[] = { -5.0, 0.0, 10.0, 1.0 };
GLfloat light3_direction[] = { 0.0f, 0.0f, -1.0f, 0.0f };
GLfloat light3_diffuse[] = { 0.0f, 7.0f, 0.0f };
GLfloat light3_ambient[] = { 0.2f, 0.2f, 0.2f };
GLfloat light3_specular[] = { 0.0f, 1.0f, 0.0f };
//green
using namespace std;

int Width = 800, Height = 800;
int ManipulateMode = 0;
int RenderMode = 3;

// 관측변환을 위한 변수
int StartPt[2];

// ROTATE를 위한 변수
double Axis[3] = { 1.0, 0.0, 0.0 };
float Angle = 0.0;


double theta0 = 0.0;
double theta1 = 90.0;
double theta2 = 180.0;
double theta3 = 270.0;


// PAN 을 위한 변수
float RotMat[16] = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };
double Distance = -30.0;

struct Vertex
{
	double P[3];
	double N[3];
};

struct Face
{
	int vIdx[3];
};

struct Object
{
	vector<Vertex> VertList;
	vector<Face> FaceList;
};

Object MyModel;

// Forward declaration of functions
void Reshape(int w, int h);
void Mouse(int button, int state, int x, int y);
void MouseMove(int x, int y);
void Timer(int id);

void InitOpenGL();
void LoadModel(char *fname);
void GetSphereCoord(int x, int y, double *p);
void Render();
void RenderFloor();
void RenderModel();

void Cross(double out[3], double a[3], double b[3]);
void Sub(double out[3], double a[3], double b[3]);
void Add(double out[3], double a[3], double b[3]);
void CalcNormal();
void MoveSpotLights();

int main(int argc, char **argv)
{
	// Initialize OpenGL.
	glutInit(&argc, argv);

	// Initialize window size and its position when it is created.
	glutInitWindowSize(Width, Height);
	glutInitWindowPosition(0, 0);

	// Initialize OpenGL display modes for double buffering, RGB color, depth buffer enabling.
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	// Create OpenGL window.
	glutCreateWindow("Hw#10");

	InitOpenGL();
	
	LoadModel("dragon.txt");


	// Register call back functions.
	glutDisplayFunc(Render);
	glutReshapeFunc(Reshape);
	glutMouseFunc(Mouse);
	glutMotionFunc(MouseMove);
	glutTimerFunc(1, Timer, 0);

	// Enter the message loop.
	glutMainLoop();
	return 0;
}

void InitOpenGL()
{
	// Enable depth test
	glEnable(GL_DEPTH_TEST);

	// Enable illumination model
	glEnable(GL_LIGHTING);

	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_POSITION, light0_positon);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light0_specular);
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, light0_direction);
	glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 10);
	glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 16);
	//white

	glEnable(GL_LIGHT1);
	glLightfv(GL_LIGHT1, GL_POSITION, light1_positon);
	glLightfv(GL_LIGHT1, GL_AMBIENT, light1_ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_diffuse);
	glLightfv(GL_LIGHT1, GL_SPECULAR, light1_specular);
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, light1_direction);
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 10);
	glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 16);
	//red


	glEnable(GL_LIGHT2);
	glLightfv(GL_LIGHT2, GL_POSITION, light2_positon);
	glLightfv(GL_LIGHT2, GL_AMBIENT, light2_ambient);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, light2_diffuse);
	glLightfv(GL_LIGHT2, GL_SPECULAR, light2_specular);
	glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, light2_direction);
	glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, 10);
	glLightf(GL_LIGHT2, GL_SPOT_EXPONENT, 16);
	//blue


	glEnable(GL_LIGHT3);
	glLightfv(GL_LIGHT3, GL_POSITION, light3_positon);
	glLightfv(GL_LIGHT3, GL_AMBIENT, light3_ambient);
	glLightfv(GL_LIGHT3, GL_DIFFUSE, light3_diffuse);
	glLightfv(GL_LIGHT3, GL_SPECULAR, light3_specular);
	glLightfv(GL_LIGHT3, GL_SPOT_DIRECTION, light3_direction);
	glLightf(GL_LIGHT3, GL_SPOT_CUTOFF, 10);
	glLightf(GL_LIGHT3, GL_SPOT_EXPONENT, 16);
	//green

}

void Cross(double out[3], double a[3], double b[3])
{
	out[0] = a[1] * b[2] - a[2] * b[1];
	out[1] = a[2] * b[0] - a[0] * b[2];
	out[2] = a[0] * b[1] - a[1] * b[0];
}

void Sub(double out[3], double a[3], double b[3])
{
	out[0] = a[0] - b[0];
	out[1] = a[1] - b[1];
	out[2] = a[2] - b[2];
}

void Add(double out[3], double a[3], double b[3])
{
	out[0] = a[0] + b[0];
	out[1] = a[1] + b[1];
	out[2] = a[2] + b[2];
}

void LoadModel(char *fname)
{
	// Open the user-specified file
	FILE *fp;
	fopen_s(&fp, fname, "r");

	// Read the numbers of vertex and triangle
	int vnum, fnum;

	fscanf_s(fp, "%d", &vnum);
	fscanf_s(fp, "%d", &fnum);

	// Read vertex coordinates
	for (int i = 0; i < vnum; ++i)
	{
		Vertex v;
		fscanf_s(fp, "%lf%lf%lf", &v.P[0], &v.P[1], &v.P[2]);
		v.N[0] = v.N[1] = v.N[2] = 0.0;
		MyModel.VertList.push_back(v);
	}

	// Read triangles
	for (int i = 0; i < fnum; ++i)
	{
		// Read vertex indices from a file.
		int vidx0, vidx1, vidx2;
		fscanf_s(fp, "%d%d%d", &vidx0, &vidx1, &vidx2);

		Face f;
		f.vIdx[0] = vidx0;
		f.vIdx[1] = vidx1;
		f.vIdx[2] = vidx2;
		MyModel.FaceList.push_back(f);
	}

	CalcNormal();
}

void Reshape(int w, int h)
{
	// View-port transformation
	glViewport(0, 0, w, h);
	Width = w;
	Height = h;
}

void SetupViewTransform()
{
	glMatrixMode(GL_MODELVIEW);

	// Load identity matrix
	glLoadIdentity();

	// Translate {WC}
	glTranslatef(0.0, 0.0, Distance);

	// Rotate {WC}
	glRotatef(Angle, Axis[0], Axis[1], Axis[2]);

	// Apply the previous rotation matrix
	glMultMatrixf(RotMat);

	// Save the rotation part of the modelview matrix
	glGetFloatv(GL_MODELVIEW_MATRIX, RotMat);
	RotMat[12] = RotMat[13] = RotMat[14] = 0.0;
}

void SetupViewVolume()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (double)Width / (double)Height, 1.0, 10000.0);
}

void Timer(int id)
{
	theta0 += 1.0;
	theta1 += 1.0;
	theta2 += 1.0;
	theta3 += 1.0;

	glutPostRedisplay();

	glutTimerFunc(10, Timer, 0);
}

void MoveSpotLights()
{
	glMatrixMode(GL_MODELVIEW);

	// Load identity matrix
	glLoadIdentity();


	light0_positon[0] = 5 * cos(theta0 * 3.141592 / 180);
	light0_positon[1] = 5 * sin(theta0 * 3.141592 / 180);
	light0_positon[2] = 10;
	light0_positon[3] = 1.0;
	glLightfv(GL_LIGHT0, GL_POSITION, light0_positon);

	light1_positon[0] = 5 * cos(theta1 * 3.141592 / 180);
	light1_positon[1] = 5 * sin(theta1 * 3.141592 / 180);
	light1_positon[2] = 10;
	light1_positon[3] = 1.0;
	glLightfv(GL_LIGHT1, GL_POSITION, light1_positon);

	light2_positon[0] = 5 * cos(theta2 * 3.141592 / 180);
	light2_positon[1] = 5 * sin(theta2 * 3.141592 / 180);
	light2_positon[2] = 10;
	light2_positon[3] = 1.0;
	glLightfv(GL_LIGHT2, GL_POSITION, light2_positon);

	light3_positon[0] = 5 * cos(theta3 * 3.141592 / 180);
	light3_positon[1] = 5 * sin(theta3 * 3.141592 / 180);
	light3_positon[2] = 10;
	light3_positon[3] = 1.0;
	glLightfv(GL_LIGHT3, GL_POSITION, light3_positon);

	// Write down your codes......
}

void Render()
{
	// Clear color buffer with specified clear color (R, G, B, A)
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Set up moving spot lights
	MoveSpotLights();

	// Set up view transformation
	SetupViewTransform();

	// Set up viewing volume and choose camera lens
	SetupViewVolume();

	// Render scene
	glMatrixMode(GL_MODELVIEW);
	RenderFloor();
	RenderModel();

	// Swap buffers for double buffering.
	glutSwapBuffers();
}

void RenderFloor()
{
	glDisable(GL_LIGHTING);

	for (int i = 0; i < 20; ++i)
	{
		float x, z;
		x = z = (double)i / 19 * 20.0 - 10.0;


		glColor3f(1.0f, 1.0f, 1.0f);
		glBegin(GL_LINES);
		glVertex3f(x, 0.0, -10.0f);
		glVertex3f(x, 0.0, 10.0f);
		glEnd();

		glColor3f(1.0f, 1.0f, 1.0f);
		glBegin(GL_LINES);
		glVertex3f(-10.0f, 0.0, z);
		glVertex3f(10.0f, 0.0, z);
		glEnd();

	}
	glEnable(GL_LIGHTING);
}

void RenderModel()
{
	float mat0_ambient[] = { 0.5, 0.5, 0.5 };
	float mat0_diffuse[] = { 0.5, 0.5, 0.5 };
	float mat0_specular[] = { 1.0, 1.0, 1.0 };
	float mat0_shininess = 32;
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat0_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat0_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat0_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, &mat0_shininess);

	// Draw the model as triangles
	glBegin(GL_TRIANGLES);
	for (int i = 0; i < MyModel.FaceList.size(); ++i)
	{
		int idx0, idx1, idx2;
		idx0 = MyModel.FaceList[i].vIdx[0];
		idx1 = MyModel.FaceList[i].vIdx[1];
		idx2 = MyModel.FaceList[i].vIdx[2];

		glNormal3dv(MyModel.VertList[idx0].N);
		glVertex3dv(MyModel.VertList[idx0].P);

		glNormal3dv(MyModel.VertList[idx1].N);
		glVertex3dv(MyModel.VertList[idx1].P);

		glNormal3dv(MyModel.VertList[idx2].N);
		glVertex3dv(MyModel.VertList[idx2].P);
	}
	glEnd();
}

void Mouse(int button, int state, int x, int y)
{
	if (state == GLUT_DOWN)
	{
		StartPt[0] = x;
		StartPt[1] = y;

		if (button == GLUT_LEFT_BUTTON)
			ManipulateMode = 1;

		if (button == GLUT_RIGHT_BUTTON)
			ManipulateMode = 2;
	}

	if (state == GLUT_UP)
	{
		ManipulateMode = 0;
		StartPt[0] = StartPt[1] = 0;
		Angle = 0.0;
	}
}

void MouseMove(int x, int y)
{
	if (ManipulateMode == 1)
	{
		double p[3];
		double q[3];

		GetSphereCoord(StartPt[0], StartPt[1], p);
		GetSphereCoord(x, y, q);

		Cross(Axis, p, q);

		float len = Axis[0] * Axis[0] + Axis[1] * Axis[1] + Axis[2] * Axis[2];
		if (len < 0.0001)
		{
			Axis[0] = 1.0;
			Axis[1] = 0.0;
			Axis[2] = 0.0;
			Angle = 0.0;
		}
		else
		{
			Angle = p[0] * q[0] + p[1] * q[1] + p[2] * q[2];
			Angle = acos(Angle) * 180.0f / 3.141592f;
		}
	}
	else
	{
		int dy = StartPt[1] - y;
		Distance += dy;
	}

	StartPt[0] = x;
	StartPt[1] = y;

	glutPostRedisplay();
}


void GetSphereCoord(int x, int y, double *p)
{
	float r;
	p[0] = (2.0 * x - Width) / Width;
	p[1] = (-2.0 * y + Height) / Height;

	r = p[0] * p[0] + p[1] * p[1];
	if (r >= 1.0)
	{
		p[0] = p[0] / sqrt(r);
		p[1] = p[1] / sqrt(r);
		p[2] = 0.0;
	}
	else
	{
		p[2] = sqrt(1.0 - r);
	}
}

void CalcNormal()
{
	int vnum = (int)MyModel.VertList.size();
	int fnum = (int)MyModel.FaceList.size();

	for (int i = 0; i < fnum; ++i)
	{
		int vidx0 = MyModel.FaceList[i].vIdx[0];
		int vidx1 = MyModel.FaceList[i].vIdx[1];
		int vidx2 = MyModel.FaceList[i].vIdx[2];

		// Compute the normal vector of a triangle
		double e1[3], e2[3], n[3];
		double *p0 = MyModel.VertList[vidx0].P;
		double *p1 = MyModel.VertList[vidx1].P;
		double *p2 = MyModel.VertList[vidx2].P;
		Sub(e1, p1, p0);
		Sub(e2, p2, p0);
		Cross(n, e1, e2);

		// Add the normal vector to each vertex
		double *n0 = MyModel.VertList[vidx0].N;
		double *n1 = MyModel.VertList[vidx1].N;
		double *n2 = MyModel.VertList[vidx2].N;
		Add(n0, n0, n);
		Add(n1, n1, n);
		Add(n2, n2, n);
	}

	// Normalize the normal vector assigned to each vertex
	for (int i = 0; i < vnum; ++i)
	{
		double *n = MyModel.VertList[i].N;
		double norm = sqrt(n[0] * n[0] + n[1] * n[1] + n[2] * n[2]);
		n[0] /= norm;
		n[1] /= norm;
		n[2] /= norm;
	}
}
