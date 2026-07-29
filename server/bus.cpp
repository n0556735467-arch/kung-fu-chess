#include "bus.hpp"

void Bus::subscribe(EventType type, Handler handler) {
    handlers[type].push_back(handler);
}

void Bus::publish(const BusEvent& event) {
    auto it = handlers.find(event.type);
    if (it == handlers.end()) {
        return;
    }
    for (const Handler& handler : it->second) {
        handler(event);
    }
}