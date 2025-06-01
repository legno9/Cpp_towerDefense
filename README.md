# CMake SFML Tower defense project by Ángel Formoso Caamaño
![image](https://github.com/user-attachments/assets/fb872d1d-699e-483d-a105-d3becb3384aa)

# Simple 2D Tower Defense Game

## Project Description

This is a **2D Tower Defense game** developed in C++ using the SFML multimedia library. The project serves as a comprehensive exercise in game development architecture, focusing on building a robust, modular, and data-driven game system from the ground up. Players aim to strategically place towers to defend their base against waves of incoming enemies, managing gold resources and base health.

## Core Features

* **Solid Game Foundation:** uilt with a robust, data-driven architecture that allows for easy customization and scalability.
* **Level & Enemy Waves:** Dynamically loads levels from Tiled map files and spawns configurable enemy waves that follow predefined paths.
* **Living Game Objects:** Includes diverse enemies, turrets, and projectiles, each with unique behavior and fully configured through external files.
* **Awesome Animations:** Sprite sheets support multiple directions and sequences, including frame-based event triggering.
* **Projectiles Galore:** Specialized shots including:
    * **Homing Projectiles:** Homing Projectiles: Actively track and follow their targets.
    * **Area Damage Projectiles:** Travel to a target location and deal splash damage on impact.
    * **Area Slow Projectiles:** Apply a slowing effect in an area upon reaching their destination.
* **Smart Turrets:** Turrets prioritize the enemy that has advanced the furthest and use a predicted health system to avoid overkilling targets already doomed by other projectiles.
* **Game Economy & Base Survival:** Players earn gold by defeating enemies and spend it to build or upgrade towers. Base health decreases when enemies reach the end of the path.
* **Interactive Interface:** Real-time HUD displays gold and health. Tower selection bar enables placing, upgrading, and selling turrets.
* **Behind-the-Scenes Cleanliness:** Centralized managers handle assets, rendering, game objects, input, levels, and UI.

## Controls

* **Mouse Left Click:**
    * On a tower selection button to select the turret type to build.
    * On "Sell" / "Upgrade" buttons to interact with the active turrets.
    * On a buildable tile to Place, upgrade or sell a turret depending on the previous action selected.
* **Mouse Movement:** Controls a tile indicator to show potential tower placement.

## Main Technical Challenges & Solutions

### 1. Robust Object Lifecycle Management

* **The Challenge:** Dynamically creating and destroying hundreds of game objects (enemies, projectiles, turrets) without introducing **dangling pointers**, **memory leaks**, or **use-after-free crashes**, especially when objects reference each other or are in containers that are modified during iteration.
* **The Solution:** Implemented `std::unique_ptr` for strict object ownership within the `GameObjectManager`. Employed **deferred addition** for newly created objects (queued for addition after the update phase) and **deferred removal** for destroyed objects (processed after the update phase). Critical `sf::Drawable` elements are automatically registered/deregistered with the `RenderManager` in object constructors/destructors. I also introduced controlled **Singleton shutdown order** to prevent access to deallocated managers during application termination.

### 2. Data-Driven Game Design

* **The Challenge:** Externalizing all game parameters (enemy stats, turret properties, animation data, level layouts, wave configurations) from hardcoded C++ into easily editable files.
* **The Solution:** All game data is now configured using **JSON files**. I integrated `nlohmann/json` with custom **`from_json` functions** for automatic, type-safe, and efficient deserialization of complex nested JSON structures into C++ structs. A centralized `JsonManager` (Singleton) handles all JSON file loading and caching.

### 3. Advanced Animation System & Event Synchronization

* **The Challenge:** Building a flexible animation system for sprite sheets with multiple directions and animation sequences, and precisely timing game events (like projectile launches) with specific animation frames.
* **The Solution:** Developed a modular `AnimationComponent` that reads all animation data and associated sprite sheet paths from JSON. Implemented **animation events/callbacks** (`onFrameEvent`) that allow specific game logic (e.g., projectile spawning) to be triggered exactly when a particular animation frame is displayed. The `AnimationDataManager` efficiently loads and **precaches all sprite sheet textures** for an animation set, allowing quick texture switching (e.g., on tower upgrade) without reloading from disk.

### 4. Smart Turret Targeting & Overkill Prevention

* **The Challenge:** Enabling turrets to intelligently select targets (e.g., "furthest advanced enemy") and preventing them from wasting shots on enemies that are already going to die from other incoming damage.
* **The Solution:** Implemented a **"Predicted Health" system** for enemies. When a turret selects an enemy, it calls `enemy->addPredictedDamage()`, marking that enemy as having "damage in flight." `TurretBase::getLeadEnemy()` then avoids targeting enemies whose `calculatePredictedHealth()` is already zero or negative, significantly reducing wasted projectiles. Targets are referenced by **unique IDs** (`unsigned int targetId`) to safely resolve pointers at the moment of interaction, preventing dangling pointer crashes.

### 5. Modular Game Architecture & Dependency Management

* **The Challenge:** Structuring the game into distinct, manageable modules (Managers for Assets, Rendering, Game Objects, Player, World/Level, Input, UI) and clearly defining their interdependencies.
* **The Solution:** Employed the **Singleton pattern** for core, globally accessed services (`RenderManager`, `GameObjectManager`, `AssetManager`) to simplify access and centralize their responsibilities. Utilized **Dependency Injection** where appropriate for non-Singleton components. Implemented **factory methods** within `GameObjectManager` (`spawnEnemy`, `buildTurret`, `buildProjectile`) for polymorphic and data-driven creation of `GameObject` instances.

### 6. Custom User Interface (UI) System

* **The Challenge:** Building an interactive user interface (HUD for stats, tower selection bar with buttons) using SFML's low-level drawing primitives.
* **The Solution:** Developed custom reusable UI elements: `Button` (for clickable actions) and `TextLabel` (for dynamic text display). Implemented top-level UI components (`HUDManager`, `TowerSelectorUI`) that compose these basic elements. These UI components manage their own drawing directly after being added to the `RenderManager` queue.

---
