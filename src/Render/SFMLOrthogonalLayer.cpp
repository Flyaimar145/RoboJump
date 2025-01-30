#include <Render/SFMLOrthogonalLayer.h>

MapLayer::MapLayer(const tmx::Map& map, std::size_t idx)
{
    const auto& layers = map.getLayers();
    if (map.getOrientation() != tmx::Orientation::Orthogonal)
    {
        std::cout << "Map is not orthogonal - nothing will be drawn" << std::endl;
    }
    else if (idx >= layers.size())
    {
        std::cout << "Layer index " << idx << " is out of range, layer count is " << layers.size() << std::endl;
    }
    else if (layers[idx]->getType() != tmx::Layer::Type::Tile)
    {
        std::cout << "layer " << idx << " is not a Tile layer..." << std::endl;
    }
    else
    {
        const auto tileSize = map.getTileSize();
        m_chunkSize.x = std::floor(m_chunkSize.x / tileSize.x) * tileSize.x;
        m_chunkSize.y = std::floor(m_chunkSize.y / tileSize.y) * tileSize.y;
        m_mapTileSize.x = map.getTileSize().x;
        m_mapTileSize.y = map.getTileSize().y;
        const auto& layer = layers[idx]->getLayerAs<tmx::TileLayer>();
        createChunks(map, layer);

        auto mapSize = map.getBounds();
        m_globalBounds.width = mapSize.width;
        m_globalBounds.height = mapSize.height;
    }
}

MapLayer::~MapLayer() = default;

const sf::FloatRect& MapLayer::getGlobalBounds() const
{
    return m_globalBounds;
}

void MapLayer::setTile(std::int32_t tileX, std::int32_t tileY, tmx::TileLayer::Tile tile, bool refresh)
{
    sf::Vector2u chunkLocale;
    const auto& selectedChunk = getChunkAndTransform(tileX, tileY, chunkLocale);
    selectedChunk->setTile(chunkLocale.x, chunkLocale.y, tile, refresh);
}

tmx::TileLayer::Tile MapLayer::getTile(std::int32_t tileX, std::int32_t tileY)
{
    sf::Vector2u chunkLocale;
    const auto& selectedChunk = getChunkAndTransform(tileX, tileY, chunkLocale);
    return selectedChunk->getTile(chunkLocale.x, chunkLocale.y);
}

void MapLayer::setColor(std::int32_t tileX, std::int32_t tileY, sf::Color color, bool refresh)
{
    sf::Vector2u chunkLocale;
    const auto& selectedChunk = getChunkAndTransform(tileX, tileY, chunkLocale);
    selectedChunk->setColor(chunkLocale.x, chunkLocale.y, color, refresh);
}

sf::Color MapLayer::getColor(std::int32_t tileX, std::int32_t tileY)
{
    sf::Vector2u chunkLocale;
    const auto& selectedChunk = getChunkAndTransform(tileX, tileY, chunkLocale);
    return selectedChunk->getColor(chunkLocale.x, chunkLocale.y);
}

void MapLayer::setOffset(sf::Vector2f offset)
{
    m_offset = offset;
}

sf::Vector2f MapLayer::getOffset() const
{
    return m_offset;
}

void MapLayer::update(sf::Time elapsed)
{
    for (auto& c : m_visibleChunks)
    {
        for (AnimationState& as : c->getActiveAnimations())
        {
            as.currentTime += elapsed;

            tmx::TileLayer::Tile tile;
            std::int32_t animTime = 0;
            auto x = as.animTile.animation.frames.begin();
            while (animTime < as.currentTime.asMilliseconds())
            {
                if (x == as.animTile.animation.frames.end())
                {
                    x = as.animTile.animation.frames.begin();
                    as.currentTime -= sf::milliseconds(animTime);
                    animTime = 0;
                }

                tile.ID = x->tileID;
                animTime += x->duration;
                x++;
            }

            setTile(as.tileCords.x, as.tileCords.y, tile);
        }
    }
}

MapLayer::Chunk::Chunk(const tmx::TileLayer& layer, std::vector<const tmx::Tileset*> tilesets,
    const sf::Vector2f& position, const sf::Vector2f& tileCount,
    const sf::Vector2u& tileSize, std::size_t rowSize,
    TextureResource& tr, const std::map<std::uint32_t, tmx::Tileset::Tile>& animTiles)
    : m_animTiles(animTiles)
{
    setPosition(position);
    layerOpacity = static_cast<sf::Uint8>(layer.getOpacity() / 1.f * 255.f);

    sf::Color vertColour = sf::Color(200, 200, 200, layerOpacity);
    auto offset = layer.getOffset();
    layerOffset = { static_cast<float>(offset.x), static_cast<float>(offset.y) };
    chunkTileCount = { tileCount.x, tileCount.y };
    mapTileSize = tileSize;

    const auto& tileIDs = layer.getTiles();

    for (const auto& ts : tilesets)
    {
        if (ts->getImagePath().empty())
        {
            tmx::Logger::log("This example does not support Collection of Images tilesets", tmx::Logger::Type::Info);
            tmx::Logger::log("Chunks using " + ts->getName() + " will not be created", tmx::Logger::Type::Info);
            continue;
        }
        m_chunkArrays.emplace_back(std::make_unique<ChunkArray>(*tr.find(ts->getImagePath())->second, *ts));
    }
    std::size_t xPos = static_cast<std::size_t>(position.x / tileSize.x);
    std::size_t yPos = static_cast<std::size_t>(position.y / tileSize.y);
    for (auto y = yPos; y < yPos + tileCount.y; ++y)
    {
        for (auto x = xPos; x < xPos + tileCount.x; ++x)
        {
            auto idx = (y * rowSize + x);
            m_chunkTileIDs.emplace_back(tileIDs[idx]);
            m_chunkColors.emplace_back(vertColour);
        }
    }
    generateTiles(true);
}

MapLayer::Chunk::~Chunk() = default;

std::vector<MapLayer::AnimationState>& MapLayer::Chunk::getActiveAnimations()
{
    return m_activeAnimations;
}

tmx::TileLayer::Tile MapLayer::Chunk::getTile(std::int32_t x, std::int32_t y) const
{
    return m_chunkTileIDs[calcIndexFrom(x, y)];
}

void MapLayer::Chunk::setTile(std::int32_t x, std::int32_t y, tmx::TileLayer::Tile tile, bool refresh)
{
    m_chunkTileIDs[calcIndexFrom(x, y)] = tile;
    maybeRegenerate(refresh);
}

sf::Color MapLayer::Chunk::getColor(std::int32_t x, std::int32_t y) const
{
    return m_chunkColors[calcIndexFrom(x, y)];
}

void MapLayer::Chunk::setColor(std::int32_t x, std::int32_t y, sf::Color color, bool refresh)
{
    m_chunkColors[calcIndexFrom(x, y)] = color;
    maybeRegenerate(refresh);
}

bool MapLayer::Chunk::empty() const
{
    return m_chunkArrays.empty();
}
void MapLayer::Chunk::generateTiles(bool registerAnimation)
{
    if (registerAnimation)
    {
        m_activeAnimations.clear();
    }
    for (const auto& ca : m_chunkArrays)
    {
        std::uint32_t idx = 0;
        std::uint32_t xPos = static_cast<std::uint32_t>(getPosition().x / mapTileSize.x);
        std::uint32_t yPos = static_cast<std::uint32_t>(getPosition().y / mapTileSize.y);
        for (auto y = yPos; y < yPos + chunkTileCount.y; ++y)
        {
            for (auto x = xPos; x < xPos + chunkTileCount.x; ++x)
            {
                if (idx < m_chunkTileIDs.size() && m_chunkTileIDs[idx].ID >= ca->m_firstGID
                    && m_chunkTileIDs[idx].ID <= ca->m_lastGID)
                {
                    if (registerAnimation && m_animTiles.find(m_chunkTileIDs[idx].ID) != m_animTiles.end())
                    {
                        AnimationState as;
                        as.animTile = m_animTiles[m_chunkTileIDs[idx].ID];
                        as.startTime = sf::milliseconds(0);
                        as.tileCords = sf::Vector2u(x, y);
                        m_activeAnimations.push_back(as);
                    }

                    sf::Vector2f tileOffset(static_cast<float>(x) * mapTileSize.x, static_cast<float>(y) * mapTileSize.y + mapTileSize.y - ca->tileSetSize.y);

                    auto idIndex = m_chunkTileIDs[idx].ID - ca->m_firstGID;
                    sf::Vector2f tileIndex(sf::Vector2i(idIndex % ca->tsTileCount.x, idIndex / ca->tsTileCount.x));
                    tileIndex.x *= ca->tileSetSize.x;
                    tileIndex.y *= ca->tileSetSize.y;
                    Tile tile =
                    {
                        sf::Vertex(tileOffset - getPosition(), m_chunkColors[idx], tileIndex),
                        sf::Vertex(tileOffset - getPosition() + sf::Vector2f(static_cast<float>(ca->tileSetSize.x), 0.f), m_chunkColors[idx], tileIndex + sf::Vector2f(static_cast<float>(ca->tileSetSize.x), 0.f)),
                        sf::Vertex(tileOffset - getPosition() + sf::Vector2f(sf::Vector2u(ca->tileSetSize.x, ca->tileSetSize.y)), m_chunkColors[idx], tileIndex + sf::Vector2f(sf::Vector2u(ca->tileSetSize.x, ca->tileSetSize.y))),
                        sf::Vertex(tileOffset - getPosition(), m_chunkColors[idx], tileIndex),
                        sf::Vertex(tileOffset - getPosition() + sf::Vector2f(sf::Vector2u(ca->tileSetSize.x, ca->tileSetSize.y)), m_chunkColors[idx], tileIndex + sf::Vector2f(sf::Vector2u(ca->tileSetSize.x, ca->tileSetSize.y))),
                        sf::Vertex(tileOffset - getPosition() + sf::Vector2f(0.f, static_cast<float>(ca->tileSetSize.y)), m_chunkColors[idx], tileIndex + sf::Vector2f(0.f, static_cast<float>(ca->tileSetSize.y)))
                    };
                    doFlips(m_chunkTileIDs[idx].flipFlags, &tile[0].texCoords, &tile[1].texCoords, &tile[2].texCoords, &tile[3].texCoords, &tile[4].texCoords, &tile[5].texCoords);
                    ca->addTile(tile);
                }
                idx++;
            }
        }
    }
}

void MapLayer::Chunk::maybeRegenerate(bool refresh)
{
    if (refresh)
    {
        for (const auto& ca : m_chunkArrays)
        {
            ca->reset();
        }
        generateTiles();
    }
}

std::int32_t MapLayer::Chunk::calcIndexFrom(std::int32_t x, std::int32_t y) const
{
    return x + y * static_cast<std::int32_t>(chunkTileCount.x);
}

void MapLayer::Chunk::flipY(sf::Vector2f* v0, sf::Vector2f* v1, sf::Vector2f* v2, sf::Vector2f* v3, sf::Vector2f* v4, sf::Vector2f* v5)
{
    sf::Vector2f tmp0 = *v0;
    v0->y = v5->y;
    v3->y = v5->y;
    v5->y = tmp0.y;
    sf::Vector2f tmp2 = *v2;
    v2->y = v1->y;
    v4->y = v1->y;
    v1->y = tmp2.y;
}

void MapLayer::Chunk::flipX(sf::Vector2f* v0, sf::Vector2f* v1, sf::Vector2f* v2, sf::Vector2f* v3, sf::Vector2f* v4, sf::Vector2f* v5)
{
    sf::Vector2f tmp0 = *v0;
    v0->x = v1->x;
    v3->x = v1->x;
    v1->x = tmp0.x;
    sf::Vector2f tmp2 = *v2;
    v2->x = v5->x;
    v4->x = v5->x;
    v5->x = tmp2.x;
}

void MapLayer::Chunk::flipD(sf::Vector2f* v0, sf::Vector2f* v1, sf::Vector2f* v2, sf::Vector2f* v3, sf::Vector2f* v4, sf::Vector2f* v5)
{
    sf::Vector2f tmp2 = *v2;
    *v2 = *v4;
    *v4 = tmp2;
    sf::Vector2f tmp0 = *v0;
    *v0 = *v3;
    *v3 = tmp0;
    sf::Vector2f tmp1 = *v1;
    *v1 = *v5;
    *v5 = tmp1;
}

void MapLayer::Chunk::doFlips(std::uint8_t bits, sf::Vector2f* v0, sf::Vector2f* v1, sf::Vector2f* v2, sf::Vector2f* v3, sf::Vector2f* v4, sf::Vector2f* v5)
{
    if (!(bits & tmx::TileLayer::FlipFlag::Horizontal) &&
        !(bits & tmx::TileLayer::FlipFlag::Vertical) &&
        !(bits & tmx::TileLayer::FlipFlag::Diagonal))
    {
        return;
    }
    else if (!(bits & tmx::TileLayer::FlipFlag::Horizontal) &&
        (bits & tmx::TileLayer::FlipFlag::Vertical) &&
        !(bits & tmx::TileLayer::FlipFlag::Diagonal))
    {
        flipY(v0, v1, v2, v3, v4, v5);
    }
    else if ((bits & tmx::TileLayer::FlipFlag::Horizontal) &&
        !(bits & tmx::TileLayer::FlipFlag::Vertical) &&
        !(bits & tmx::TileLayer::FlipFlag::Diagonal))
    {
        flipX(v0, v1, v2, v3, v4, v5);
    }
    else if ((bits & tmx::TileLayer::FlipFlag::Horizontal) &&
        (bits & tmx::TileLayer::FlipFlag::Vertical) &&
        !(bits & tmx::TileLayer::FlipFlag::Diagonal))
    {
        flipY(v0, v1, v2, v3, v4, v5);
        flipX(v0, v1, v2, v3, v4, v5);
    }
    else if (!(bits & tmx::TileLayer::FlipFlag::Horizontal) &&
        !(bits & tmx::TileLayer::FlipFlag::Vertical) &&
        (bits & tmx::TileLayer::FlipFlag::Diagonal))
    {
        flipD(v0, v1, v2, v3, v4, v5);
    }
    else if (!(bits & tmx::TileLayer::FlipFlag::Horizontal) &&
        (bits & tmx::TileLayer::FlipFlag::Vertical) &&
        (bits & tmx::TileLayer::FlipFlag::Diagonal))
    {
        flipX(v0, v1, v2, v3, v4, v5);
        flipD(v0, v1, v2, v3, v4, v5);
    }
    else if ((bits & tmx::TileLayer::FlipFlag::Horizontal) &&
        !(bits & tmx::TileLayer::FlipFlag::Vertical) &&
        (bits & tmx::TileLayer::FlipFlag::Diagonal))
    {
        flipY(v0, v1, v2, v3, v4, v5);
        flipD(v0, v1, v2, v3, v4, v5);
    }
    else if ((bits & tmx::TileLayer::FlipFlag::Horizontal) &&
        (bits & tmx::TileLayer::FlipFlag::Vertical) &&
        (bits & tmx::TileLayer::FlipFlag::Diagonal))
    {
        flipY(v0, v1, v2, v3, v4, v5);
        flipX(v0, v1, v2, v3, v4, v5);
        flipD(v0, v1, v2, v3, v4, v5);
    }
}


MapLayer::Chunk::ChunkArray::ChunkArray(const sf::Texture& t, const tmx::Tileset& ts)
    : m_texture(t)
{
    auto texSize = getTextureSize();
    tileSetSize = ts.getTileSize();
    tsTileCount.x = texSize.x / tileSetSize.x;
    tsTileCount.y = texSize.y / tileSetSize.y;
    m_firstGID = ts.getFirstGID();
    m_lastGID = ts.getLastGID();
}

MapLayer::Chunk::ChunkArray::~ChunkArray() = default;

void MapLayer::Chunk::ChunkArray::reset()
{
    m_vertices.clear();
}

void MapLayer::Chunk::ChunkArray::addTile(const Chunk::Tile& tile)
{
    for (const auto& v : tile)
    {
        m_vertices.push_back(v);
    }
}

sf::Vector2u MapLayer::Chunk::ChunkArray::getTextureSize() const
{
    return m_texture.getSize();
}

void MapLayer::Chunk::ChunkArray::draw(sf::RenderTarget& rt, sf::RenderStates states) const
{
    states.texture = &m_texture;
    rt.draw(m_vertices.data(), m_vertices.size(), sf::Triangles, states);
}

void MapLayer::Chunk::draw(sf::RenderTarget& rt, sf::RenderStates states) const
{
	states.transform *= getTransform();
	for (const auto& a : m_chunkArrays)
	{
		rt.draw(*a, states);
	}
}

MapLayer::Chunk::Ptr& MapLayer::getChunkAndTransform(std::int32_t x, std::int32_t y, sf::Vector2u& chunkRelative)
{
    std::uint32_t chunkX = (x * m_mapTileSize.x) / static_cast<std::uint32_t>(m_chunkSize.x);
    std::uint32_t chunkY = (y * m_mapTileSize.y) / static_cast<std::uint32_t>(m_chunkSize.y);
    chunkRelative.x = ((x * m_mapTileSize.x) - chunkX * static_cast<std::uint32_t>(m_chunkSize.x)) / m_mapTileSize.x;
    chunkRelative.y = ((y * m_mapTileSize.y) - chunkY * static_cast<std::uint32_t>(m_chunkSize.y)) / m_mapTileSize.y;
    return m_chunks[chunkX + chunkY * m_chunkCount.x];
}

void MapLayer::createChunks(const tmx::Map& map, const tmx::TileLayer& layer)
{
    const auto& tileSets = map.getTilesets();
    const auto& layerIDs = layer.getTiles();
    std::uint32_t maxID = std::numeric_limits<std::uint32_t>::max();
    std::vector<const tmx::Tileset*> usedTileSets;

    for (auto i = tileSets.rbegin(); i != tileSets.rend(); ++i)
    {
        for (const auto& tile : layerIDs)
        {
            if (tile.ID >= i->getFirstGID() && tile.ID < maxID)
            {
                usedTileSets.push_back(&(*i));
                break;
            }
        }
        maxID = i->getFirstGID();
    }

    sf::Image fallback;
    fallback.create(2, 2, sf::Color::Magenta);
    for (const auto& ts : usedTileSets)
    {
        const auto& path = ts->getImagePath();

        std::unique_ptr<sf::Texture> newTexture = std::make_unique<sf::Texture>();
        sf::Image img;
        if (!img.loadFromFile(path))
        {
            newTexture->loadFromImage(fallback);
        }
        else
        {
            if (ts->hasTransparency())
            {
                auto transparency = ts->getTransparencyColour();
                img.createMaskFromColor({ transparency.r, transparency.g, transparency.b, transparency.a });
            }
            newTexture->loadFromImage(img);
        }
        m_textureResource.insert(std::make_pair(path, std::move(newTexture)));
    }

    const auto bounds = map.getBounds();
    m_chunkCount.x = static_cast<sf::Uint32>(std::ceil(bounds.width / m_chunkSize.x));
    m_chunkCount.y = static_cast<sf::Uint32>(std::ceil(bounds.height / m_chunkSize.y));

    sf::Vector2u tileSize(map.getTileSize().x, map.getTileSize().y);

    for (auto y = 0u; y < m_chunkCount.y; ++y)
    {
        sf::Vector2f tileCount(m_chunkSize.x / tileSize.x, m_chunkSize.y / tileSize.y);
        for (auto x = 0u; x < m_chunkCount.x; ++x)
        {
            if ((x + 1) * m_chunkSize.x > bounds.width)
            {
                tileCount.x = (bounds.width - x * m_chunkSize.x) / map.getTileSize().x;
            }
            if ((y + 1) * m_chunkSize.y > bounds.height)
            {
                tileCount.y = (bounds.height - y * m_chunkSize.y) / map.getTileSize().y;
            }
            m_chunks.emplace_back(std::make_unique<Chunk>(layer, usedTileSets,
                sf::Vector2f(x * m_chunkSize.x, y * m_chunkSize.y), tileCount, tileSize, map.getTileCount().x, m_textureResource, map.getAnimatedTiles()));
        }
    }
}

void MapLayer::updateVisibility(const sf::View& view) const
{
    sf::Vector2f viewCorner = view.getCenter();
    viewCorner -= view.getSize() / 2.f;

    std::int32_t posX = static_cast<std::int32_t>(std::floor(viewCorner.x / m_chunkSize.x));
    std::int32_t posY = static_cast<std::int32_t>(std::floor(viewCorner.y / m_chunkSize.y));
    std::int32_t posX2 = static_cast<std::int32_t>(std::ceil((viewCorner.x + view.getSize().x) / m_chunkSize.x));
    std::int32_t posY2 = static_cast<std::int32_t>(std::ceil((viewCorner.y + view.getSize().x) / m_chunkSize.y));

    std::vector<Chunk*> visible;
    for (auto y = posY; y < posY2; ++y)
    {
        for (auto x = posX; x < posX2; ++x)
        {
            std::size_t idx = y * std::int32_t(m_chunkCount.x) + x;
            if (idx >= 0u && idx < m_chunks.size() && !m_chunks[idx]->empty())
            {
                visible.push_back(m_chunks[idx].get());
            }
        }
    }

    std::swap(m_visibleChunks, visible);
}

void MapLayer::draw(sf::RenderTarget& rt, sf::RenderStates states) const
{
    states.transform.translate(m_offset);

    updateVisibility(rt.getView());
    for (const auto& c : m_visibleChunks)
    {
        rt.draw(*c, states);
    }
}

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
        //printf("Object: %s\n", object.getName().c_str());
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