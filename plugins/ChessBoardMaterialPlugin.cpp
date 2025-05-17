#include "ChessBoardMaterialPlugin.hpp"
#include "Math/Vector.hpp"
#include "Plugin/MaterialPlugin.hpp"
#include <cmath>
#include <iostream>
#include "Parser/SceneParser.hpp"

using Raytracer::Math::Point;
using Raytracer::Math::Vector;

namespace Raytracer::Plugins {


/* bool ChessBoardMaterialPlugin::parseRgb(const libconfig::Setting &setting,
                                        Core::Color &out)
{
  if (setting.isArray() && setting.getLength() == 3) {
    int r = setting[0], g = setting[1], b = setting[2];
    out   = Core::Color(r, g, b);
    return true;
  }
  if (setting.isGroup() &&
      setting.exists("r") && setting.exists("g") && setting.exists("b")) {
    int r = setting["r"], g = setting["g"], b = setting["b"];
    out   = Core::Color(r, g, b);
    return true;
  }
  return false; 
} */


std::shared_ptr<Plugin::MaterialPlugin> ChessBoardMaterialPlugin::create()
{ return std::make_shared<ChessBoardMaterialPlugin>(); }

bool ChessBoardMaterialPlugin::configure(const libconfig::Setting &config)
{
  try {
    if (config.exists("color1"))
      m_color1 = Parser::SceneParser::parseColor(config, "color1");
    if (config.exists("color2"))
      m_color2 = Parser::SceneParser::parseColor(config, "color2");

    setAmbientCoefficient(config.lookup("ambientCoefficient"));
    setDiffuseCoefficient (config.lookup("diffuseCoefficient"));

    if (config.exists("squareSize"))
      m_squareSize = config.lookup("squareSize");
  }
  catch (const libconfig::SettingException &e) {
    std::cerr << "[ChessBoardMaterial] " << e.what() << '\n';
    return false;
  }
  return true;
}


static bool planeAxes(const Vector<3>& N, int &ia, int &ib)
{
  const double eps = 1e-6;
  const auto &n = N.m_components;
  if (std::fabs(n[1]) > 1.0 - eps) {
    ia = 0;
    ib = 2;
    return true;
  } 
  if (std::fabs(n[0]) > 1.0 - eps) {
    ia = 1;
    ib = 2;
    return true;
  } 
  if (std::fabs(n[2]) > 1.0 - eps) {
    ia = 0;
    ib = 1;
    return true;
  } 
  return false;
}


Core::Color ChessBoardMaterialPlugin::computeColor(
    const Core::Intersection &isec, const Core::Ray & ,
    const std::vector<std::shared_ptr<Core::ILight>> &lights,
    const Core::Scene &scene) const
{
  
  bool lightSquare;
  int ax1, ax2;

  if (planeAxes(isec.getNormal(), ax1, ax2)) {           
    const Point<3> &P = isec.getPoint();
    int i = int(std::floor(P.m_components[ax1] / m_squareSize));
    int j = int(std::floor(P.m_components[ax2] / m_squareSize));
    lightSquare = ((i + j) & 1) == 0;
  } else {                                               
    const Point<3> &P = isec.getPoint();
    int s = int(std::floor(P.m_components[0] / m_squareSize) +
                std::floor(P.m_components[1] / m_squareSize) +
                std::floor(P.m_components[2] / m_squareSize));
    lightSquare = (s & 1) == 0;
  }
  const std::optional<Core::Color> base = lightSquare ? m_color1 : m_color2;

  
  const Vector<3>& N = isec.getNormal();
  Core::Color C = Core::Black;

  for (const auto &L : lights) {

    if (auto amb = std::dynamic_pointer_cast<Core::IAmbientLight>(L)) {
      double I = amb->getIntensity();
      const auto &Lc = amb->getColor();
      C = C.add(Core::Color(
          base->getR()*getAmbientCoefficient()*I*Lc.getR()/255.0,
          base->getG()*getAmbientCoefficient()*I*Lc.getG()/255.0,
          base->getB()*getAmbientCoefficient()*I*Lc.getB()/255.0));
    }

    else if (auto dir = std::dynamic_pointer_cast<Core::IDirectionalLight>(L)) {
      Vector<3> Ld = dir->getDirection()*-1.0;
      double d = std::max(0.0, N.dot(Ld));
      if (d) {
        double k = getDiffuseCoefficient()*d*dir->getIntensity();
        const auto &Lc = dir->getColor();
        C = C.add(Core::Color(
            base->getR()*k*Lc.getR()/255.0,
            base->getG()*k*Lc.getG()/255.0,
            base->getB()*k*Lc.getB()/255.0));
      }
    }

    else if (auto pt = std::dynamic_pointer_cast<Core::IPositionalLight>(L)) {
      Vector<3> Ld = pt->getDirectionFrom(isec.getPoint());
      double d = std::max(0.0, N.dot(Ld));
      if (d) {
        const double eps = 1e-4;
        Point<3> o = isec.getPoint() + N*eps;
        double maxDist = (pt->getPosition() - o).length();
        if (!scene.hasIntersection({o, Ld, eps, maxDist})) {
          double k = getDiffuseCoefficient()*d*pt->getIntensity();
          const auto &Lc = pt->getColor();
          C = C.add(Core::Color(
              base->getR()*k*Lc.getR()/255.0,
              base->getG()*k*Lc.getG()/255.0,
              base->getB()*k*Lc.getB()/255.0));
        }
      }
    }
  }
  return C;
} // namespace Raytracer::Plugins

extern "C" {
Raytracer::Plugin::IPlugin* createPlugin()
{ return new ChessBoardMaterialPlugin(); }
void destroyPlugin(Raytracer::Plugin::IPlugin *p) { delete p; }
}

}