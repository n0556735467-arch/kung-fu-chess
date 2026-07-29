#ifndef BUS_HPP
#define BUS_HPP
#include <string>
#include <vector>
#include <unordered_map>
#include <functional>

enum class EventType { PieceCaptured, MoveLogged, ScoreUpdated, GameStarted, GameEnded };

struct BusEvent {
    EventType type;
    std::string payload;
};

class Bus {
public:
    using Handler = std::function<void(const BusEvent&)>;

    void subscribe(EventType type, Handler handler);
    void publish(const BusEvent& event);

private:
    std::unordered_map<EventType, std::vector<Handler>> handlers;
};

#endif