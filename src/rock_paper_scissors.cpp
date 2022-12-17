#include "rock_paper_scissors.hpp"

#include <filesystem>
#include <iostream>

#include "fixed_loop.hpp"

namespace rps {

enum class PieceType {
    e_rock,
    e_paper,
    e_scissors,
};

struct Piece {
    PieceType type;
    raylib::Vector2 prev_pos;
    raylib::Vector2 pos;
};

std::vector<Piece> init_pieces(int count, int screen_width, int screen_height)
{
    std::vector<Piece> pieces;
    pieces.reserve(count);

    for (int i = 0; i < count; i++) {

        raylib::Vector2 random_pos(
            static_cast<float>(GetRandomValue(0, screen_width)), static_cast<float>(GetRandomValue(0, screen_height)));

        Piece p {
            .type = static_cast<PieceType>(i % 3),
            .prev_pos = random_pos,
            .pos = random_pos,
        };
        pieces.push_back(p);
    }

    return pieces;
}

void update_pieces_pos(std::vector<Piece>& pieces, int screen_width, int screen_height, int piece_size)
{
    const int samples = 10;
    const int max_loops = static_cast<int>(pieces.size());

    for (Piece& p : pieces) {
        raylib::Vector2 prev_vel = p.pos - p.prev_pos;
        p.prev_pos = p.pos;

        float min_dist = std::numeric_limits<float>::max();
        int min_piece_index = -1;
        int sample_count = 0;
        for (int i = 0; i < max_loops; i++) {
            int index = GetRandomValue(0, static_cast<int>(pieces.size()) - 1);
            Piece& rand_piece = pieces.at(index);
            if (rand_piece.type == p.type) {
                continue;
            }
            sample_count++;
            float dist = p.prev_pos.DistanceSqr(rand_piece.prev_pos);
            if (dist < min_dist) {
                min_dist = dist;
                min_piece_index = index;
            }
            if (sample_count >= samples) {
                break;
            }
        }

        if (min_piece_index == -1) {
            continue;
        }

        Piece& p2 = pieces.at(min_piece_index);

        bool is_attracted;
        bool is_same = false;

        switch (p.type) {
        case PieceType::e_rock:
            switch (p2.type) {
            case PieceType::e_rock:
                is_same = true;
                break;
            case PieceType::e_paper:
                is_attracted = false;
                break;
            case PieceType::e_scissors:
                is_attracted = true;
                break;
            }
            break;
        case PieceType::e_paper:
            switch (p2.type) {
            case PieceType::e_rock:
                is_attracted = true;
                break;
            case PieceType::e_paper:
                is_same = true;
                break;
            case PieceType::e_scissors:
                is_attracted = false;
                break;
            }
            break;
        case PieceType::e_scissors:
            switch (p2.type) {
            case PieceType::e_rock:
                is_attracted = false;
                break;
            case PieceType::e_paper:
                is_attracted = true;
                break;
            case PieceType::e_scissors:
                is_same = true;
                break;
            }
            break;
        }

        if (is_same) {
            p.pos += prev_vel;
        }
        else {
            const float repel_speed = 1;
            const float attract_speed = 2;
            if (is_attracted) {
                raylib::Vector2 vel = (p2.prev_pos - p.prev_pos).Normalize() * attract_speed;
                p.pos += vel;
            }
            else {
                raylib::Vector2 vel = ((p2.prev_pos - p.prev_pos).Normalize() * repel_speed).Negate();
                p.pos += vel;
            }
        }

        p.pos.x = std::clamp(p.pos.x, 0.0f, static_cast<float>(screen_width) - static_cast<float>(piece_size));
        p.pos.y = std::clamp(p.pos.y, 0.0f, static_cast<float>(screen_height) - static_cast<float>(piece_size));
    }
}

void update_pieces_collision(std::vector<Piece>& pieces, int piece_size)
{
    float inner_padding = static_cast<float>(piece_size) * 0.2f;
    raylib::Vector2 size(
        static_cast<float>(piece_size) - inner_padding, static_cast<float>(piece_size) - inner_padding);
    for (int p1 = 0; p1 < pieces.size() - 1; p1++) {
        for (int p2 = p1 + 1; p2 < pieces.size(); p2++) {
            raylib::Rectangle p1_rect(pieces.at(p1).pos, size);
            raylib::Rectangle p2_rect(pieces.at(p2).pos, size);

            if (p1_rect.CheckCollision(p2_rect)) {
                switch (pieces.at(p1).type) {
                case PieceType::e_rock:
                    switch (pieces.at(p2).type) {
                    case PieceType::e_rock:
                        break;
                    case PieceType::e_paper:
                        pieces.at(p1).type = PieceType::e_paper;
                        break;
                    case PieceType::e_scissors:
                        pieces.at(p2).type = PieceType::e_rock;
                        break;
                    }
                    break;
                case PieceType::e_paper:
                    switch (pieces.at(p2).type) {
                    case PieceType::e_rock:
                        pieces.at(p2).type = PieceType::e_paper;
                        break;
                    case PieceType::e_paper:
                        break;
                    case PieceType::e_scissors:
                        pieces.at(p1).type = PieceType::e_scissors;
                        break;
                    }
                    break;
                case PieceType::e_scissors:
                    switch (pieces.at(p2).type) {
                    case PieceType::e_rock:
                        pieces.at(p1).type = PieceType::e_rock;
                        break;
                    case PieceType::e_paper:
                        pieces.at(p2).type = PieceType::e_scissors;
                        break;
                    case PieceType::e_scissors:
                        break;
                    }
                    break;
                }
            }
        }
    }
}

void run(const RockPaperScissorsConfig& config)
{
    int width = config.screen_width;
    int height = config.screen_height;

    SetConfigFlags(ConfigFlags::FLAG_VSYNC_HINT);
    SetConfigFlags(ConfigFlags::FLAG_WINDOW_RESIZABLE);

    raylib::Window window(config.screen_width, config.screen_height, "Rock Paper Scissors");

    SetExitKey(KEY_ESCAPE);

    std::filesystem::path res_path = std::filesystem::path(GetApplicationDirectory()) / "../" / "res";

    util::FixedLoop fixed_loop(config.simulation_rate);

    raylib::Image rock_image((res_path / "rock.png").string());
    rock_image.Resize(config.piece_size, config.piece_size);

    raylib::Image paper_image((res_path / "paper.png").string());
    paper_image.Resize(config.piece_size, config.piece_size);

    raylib::Image scissors_image((res_path / "scissors.png").string());
    scissors_image.Resize(config.piece_size, config.piece_size);

    raylib::Texture2D rock_texture(rock_image);
    raylib::Texture2D paper_texture(paper_image);
    raylib::Texture2D scissors_texture(scissors_image);

    std::vector<Piece> pieces = init_pieces(config.piece_count, config.screen_width, config.screen_height);

    fixed_loop.set_callback([&]() {
        update_pieces_pos(pieces, width, height, config.piece_size);
        update_pieces_collision(pieces, config.piece_size);
    });

    while (!window.ShouldClose()) {

        if (IsKeyPressed(KEY_SPACE)) {
            pieces = init_pieces(config.piece_count, width, height);
        }

        if (window.IsResized()) {
            height = window.GetHeight();
            width = window.GetWidth();
        }

        if (IsKeyPressed(KEY_F)) {
            int display = GetCurrentMonitor();
            window.SetSize(GetMonitorWidth(display), GetMonitorHeight(display));
            height = window.GetHeight();
            width = window.GetWidth();
            window.ToggleFullscreen();
        }

        fixed_loop.update();

        BeginDrawing();
        {
            window.ClearBackground(raylib::Color::RayWhite());

            for (Piece& p : pieces) {
                switch (p.type) {
                case PieceType::e_rock:
                    rock_texture.Draw(p.prev_pos.Lerp(p.pos, fixed_loop.blend()));
                    break;
                case PieceType::e_paper:
                    paper_texture.Draw(p.prev_pos.Lerp(p.pos, fixed_loop.blend()));
                    break;
                case PieceType::e_scissors:
                    scissors_texture.Draw(p.prev_pos.Lerp(p.pos, fixed_loop.blend()));
                    break;
                }
            }

            DrawFPS(10, 10);
        }
        EndDrawing();
    }
}

}