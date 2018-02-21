#include <iostream>
#include "Mesh2DBuilder.h"

using namespace std;

int main() {
  Mesh2D mesh = Mesh2DBuilder::buildAirBearingMesh2D();

#ifdef __HAS__SFML
  mesh.showMesh();
#endif
}