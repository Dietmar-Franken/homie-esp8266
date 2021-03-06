#pragma once

#include <vector>
#include "Arduino.h"
#include "Homie/Datatypes/Subscription.hpp"
#include "Homie/Datatypes/Callbacks.hpp"
#include "Homie/Limits.hpp"

namespace HomieInternals {
  class HomieClass;
  class BootNormal;
  class BootConfig;
}

class HomieNode {
  friend HomieInternals::HomieClass;
  friend HomieInternals::BootNormal;
  friend HomieInternals::BootConfig;
  public:
    HomieNode(const char* id, const char* type, HomieInternals::NodeInputHandler nodeInputHandler = [](String property, String value) { return false; });

    const char* getId() const { return _id; }
    const char* getType() const { return _type; }

    void subscribe(const char* property, HomieInternals::PropertyInputHandler inputHandler = [](String value) { return false; });

    void subscribeToAll();

    static void forEach(std::function<void(HomieNode *)> f) {
      for (HomieNode* n = _first; n; n = n->_next) {
        f(n);
      }
    }

    static HomieNode* find(const char* id) {
      for (HomieNode* n = _first; n; n = n->_next) {
        if (strcmp(id, n->getId()) == 0) return n;
      }

      return 0;
    }

    static uint8_t getNodeCount() {
      return _nodeCount;
    }

  protected:
    virtual void setup() {}

    virtual void loop() {}

    virtual void onReadyToOperate() {}

    virtual bool handleInput(String const &property, String const &value);

  private:
    const std::vector<HomieInternals::Subscription>& getSubscriptions() const;
    uint8_t getSubscriptionsCount() const;
    bool isSubscribedToAll() const;

    const char* _id;
    const char* _type;
    std::vector<HomieInternals::Subscription> _subscriptions;
    bool _subscribeToAll;
    HomieInternals::NodeInputHandler _inputHandler;
    HomieNode* _next;
    static HomieNode* _first;
    static HomieNode* _last;
    static uint8_t _nodeCount;
};
