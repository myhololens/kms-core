#ifndef __MEDIA_PAD_IMPL_HPP__
#define __MEDIA_PAD_IMPL_HPP__

#include "MediaObjectImpl.hpp"
#include "MediaPad.hpp"
#include <EventHandler.hpp>

namespace kurento
{

class MediaElementImpl;
class MediaType;
class MediaPadImpl;

void Serialize (std::shared_ptr<MediaPadImpl> &object, JsonSerializer &serializer);

class MediaPadImpl : public MediaObjectImpl, public virtual MediaPad
{

public:

  MediaPadImpl ();

  virtual ~MediaPadImpl () {};

  std::shared_ptr<MediaElement> getMediaElement ();
  std::shared_ptr<MediaType> getMediaType ();
  std::string getMediaDescription ();

  virtual bool connect (const std::string &eventType, std::shared_ptr<EventHandler> handler);

  class Factory : public virtual MediaObjectImpl::Factory
  {
  public:
    Factory () {};

    virtual std::string getName () const {
      return "MediaPad";
    };

  };

  virtual void invoke (std::shared_ptr<MediaObjectImpl> obj,
                       const std::string &methodName, const Json::Value &params,
                       Json::Value &response);

  virtual void Serialize (JsonSerializer &serializer);

private:

  class StaticConstructor
  {
  public:
    StaticConstructor();
  };

  static StaticConstructor staticConstructor;

};

} /* kurento */

#endif /*  __MEDIA_PAD_IMPL_HPP__ */
