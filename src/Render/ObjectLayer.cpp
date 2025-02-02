#include <Render/ObjectLayer.h>
#include <SFML/Graphics/ConvexShape.hpp>

ObjectLayer::ObjectLayer(const tmx::Map& map, std::size_t layerID)
{
    const auto& layers = map.getLayers();
    if (layerID >= layers.size() || layers[layerID]->getType() != tmx::Layer::Type::Object)
    {
        throw std::runtime_error("Invalid layer ID or layer type.");
    }

    const auto& objectGroup = dynamic_cast<const tmx::ObjectGroup&>(*layers[layerID]);
    for (const auto& object : objectGroup.getObjects())
    {
        if (auto shape = createShape(object))
        {
            m_shapes.emplace_back(shape);
        }
    }
}

const std::vector<sf::Shape*>& ObjectLayer::getShapes() const
{
    return m_shapes;
}

sf::Shape* ObjectLayer::createShape(const tmx::Object& object)
{
    sf::Shape* shape = nullptr;

    switch (object.getShape())
    {
    case tmx::Object::Shape::Rectangle:
        shape = new sf::RectangleShape({ object.getAABB().width, object.getAABB().height });
        shape->setPosition(sf::Vector2f(object.getPosition().x, object.getPosition().y));
        break;

    case tmx::Object::Shape::Ellipse:
        shape = new sf::CircleShape(object.getAABB().width / 2.f);
        shape->setScale(1.f, object.getAABB().height / object.getAABB().width);
        shape->setPosition(sf::Vector2f(object.getPosition().x, object.getPosition().y));
        break;

    case tmx::Object::Shape::Polygon:
    case tmx::Object::Shape::Polyline:
    {
        auto& points = object.getPoints();
        auto convexShape = new sf::ConvexShape(points.size());
        for (std::size_t i = 0; i < points.size(); ++i)
        {
            convexShape->setPoint(i, { points[i].x, points[i].y });
        }
        convexShape->setPosition(sf::Vector2f(object.getPosition().x, object.getPosition().y));
        shape = convexShape;
        break;
    }

    default:
        break;
    }

    if (shape)
    {
        shape->setFillColor(sf::Color::Transparent);
        shape->setOutlineColor(sf::Color::Red);
        shape->setOutlineThickness(0.0f);
    }

    return shape;
}

void ObjectLayer::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();

    for (const auto& shape : m_shapes)
    {
        target.draw(*shape, states);
    }
}