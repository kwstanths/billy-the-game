#ifndef __Events_hpp__
#define __Events_hpp__

#include <vector>
#include <functional>
#include <algorithm>

#include "game_engine/utility/HashTable.hpp"

namespace game_engine {
namespace utility {

    class EventDispatcher;
    
    class Event {
        friend EventDispatcher;
    
    private:
        /* Prototype for an event handler */
        typedef std::function<void()> Fn_t;
    
        /* Prototype for the ID of a subscribing object */
        typedef intptr_t SubId_t;
    
        /* Struct to represent a subscribing object */
        struct Subscriber_t {
            SubId_t id_;
            Fn_t handler_;
    
            Subscriber_t(SubId_t id, Fn_t handler) : id_(id), handler_(handler) {};
        };
    
    public:
    
        /**
            Subscribe to en event, when notify is called, your handler will be called. DOES NOT check for multiple subscriptions
            @param id Your id. Should be unique among other subscribing objects
            @param handler Your event handling function
        */
        void Subscribe(SubId_t id, Fn_t handler);
    
        /**
            Unsubscribe from an event
            @param Your id
            @return true = OK, false = Not subscribed
        */
        bool Unsubscribe(SubId_t id);
    
        /**
            Calls the handlers of the subscribers
        */
        void Notify();
    
    private:
        /* Holds whether the event has been notified or not */
        bool notified_ = false;

        /* Holds the subscribers of the event */
        std::vector<Subscriber_t> subscribed_handlers_;
    };
    
    
    class EventDispatcher {
    private:
        typedef unsigned int EventType;
    
    public:
    
        /**
            Creates the object's data structures
            @param number_of_events The number of maximum events. If this number is surpassed, then rehashing will be applied
        */
        EventDispatcher(size_t number_of_events);
    
        /**
            Registers an event
            @param type The type of the event
            @return true = OK, false = Already exists
        */
        bool RegisterEvent(EventType type);
    
        /**
            Subscribe to an event. Does not check for multiple subscriptions for the same id
            @param type Event type
            @param id The id of the subscribing object
            @param handler The handler to be called when event happens
            @return true = OK, false = Event does not exist
        */
        bool SubscribeToEvent(EventType type, Event::SubId_t id, Event::Fn_t handler);
    
        /**
            Unsubscribe from event.
            @param type Event type
            @param id The id of the subscribing object. Should be the same with SubscribeToEvent()
            @return true = OK, false = Event does not exist, or not subscribed
        */
        bool UnsubscribeFromEvent(EventType type, Event::SubId_t id);
    
        /**
            Notify that an event happened
            @param type Event typr
            @return true = OK, false = Event does not exist
        */
        bool NotifyEvent(EventType type);
    
        /* !!!!!!!!!!!!!!! Needs to be private in the end */
        /**
            Calls all the subscribed handlers for all the notified events
        */
        void Dispatch();
    
    private:
    
        /* Holds the registered events */
        HashTable<int, Event *> * events_;
    };
    
}
}


#endif