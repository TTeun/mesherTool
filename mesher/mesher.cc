#include "meshbuilder.h"
#include "config.h"
#include "../exc/exc.h"
#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <string>
#include <tuple>

using namespace std;

int main(){
  MeshBuilder::Config config;
  try {
    MeshBuilder::buildMesh(config);
  } catch (Exc &e){
    cout << e << '\n';
  }
}
