#include "Mesh2D.h"
#include <cassert>

Mesh2D::Mesh2D() {}

void normalizePoints(std::vector<sf::Vertex> &points,
                     std::vector<sf::Vertex> &quads,
                     std::vector<sf::Vertex> &centers,
                     const double             scaler = 1.1f) {
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
  for (auto ptIt = centers.begin(); ptIt != centers.end(); ++ptIt) {
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
    pt.position.x *= 1100.f / (xMax - xMin);
    pt.position.y -= yMin;
    pt.position.y *= 1100.f / (yMax - yMin);
  }
  for (auto &pt : quads) {
    pt.position.x -= xMin;
    pt.position.x *= 1100.f / (xMax - xMin);
    pt.position.y -= yMin;
    pt.position.y *= 1100.f / (yMax - yMin);
  }
  for (auto &pt : centers) {
    pt.position.x -= xMin;
    pt.position.x *= 1100.f / (xMax - xMin);
    pt.position.y -= yMin;
    pt.position.y *= 1100.f / (yMax - yMin);
  }
}

void Mesh2D::showMesh() {
  sf::Color        grey(190, 190, 190);
  sf::RenderWindow window(sf::VideoMode(1100, 1100), "Mesh!");

  std::vector<sf::Vertex> quads;
  for (auto faceIt = _faces.begin(); faceIt != _faces.end(); ++faceIt) {
    assert((*faceIt)->getEdges().size() == 4);
    for (auto vertIt = (*faceIt)->getVertices().begin(); vertIt != (*faceIt)->getVertices().end(); ++vertIt) {
      quads.push_back(sf::Vertex((**vertIt).position, sf::Color::Black));
    }
    quads.push_back(sf::Vertex((**((*faceIt)->getVertices().begin())).position, sf::Color::Black));
  }

  std::vector<sf::Vertex> points;
  for (auto vertIt = getVertices().begin(); vertIt != getVertices().end(); ++vertIt) {
    points.push_back(sf::Vertex((**vertIt).position, sf::Color::Black));
  }
  std::vector<sf::Vertex> centersConnections;
  sf::Vector2f            c;

  for (auto edgeIt = _edges.begin(); edgeIt != _edges.end(); ++edgeIt) {
    c = .5f * (_vertices[edgeIt->first.first]->position + _vertices[edgeIt->first.second]->position);
    sf::Vector2f f1 = 0.6f * edgeIt->second->_faces[0]->getCenter() + 0.4f * c;

    centersConnections.push_back(sf::Vertex(f1, sf::Color::Magenta));
    centersConnections.push_back(sf::Vertex(c, sf::Color::Magenta));
    if (edgeIt->second->_faces[1] != nullptr) {
      f1 = 0.6f * edgeIt->second->_faces[1]->getCenter() + 0.4f * c;
    }
    centersConnections.push_back(sf::Vertex(f1, sf::Color::Magenta));
  }

  normalizePoints(points, quads, centersConnections);

  std::vector<sf::ConvexShape> polygons(quads.size() / 5);

  for (size_t i = 0; i != polygons.size(); ++i) {
    polygons[i].setPointCount(4);
    polygons[i].setPoint(0, quads[5 * i].position);
    polygons[i].setPoint(1, quads[5 * i + 1].position);
    polygons[i].setPoint(2, quads[5 * i + 2].position);
    polygons[i].setPoint(3, quads[5 * i + 3].position);
    polygons[i].setPoint(4, quads[5 * i + 4].position);

    polygons[i].setOutlineColor(sf::Color::Black);
    polygons[i].setOutlineThickness(1);
    auto color = _faces[i]->getType() == Face2D::FaceType::Nozzle
                     ? sf::Color::Red
                     : _faces[i]->getType() == Face2D::FaceType::Pipe ? sf::Color::Green : sf::Color::Yellow;
    polygons[i].setFillColor(color);
  }

  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        window.close();
      }
    }
    window.clear(grey);
    for (size_t i = 0; i != polygons.size(); ++i) {
      window.draw(polygons[i]);
    }
    // for (size_t i = 0; i < centersConnections.size(); i += 3) {
    //   window.draw(centersConnections.data() + i, 3, sf::LinesStrip);
    // }
    window.draw(points.data(), points.size(), sf::Points);
    window.display();
  }
}

void Mesh2D::addFace(
    const size_t idx0, const size_t idx1, const size_t idx2, const size_t idx3, const Face2D::FaceType type) {
  _faces.push_back(std::unique_ptr<Face2D>(new Face2D(
      _vertices[idx0].get(), _vertices[idx1].get(), _vertices[idx2].get(), _vertices[idx3].get(), type)));
  addEdgesFromFace(_faces.back().get());
}

void Mesh2D::addEdgesFromFace(Face2D *addedFace) {
  for (auto vertIt = addedFace->getVertices().begin(); vertIt != addedFace->getVertices().end(); ++vertIt) {
    auto sortedIndices = std::make_pair(std::min((*vertIt)->_index, (*(vertIt + 1))->_index),
                                        std::max((*vertIt)->_index, (*(vertIt + 1))->_index));

    if (vertIt + 1 == addedFace->getVertices().end())
      sortedIndices =
          std::make_pair(std::min((*vertIt)->_index, (*addedFace->getVertices().begin())->_index),
                         std::max((*vertIt)->_index, (*addedFace->getVertices().begin())->_index));

    if (_edges.find(sortedIndices) != _edges.end()) {
      // assert(_edges[sortedIndices]->_faces[0] != nullptr && _edges[sortedIndices]->_faces[1] == nullptr);
      _edges[sortedIndices]->_faces[1] = addedFace;
    } else {
      _edges[sortedIndices] = std::unique_ptr<Edge2D>(new Edge2D(addedFace));
    }
    addedFace->addEdge(_edges[sortedIndices].get());
  }
}
