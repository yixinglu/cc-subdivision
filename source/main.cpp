#include "datatype.h"
#include "readobj.h"

#include <iostream>

#include <gl/freeglut.h>

using namespace ccsubdiv;

static mesh_ptr mesh;
static std::shared_ptr<MeshMgr> mesh_mgr_ptr;

const double eyez = 5.0;
static int spin = 0;
static double rotate_direction[3] = { 0.0, 1.0, 0.0 };


double compute_fovy() {
  auto vdiff = mesh->boundingbox[1] - mesh->boundingbox[0];
  auto center = vdiff * 0.5;
  vec3d eye(0.0, 0.0, eyez);
  double distance = sqrt(dot_prod(eye, center));
  double diag_len = sqrt(dot_prod(vdiff, vdiff));
  return 2.0*atan2(diag_len / 4.0, distance) * 180 / 3.14159265;
}


void init() {
  glClearColor(0.0, 0.0, 0.0, 0.0);
  glShadeModel(GL_SMOOTH);
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_DEPTH_TEST);
}

void display() {
  GLfloat position[] = { 0.0, 0.0, 10.5, 10.0 };

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glColor3f(1.0, 1.0, 1.0);

  auto vdiff = mesh->boundingbox[1] - mesh->boundingbox[0];
  auto center = vdiff * 0.5;

  glLightfv(GL_LIGHT0, GL_POSITION, position);

  glPushMatrix();
  glTranslated(0.0, 0.0, -center[2] - eyez);
  glRotated((GLdouble)spin, rotate_direction[0],
            rotate_direction[1], rotate_direction[2]);
  glTranslated(-center[0], -center[1], -center[2]);

  if (mesh) {
    for (auto & edge : mesh->edges) {
      auto& v1 = edge->vert;
      auto& v2 = edge->next->vert;
      glBegin(GL_LINES);
      glNormal3dv(v1->norm.xyz());
      glVertex3dv(v1->coord.xyz());
      glNormal3dv(v2->norm.xyz());
      glVertex3dv(v2->coord.xyz());
      glEnd();
    }
  }

  glPopMatrix();
  glFlush();
}

void reshape(int w, int h) {
  glViewport(0, 0, (GLsizei)w, (GLsizei)h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  double fovy = compute_fovy();
  gluPerspective(fovy, (GLfloat)w / (GLfloat)h, 1, 100.0);
  glMatrixMode(GL_MODELVIEW);
}

void control_rotation(double x, double y, double z,
                      bool anticlock = true) {
  if (anticlock) {
    spin = (spin + 30) % 360;
  }
  else {
    spin = (spin - 30) % 360;
  }
  rotate_direction[0] = x;
  rotate_direction[1] = y;
  rotate_direction[2] = z;
  glutPostRedisplay();
}

void press_arrow_key(int key, int, int) {
  switch (key)
  {
  case GLUT_KEY_LEFT:
    control_rotation(0, 1, 0, false);
    break;
  case GLUT_KEY_RIGHT:
    control_rotation(0, 1, 0);
    break;
  case GLUT_KEY_UP:
    control_rotation(1, 0, 0, false);
    break;
  case GLUT_KEY_DOWN:
    control_rotation(1, 0, 0);
    break;
  default: break;
  }
}

void press_key(unsigned char key, int, int) {
  if (!mesh_mgr_ptr) return;
  switch (key)
  {
  case 'c':
    mesh = mesh_mgr_ptr->ccsubdiv();
    glutPostRedisplay();
    break;
  case 'u':
    mesh = mesh_mgr_ptr->previous_mesh();
    glutPostRedisplay();
    break;
  default:
    break;
  }
}


int main(int argc, char** argv) {

  if (argc != 2) {
    std::cerr << "Usage: ccsubdiv \"file.obj\"" << std::endl;
    return 1;
  }

  Reader reader(argv[1]);
  if (!reader.is_open()) {
    std::cerr << "Can not open " << argv[1] << std::endl;
    return 1;
  }

  mesh = reader.load_obj_file();
  mesh_mgr_ptr = std::make_shared<MeshMgr>(mesh);

  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(500, 500);
  glutInitWindowPosition(100, 100);
  glutCreateWindow(argv[0]);
  init();
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutKeyboardFunc(press_key);
  glutSpecialFunc(press_arrow_key); // arrow key
  glutMainLoop();

  return 0;
}