#include <iostream>
#include "Mesh2D.h"
#include "Mesh2DBuilder.h"

using namespace std;

int main() {
  Mesh2D mesh = Mesh2DBuilder::buildAirBearingMesh2D();
  mesh.showMesh();
}