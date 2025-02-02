/*********************************************************************
(c) Matt Marchant & contributors 2016 - 2024
http://trederia.blogspot.com

tmxlite - Zlib license.

This software is provided 'as-is', without any express or
implied warranty. In no event will the authors be held
liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute
it freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented;
you must not claim that you wrote the original software.
If you use this software in a product, an acknowledgment
in the product documentation would be appreciated but
is not required.

2. Altered source versions must be plainly marked as such,
and must not be misrepresented as being the original software.

3. This notice may not be removed or altered from any
source distribution.
*********************************************************************/

/*
Creates an SFML drawable from an Orthogonal tmx map layer.
This is an example of drawing with SFML - not all features
are implemented.
*/

#ifndef SFML_ORTHO_HPP_
#define SFML_ORTHO_HPP_

#include <tmxlite/Map.hpp>
#include <tmxlite/TileLayer.hpp>

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Time.hpp>


class MapLayer final : public sf::Drawable
{
public:

    MapLayer(const tmx::Map& map, std::size_t idx);

    ~MapLayer() = default;
    MapLayer(const MapLayer&) = delete;
    MapLayer& operator = (const MapLayer&) = delete;

    const sf::FloatRect& getGlobalBounds() const { return m_globalBounds; }

    void setTile(std::int32_t tileX, std::int32_t tileY, tmx::TileLayer::Tile tile, bool refresh = true);

    tmx::TileLayer::Tile getTile(std::int32_t tileX, std::int32_t tileY);

    void setColor(std::int32_t tileX, std::int32_t tileY, sf::Color color, bool refresh = true);

    sf::Color getColor(std::int32_t tileX, std::int32_t tileY);

    void setOffset(sf::Vector2f offset) { m_offset = offset; }

    sf::Vector2f getOffset() const { return m_offset; }
    
    void update(sf::Time elapsed);

    bool isTileEmpty(std::int32_t tileX, std::int32_t tileY);

    sf::Vector2u getMapTileSize() const { return m_mapTileSize; }

private:
    //increasing m_chunkSize by 4; fixes render problems when mapsize != chunksize
    // sf::Vector2f m_chunkSize = sf::Vector2f(1024.f, 1024.f);
    sf::Vector2f m_chunkSize = sf::Vector2f(512.f, 512.f);
    sf::Vector2u m_chunkCount;
    sf::Vector2u m_mapTileSize;   // general Tilesize of Map
    sf::FloatRect m_globalBounds;
    sf::Vector2f m_offset;

    using TextureResource = std::map<std::string, std::unique_ptr<sf::Texture>>;
    TextureResource m_textureResource;

    struct AnimationState
    {
        sf::Vector2u tileCords;
        sf::Time startTime;
        sf::Time currentTime;
        tmx::Tileset::Tile animTile;
        std::uint8_t flipFlags;
    };

    class Chunk final : public sf::Transformable, public sf::Drawable
    {
    public:
        using Ptr = std::unique_ptr<Chunk>;
        using Tile = std::array<sf::Vertex, 6u>;

        Chunk(const tmx::TileLayer& layer,  std::vector<const tmx::Tileset*> tilesets,
              const sf::Vector2f& position, const sf::Vector2f& tileCount, 
              const sf::Vector2u& tileSize, std::size_t rowSize,  
              TextureResource& tr,          const std::map<std::uint32_t, tmx::Tileset::Tile>& animTiles);

        void generateTiles(bool registerAnimation = false);
        
        ~Chunk() = default;
        Chunk(const Chunk&) = delete;
        Chunk& operator = (const Chunk&) = delete;

        std::vector<AnimationState>& getActiveAnimations(){ return m_activeAnimations;}

        tmx::TileLayer::Tile getTile(std::int32_t x, std::int32_t y) const;

        void setTile(std::int32_t x, std::int32_t y, tmx::TileLayer::Tile tile, bool refresh);

        sf::Color getColor(std::int32_t x, std::int32_t y) const;

        void setColor(std::int32_t x, std::int32_t y, sf::Color color, bool refresh);

        void maybeRegenerate(bool refresh);

        std::int32_t calcIndexFrom(std::int32_t x, std::int32_t y) const;

        bool empty() const{return m_chunkArrays.empty();}
        
        void flipY(sf::Vector2f* v0, sf::Vector2f* v1, sf::Vector2f* v2, sf::Vector2f* v3, sf::Vector2f* v4, sf::Vector2f* v5);

        void flipX(sf::Vector2f* v0, sf::Vector2f* v1, sf::Vector2f* v2, sf::Vector2f* v3, sf::Vector2f* v4, sf::Vector2f* v5);

        void flipD(sf::Vector2f* v0, sf::Vector2f* v1, sf::Vector2f* v2, sf::Vector2f* v3, sf::Vector2f* v4, sf::Vector2f* v5);

        void doFlips(std::uint8_t bits, sf::Vector2f* v0, sf::Vector2f* v1, sf::Vector2f* v2, sf::Vector2f* v3, sf::Vector2f* v4, sf::Vector2f* v5);

    private:
        class ChunkArray final : public sf::Drawable
        {
        public:
            using Ptr = std::unique_ptr<ChunkArray>;

            tmx::Vector2u tileSetSize;
            sf::Vector2u tsTileCount;
            std::uint32_t m_firstGID, m_lastGID;

            explicit ChunkArray(const sf::Texture& t, const tmx::Tileset& ts);

            ~ChunkArray() = default;
            ChunkArray(const ChunkArray&) = delete;
            ChunkArray& operator = (const ChunkArray&) = delete;

            void reset()
            {
                m_vertices.clear();
            }

            void addTile(const Chunk::Tile& tile);

            sf::Vector2u getTextureSize() const
            { 
                return m_texture.getSize();
            }

        private:
            const sf::Texture& m_texture;
            std::vector<sf::Vertex> m_vertices;
            void draw(sf::RenderTarget& rt, sf::RenderStates states) const override;
        };

        sf::Uint8 layerOpacity;     // opacity of the layer
        sf::Vector2f layerOffset;   // Layer offset
        sf::Vector2u mapTileSize;   // general Tilesize of Map
        sf::Vector2f chunkTileCount;   // chunk tilecount
        std::vector<tmx::TileLayer::Tile> m_chunkTileIDs; // stores all tiles in this chunk for later manipulation
        std::vector<sf::Color> m_chunkColors; // stores colors for extended color effects
        std::map<std::uint32_t, tmx::Tileset::Tile> m_animTiles;    // animation catalogue
        std::vector<AnimationState> m_activeAnimations;     // Animations to be done in this chunk
        std::vector<ChunkArray::Ptr> m_chunkArrays;

        void draw(sf::RenderTarget& rt, sf::RenderStates states) const override;
    };

    std::vector<Chunk::Ptr> m_chunks;
    mutable std::vector<Chunk*> m_visibleChunks;

    Chunk::Ptr& getChunkAndTransform(std::int32_t x, std::int32_t y, sf::Vector2u& chunkRelative);

    void createChunks(const tmx::Map& map, const tmx::TileLayer& layer);

    void updateVisibility(const sf::View& view) const;


    void draw(sf::RenderTarget& rt, sf::RenderStates states) const override;
};

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

#endif // SFML_ORTHOGONAL_LAYER_HPP_
