#include "Mesh2D.h"
#include "Mesh2DBuilder.h"
#include <iostream>

using namespace std;

int main()
{
  Mesh2D mesh = Mesh2DBuilder::buildAirBearingMesh2D();
  mesh.showMesh();
}