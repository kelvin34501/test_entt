#include <iostream>
#include <memory>
#include <exception>
#include <utility>

#include <entt/entt.hpp>

struct Position {
    float x;
    float y;
};

std::ostream &operator<<(std::ostream &os, const Position &pos) {
    os << "{x: " << pos.x << ", y:" << pos.y << "}";
    return os;
}

struct Velocity {
    float dx;
    float dy;
};

void update(entt::registry &registry) {
    auto view = registry.view<Position, Velocity>();

    for (auto entity: view) {
        // gets only the components that are going to be used
        auto &vel = view.get<Velocity>(entity);

        vel.dx = 0.0f;
        vel.dy = 0.0f;
    }
}

void update(std::uint64_t dt, entt::registry &registry) {
    registry.view<Position, Velocity>().each(
            [dt](auto &pos, auto &vel) {
                // gets all the components of the view at once
                pos.x += vel.dx * dt;
                pos.y += vel.dy * dt;
            }
    );
}

void show_position(entt::registry &registry) {
    auto view = registry.view<Position>();

    view.each([](auto &pos) {
        std::cout << pos << '\n';
    });
}

int main(int argn, char *argv[]) {
    std::cout << "Hello, world!\n";

    entt::registry registry;
    std::uint64_t dt = 16;

    for (auto i = 0; i < 10; ++i) {
        auto entity = registry.create();
        registry.emplace<Position>(entity, i * 1.0f, i * 1.0f);
        if (i % 2 == 0) {
            registry.emplace<Velocity>(
                    entity, i * 0.1f, i * 0.1f
            );
        }
    }

    show_position(registry);
    std::cout << "=== split ===\n";
    update(dt, registry);
    update(registry);
    show_position(registry);


    return 0;
}