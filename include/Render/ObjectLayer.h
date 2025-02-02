#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <tmxlite/Layer.hpp>
#include <tmxlite/Map.hpp>
#include <tmxlite/ObjectGroup.hpp>
#include <vector>

class ObjectLayer : public sf::Drawable, public sf::Transformable
{
public:
    ObjectLayer(const tmx::Map& map, std::size_t layerID);
    const std::vector<sf::Shape*>& getShapes() const;

private:
    sf::Shape* createShape(const tmx::Object& object);
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    std::vector<sf::Shape*> m_shapes;
};

