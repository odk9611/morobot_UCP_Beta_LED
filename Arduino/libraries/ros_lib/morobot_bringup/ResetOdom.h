#ifndef _ROS_SERVICE_ResetOdom_h
#define _ROS_SERVICE_ResetOdom_h
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"

namespace morobot_bringup
{

static const char RESETODOM[] = "morobot_bringup/ResetOdom";

  class ResetOdomRequest : public ros::Msg
  {
    public:
      typedef float _x_type;
      _x_type x;
      typedef float _y_type;
      _y_type y;
      typedef float _theta_type;
      _theta_type theta;

    ResetOdomRequest():
      x(0),
      y(0),
      theta(0)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
      offset += serializeAvrFloat64(outbuffer + offset, this->x);
      offset += serializeAvrFloat64(outbuffer + offset, this->y);
      offset += serializeAvrFloat64(outbuffer + offset, this->theta);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
      offset += deserializeAvrFloat64(inbuffer + offset, &(this->x));
      offset += deserializeAvrFloat64(inbuffer + offset, &(this->y));
      offset += deserializeAvrFloat64(inbuffer + offset, &(this->theta));
     return offset;
    }

    const char * getType(){ return RESETODOM; };
    const char * getMD5(){ return "938fa65709584ad8e77d238529be13b8"; };

  };

  class ResetOdomResponse : public ros::Msg
  {
    public:

    ResetOdomResponse()
    {
    }

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
     return offset;
    }

    const char * getType(){ return RESETODOM; };
    const char * getMD5(){ return "d41d8cd98f00b204e9800998ecf8427e"; };

  };

  class ResetOdom {
    public:
    typedef ResetOdomRequest Request;
    typedef ResetOdomResponse Response;
  };

}
#endif
