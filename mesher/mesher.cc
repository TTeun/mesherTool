#include "meshbuilder.h"
#include "config.h"
#include "../exc/exc.h"
#include <iostream>

using namespace std;

int main(){
  try {
    MeshBuilder::Config config;
    MeshBuilder::buildMesh(config);
  } catch (Exc &e){
    cout << e << '\n';
  }
}
