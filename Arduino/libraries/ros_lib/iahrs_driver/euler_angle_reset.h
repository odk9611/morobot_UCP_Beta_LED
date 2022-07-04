#ifndef _ROS_SERVICE_euler_angle_reset_h
#define _ROS_SERVICE_euler_angle_reset_h
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"

namespace iahrs_driver
{

static const char EULER_ANGLE_RESET[] = "iahrs_driver/euler_angle_reset";

  class euler_angle_resetRequest : public ros::Msg
  {
    public:

    euler_angle_resetRequest()
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

    const char * getType(){ return EULER_ANGLE_RESET; };
    const char * getMD5(){ return "d41d8cd98f00b204e9800998ecf8427e"; };

  };

  class euler_angle_resetResponse : public ros::Msg
  {
    public:
      typedef bool _command_Result_type;
      _command_Result_type command_Result;

    euler_angle_resetResponse():
      command_Result(0)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
      union {
        bool real;
        uint8_t base;
      } u_command_Result;
      u_command_Result.real = this->command_Result;
      *(outbuffer + offset + 0) = (u_command_Result.base >> (8 * 0)) & 0xFF;
      offset += sizeof(this->command_Result);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
      union {
        bool real;
        uint8_t base;
      } u_command_Result;
      u_command_Result.base = 0;
      u_command_Result.base |= ((uint8_t) (*(inbuffer + offset + 0))) << (8 * 0);
      this->command_Result = u_command_Result.real;
      offset += sizeof(this->command_Result);
     return offset;
    }

    const char * getType(){ return EULER_ANGLE_RESET; };
    const char * getMD5(){ return "6b370111df6d9d8116fbdd43bd394a52"; };

  };

  class euler_angle_reset {
    public:
    typedef euler_angle_resetRequest Request;
    typedef euler_angle_resetResponse Response;
  };

}
#endif
