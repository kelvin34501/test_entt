#include <iostream>
#include <memory>
#include <exception>
#include <utility>
#include <execution>
#include <random>
#include <chrono>

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
//    registry.view<Position, Velocity>().each(
//            [dt](auto &pos, auto &vel) {
//                // gets all the components of the view at once
//                pos.x += vel.dx * dt;
//                pos.y += vel.dy * dt;
//            }
//    );
    auto view = registry.view<Position, Velocity>();
    std::for_each(
            std::execution::par_unseq, view.begin(), view.end(),
            [&view, dt](const auto entity) {
                auto &pos = view.get<Position>(entity);
                const auto &vel = view.get<Velocity>(entity);
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

//    for (auto i = 0; i < 100000000; ++i) {
//        auto entity = registry.create();
//        registry.emplace<Position>(entity, i * 1.0f, i * 1.0f);
//        if (i % 2 == 0) {
//            registry.emplace<Velocity>(
//                    entity, i * 0.1f, i * 0.1f
//            );
//        }
//    }

//    show_position(registry);
//    std::cout << "=== split ===\n";
//    for (auto i = 0; i < 32; ++i) {
//        update(dt, registry);
//    }
//    update(registry);
//    show_position(registry);

    std::random_device rnd_dev;
    std::mt19937 mersenne_engine{rnd_dev()};
    std::uniform_int_distribution<int32_t> dist{1, 60};
    auto gen = [&dist, &mersenne_engine] {
        return dist(mersenne_engine);
    };
    std::vector<int> vec(100000000);
    auto print_container_simple_type = [](const auto &container) {
        for (const auto elem: container) {
            std::cout << elem <<", ";
        }
        std::cout << '\n';
    };
//    print_container_simple_type(vec);

    std::generate(begin(vec), end(vec), gen);
    const auto start_1 = std::chrono::steady_clock::now();
    std::sort(std::execution::seq, begin(vec), end(vec));
    const auto end_1 = std::chrono::steady_clock::now();
    auto diff_1 = end_1 - start_1;
    std::cout << std::chrono::duration<double, std::milli>(diff_1).count() << " ms\n";

    std::generate(begin(vec), end(vec), gen);
    const auto start_2 = std::chrono::steady_clock::now();
    std::sort(std::execution::par, begin(vec), end(vec));
    const auto end_2 = std::chrono::steady_clock::now();
    auto diff_2 = end_2 - start_2;
    std::cout << std::chrono::duration<double, std::milli>(diff_2).count() << " ms\n";

    std::generate(begin(vec), end(vec), gen);
    const auto start_3 = std::chrono::steady_clock::now();
    std::sort(std::execution::par_unseq, begin(vec), end(vec));
    const auto end_3 = std::chrono::steady_clock::now();
    auto diff_3 = end_3 - start_3;
    std::cout << std::chrono::duration<double, std::milli>(diff_3).count() << " ms\n";

    std::generate(begin(vec), end(vec), gen);
    const auto start_4 = std::chrono::steady_clock::now();
    std::sort(begin(vec), end(vec));
    const auto end_4 = std::chrono::steady_clock::now();
    auto diff_4 = end_4 - start_4;
    std::cout << std::chrono::duration<double, std::milli>(diff_4).count() << " ms\n";

//    print_container_simple_type(vec);

    return 0;
}