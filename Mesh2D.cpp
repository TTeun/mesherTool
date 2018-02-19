#include "Mesh2D.h"
#include <cassert>

Mesh2D::Mesh2D() {}

void Mesh2D::normalizePoints(std::vector<sf::Vertex> &points,
                             std::vector<sf::Vertex> &quads,
                             const double             scaler) {
  double xMin = std::numeric_limits<double>::max();
  double xMax = std::numeric_limits<double>::min();
  double yMin = std::numeric_limits<double>::max();
  double yMax = std::numeric_limits<double>::min();
  for (auto ptIt = points.begin(); ptIt != points.end(); ++ptIt) {
    xMin = std::min(xMin, static_cast<double>((*ptIt).position.x));
    xMax = std::max(xMax, static_cast<double>((*ptIt).position.x));
    yMin = std::min(yMin, static_cast<double>((*ptIt).position.y));
    yMax = std::max(yMax, static_cast<double>((*ptIt).position.y));
  }
  for (auto ptIt = quads.begin(); ptIt != quads.end(); ++ptIt) {
    xMin = std::min(xMin, static_cast<double>((*ptIt).position.x));
    xMax = std::max(xMax, static_cast<double>((*ptIt).position.x));
    yMin = std::min(yMin, static_cast<double>((*ptIt).position.y));
    yMax = std::max(yMax, static_cast<double>((*ptIt).position.y));
  }
  xMin *= scaler;
  xMax *= scaler;
  yMin *= scaler;
  yMax *= scaler;
  for (auto &pt : points) {
    pt.position.x -= xMin;
    pt.position.x *= 1000.f / (xMax - xMin);
    pt.position.y -= yMin;
    pt.position.y *= 1000.f / (yMax - yMin);
  }
  for (auto &pt : quads) {
    pt.position.x -= xMin;
    pt.position.x *= 1000.f / (xMax - xMin);
    pt.position.y -= yMin;
    pt.position.y *= 1000.f / (yMax - yMin);
  }
}

void Mesh2D::showMesh() {
  sf::Color               grey(190, 190, 190);
  sf::RenderWindow        window(sf::VideoMode(1000, 1000), "Mesh!");
  std::vector<sf::Vertex> quads;
  for (auto faceIt = _faces.begin(); faceIt != _faces.end(); ++faceIt) {
    for (auto vertIt = (*faceIt)->begin(); vertIt != (*faceIt)->end(); ++vertIt) {
      quads.push_back(sf::Vertex((**vertIt).position, sf::Color::Black));
    }
    quads.push_back(sf::Vertex((**((*faceIt)->begin())).position, sf::Color::Black));
  }
  std::vector<sf::Vertex> points;
  for (auto vertIt = _vertices.begin(); vertIt != _vertices.end(); ++vertIt) {
    points.push_back(sf::Vertex((**vertIt).position, sf::Color::Black));
  }
  normalizePoints(points, quads);

  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        window.close();
      }
      // if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
      //   normalizePoints(points, quads, 2.);
      // }
    }
    window.clear(grey);
    for (size_t i = 0; i < quads.size(); i += 5) {
      window.draw(quads.data() + i, 5, sf::LinesStrip);
    }
    window.draw(points.data(), points.size(), sf::Points);
    window.display();
  }
}

void Mesh2D::addFace(size_t idx0, size_t idx1, size_t idx2, size_t idx3) {
  _faces.push_back(std::unique_ptr<Face2D>(
      new Face2D{_vertices[idx0], _vertices[idx1], _vertices[idx2], _vertices[idx3]}));
  addEdgesFromFace(_faces.back().get());
}

void Mesh2D::addEdgesFromFace(Face2D *addedFace) {
  for (auto vertIt = addedFace->begin(); vertIt + 1 != addedFace->end(); ++vertIt) {
    auto sortedIndices = std::make_pair(std::min((*vertIt)->_index, (*(vertIt + 1))->_index),
                                        std::max((*vertIt)->_index, (*(vertIt + 1))->_index));

    if (_edges.find(sortedIndices) != _edges.end()) {
      assert(_edges[sortedIndices]->_faces[0] != nullptr && _edges[sortedIndices]->_faces[1] == nullptr);
      _edges[sortedIndices]->_faces[1] = addedFace;
    } else {
      _edges[sortedIndices] = std::unique_ptr<Edge2D>(new Edge2D(addedFace));
    }
  }
}
