#include "Events.hpp"


namespace game_engine {
namespace utility {

    void Event::Subscribe(SubId_t id, Fn_t handler) {
        subscribed_handlers_.push_back(Subscriber_t(id, handler));
    }
    
    bool Event::Unsubscribe(SubId_t id) {
        
        int position = -1;
        for (size_t i = 0; i < subscribed_handlers_.size(); i++)
            if (subscribed_handlers_[i].id_ == id)
                position = i;
    
        if (position == -1) return false;
    
        subscribed_handlers_.erase(subscribed_handlers_.begin() + position);
        return true;
    }
    
    void Event::Notify() {
        for (size_t i = 0; i < subscribed_handlers_.size(); i++)
            subscribed_handlers_[i].handler_();
    }
    
    EventDispatcher::EventDispatcher(size_t number_of_events) {
        events_ = new HashTable<int, Event *>(number_of_events, 1.0);
    }
    
    bool EventDispatcher::RegisterEvent(EventType type) {
    
        Event * new_event = new Event();
        bool ret = events_->Insert(type, new_event);
        if (!ret) {
            delete new_event;
            return false;
        }
    
        return true;
    }
    
    bool EventDispatcher::SubscribeToEvent(EventType type, Event::SubId_t id, Event::Fn_t handler) {
    
        HashTable<int, Event *>::iterator itr = events_->Find(type);
        if (itr == events_->end()) {
            return false;
        }
    
        Event * registered_event = itr.GetValue();
        registered_event->Subscribe(id, handler);
    
        return true;
    }
    
    bool EventDispatcher::UnsubscribeFromEvent(EventType type, Event::SubId_t id) {
    
        HashTable<int, Event *>::iterator itr = events_->Find(type);
        if (itr == events_->end()) {
            return false;
        }
    
        Event * registered_event = itr.GetValue();
        bool ret = registered_event->Unsubscribe(id);

        return ret;
    }
    
    bool EventDispatcher::NotifyEvent(EventType type) {
        
        HashTable<int, Event *>::iterator itr = events_->Find(type);
        if (itr == events_->end()) {
            return false;
        }
    
        itr.GetValue()->notified_ = true;
        return true;
    }
    
    void EventDispatcher::Dispatch() {
    
        for (typename HashTable<int, Event *>::iterator itr = events_->begin(); itr != events_->end(); ++itr) {
            if (itr.GetValue()->notified_) {
                itr.GetValue()->Notify();
                itr.GetValue()->notified_ = false;
            }
        }
    }
    
}
}
