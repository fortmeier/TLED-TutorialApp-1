/*
    Simple Demo Application for the TLED Solver Library initially created by
    Karsten Noe and modified by Dirk Fortmeier. The source for the library can be found here:

    https://github.com/fortmeier/CUDA-FE-With-Haptics


    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>

*/

#include <GL/glew.h>
#include <vector>
#include <GL/glut.h>

#include <stdio.h>
#include <stdlib.h>

#include "TLEDSolver.h"

static int wWidth = 1024, wHeight = 1024;

TLED::TetrahedralTLEDState* state;
TLED::TetrahedralMesh* mesh;

TLED::Tetrahedron* hTets;
TLED::Point* hPoints;
TLED::Point* hDisplacements;

void createSingleTetrahedron()
{
  int numTetrahedra = 1;
  int numVertices = 4;

  hTets = new TLED::Tetrahedron[numTetrahedra];
  hPoints = new TLED::Point[numVertices];
  hDisplacements = new TLED::Point[numVertices];

  hPoints[0].x = 0;
  hPoints[0].y = 0;
  hPoints[0].z = 0;

  hPoints[1].x = 0;
  hPoints[1].y = 1;
  hPoints[1].z = 0;

  hPoints[2].x = 1;
  hPoints[2].y = 1;
  hPoints[2].z = 1;

  hPoints[3].x = 0;
  hPoints[3].y = 1;
  hPoints[3].z = 1;

  hTets[0].x = 0;
  hTets[0].y = 1;
  hTets[0].z = 2;
  hTets[0].w = 3;

  mesh = TLED::allocAndCopyMesh( hTets, numTetrahedra, hPoints, numVertices );
  state = new TLED::TetrahedralTLEDState();
  TLED::precompute( mesh, state, 0.001f, 0.0f, 0.0f, 10007.0f, 5500.0f, 0.05f, 10.0f );

}

void display()
{
  glPushMatrix();
  for( int i = 0; i < 1; i++ )
  {
    TLED::calculateGravityForces( mesh, state );
    TLED::doTimeStep( mesh, state );
    TLED::applyFloorConstraint( mesh, state, -1 );
  }

  TLED::copyStateToHost( state, mesh, hDisplacements );
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
  glDisable(GL_LIGHTING);
  glEnable(GL_DEPTH_TEST);

  // draw vertices of tetrahedron
  glPointSize( 5 );
  glColor3f( 1, 1, 1 );
  glBegin(GL_POINTS);
  for( int i = 0; i < 4; i++ )
  {
    glVertex3f( hPoints[i].x + hDisplacements[i].x, hPoints[i].y + hDisplacements[i].y, hPoints[i].z + hDisplacements[i].z );
  }
  glEnd();

  // draw edges of tetrahedron
  glColor3f( 0, 0, 1 );
  for( int i = 0; i < 4; i++ )
    for( int j = 0; j < 4; j++ )
    {
      glBegin(GL_LINES);
      glVertex3f( hPoints[i].x + hDisplacements[i].x, hPoints[i].y + hDisplacements[i].y, hPoints[i].z + hDisplacements[i].z );
      glVertex3f( hPoints[j].x + hDisplacements[j].x, hPoints[j].y + hDisplacements[j].y, hPoints[j].z + hDisplacements[j].z );

      glEnd();
    }

  // draw ground plane
  glColor3f( 1, 0, 0 );
  glBegin(GL_QUADS);
  glVertex3f( -2, -1, -2 );
  glVertex3f( -2, -1, 2 );
  glVertex3f( 2, -1, 2 );
  glVertex3f( 2, -1, -2 );
  glEnd();

  glPopMatrix();

  glutPostRedisplay();
  glutSwapBuffers();
}

void keyboard( unsigned char key, int x, int y )
{
  switch( key )
  {
    case 27:
      exit( 0 );
    default:
      break;
  }
}

void reshape( int x, int y )
{
  wWidth = x;
  wHeight = y;
  glViewport( 0, 0, x, y );
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glFrustum( -4, 4, -4, 4, 5, 200 );
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glEnable(GL_LIGHTING);
  //float redMaterial[] = { 1, 1, 1, 1 };
  float lightmat[] = { 0.7f, 0.7f, 0.7f, 0.001f };
  float lightamb[] = { 0.7f, 0.7f, 0.7f, 0.001f };
  float lightpos[] = { 0.f, 0.f, 1500.f, 1.f };
  glEnable(GL_LIGHT0);
  glLightfv( GL_LIGHT0, GL_POSITION, lightpos );
  glLightfv( GL_LIGHT0, GL_DIFFUSE, lightmat );
  gluLookAt( -6, 2, -6, 0, 0, 0, 0, 1, 0 );
  glutPostRedisplay();
}

int main( int argc, char** argv )
{

  glutInit( &argc, argv );

  glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );
  glutInitWindowSize( wWidth, wHeight );
  glutCreateWindow( "Application using the TLED Library" );

  glutDisplayFunc( display );
  glutKeyboardFunc( keyboard );
  glutReshapeFunc( reshape );

  glewInit();

  createSingleTetrahedron();

  glutMainLoop();

  return 0;

}
