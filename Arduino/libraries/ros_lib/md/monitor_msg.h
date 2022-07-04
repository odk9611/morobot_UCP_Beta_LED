#ifndef _ROS_md_monitor_msg_h
#define _ROS_md_monitor_msg_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"

namespace md
{

  class monitor_msg : public ros::Msg
  {
    public:
      typedef int32_t _lPosiX_type;
      _lPosiX_type lPosiX;
      typedef int32_t _lPosiY_type;
      _lPosiY_type lPosiY;
      typedef int16_t _sTheta_type;
      _sTheta_type sTheta;
      typedef int16_t _sRealLinearVel_type;
      _sRealLinearVel_type sRealLinearVel;
      typedef int16_t _sRealAngularVel_type;
      _sRealAngularVel_type sRealAngularVel;
      typedef int16_t _sVoltIn_type;
      _sVoltIn_type sVoltIn;
      typedef int16_t _sLeftMotCur_type;
      _sLeftMotCur_type sLeftMotCur;
      typedef int16_t _sRightMotCur_type;
      _sRightMotCur_type sRightMotCur;
      typedef uint8_t _byUS1_type;
      _byUS1_type byUS1;
      typedef uint8_t _byUS2_type;
      _byUS2_type byUS2;
      typedef uint8_t _byUS3_type;
      _byUS3_type byUS3;
      typedef uint8_t _byUS4_type;
      _byUS4_type byUS4;
      typedef uint8_t _byPlatStatus_type;
      _byPlatStatus_type byPlatStatus;
      typedef uint8_t _byDocStatus_type;
      _byDocStatus_type byDocStatus;
      typedef uint8_t _byLeftMotStatus_type;
      _byLeftMotStatus_type byLeftMotStatus;
      typedef uint8_t _byRightMotStatus_type;
      _byRightMotStatus_type byRightMotStatus;
      typedef int32_t _lLeftMotPosi_type;
      _lLeftMotPosi_type lLeftMotPosi;
      typedef int32_t _lRightMotPosi_type;
      _lRightMotPosi_type lRightMotPosi;
      typedef uint8_t _byLeftIOMonitor_type;
      _byLeftIOMonitor_type byLeftIOMonitor;
      typedef uint8_t _byRightIOMonitor_type;
      _byRightIOMonitor_type byRightIOMonitor;

    monitor_msg():
      lPosiX(0),
      lPosiY(0),
      sTheta(0),
      sRealLinearVel(0),
      sRealAngularVel(0),
      sVoltIn(0),
      sLeftMotCur(0),
      sRightMotCur(0),
      byUS1(0),
      byUS2(0),
      byUS3(0),
      byUS4(0),
      byPlatStatus(0),
      byDocStatus(0),
      byLeftMotStatus(0),
      byRightMotStatus(0),
      lLeftMotPosi(0),
      lRightMotPosi(0),
      byLeftIOMonitor(0),
      byRightIOMonitor(0)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
      union {
        int32_t real;
        uint32_t base;
      } u_lPosiX;
      u_lPosiX.real = this->lPosiX;
      *(outbuffer + offset + 0) = (u_lPosiX.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_lPosiX.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_lPosiX.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_lPosiX.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->lPosiX);
      union {
        int32_t real;
        uint32_t base;
      } u_lPosiY;
      u_lPosiY.real = this->lPosiY;
      *(outbuffer + offset + 0) = (u_lPosiY.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_lPosiY.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_lPosiY.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_lPosiY.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->lPosiY);
      union {
        int16_t real;
        uint16_t base;
      } u_sTheta;
      u_sTheta.real = this->sTheta;
      *(outbuffer + offset + 0) = (u_sTheta.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_sTheta.base >> (8 * 1)) & 0xFF;
      offset += sizeof(this->sTheta);
      union {
        int16_t real;
        uint16_t base;
      } u_sRealLinearVel;
      u_sRealLinearVel.real = this->sRealLinearVel;
      *(outbuffer + offset + 0) = (u_sRealLinearVel.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_sRealLinearVel.base >> (8 * 1)) & 0xFF;
      offset += sizeof(this->sRealLinearVel);
      union {
        int16_t real;
        uint16_t base;
      } u_sRealAngularVel;
      u_sRealAngularVel.real = this->sRealAngularVel;
      *(outbuffer + offset + 0) = (u_sRealAngularVel.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_sRealAngularVel.base >> (8 * 1)) & 0xFF;
      offset += sizeof(this->sRealAngularVel);
      union {
        int16_t real;
        uint16_t base;
      } u_sVoltIn;
      u_sVoltIn.real = this->sVoltIn;
      *(outbuffer + offset + 0) = (u_sVoltIn.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_sVoltIn.base >> (8 * 1)) & 0xFF;
      offset += sizeof(this->sVoltIn);
      union {
        int16_t real;
        uint16_t base;
      } u_sLeftMotCur;
      u_sLeftMotCur.real = this->sLeftMotCur;
      *(outbuffer + offset + 0) = (u_sLeftMotCur.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_sLeftMotCur.base >> (8 * 1)) & 0xFF;
      offset += sizeof(this->sLeftMotCur);
      union {
        int16_t real;
        uint16_t base;
      } u_sRightMotCur;
      u_sRightMotCur.real = this->sRightMotCur;
      *(outbuffer + offset + 0) = (u_sRightMotCur.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_sRightMotCur.base >> (8 * 1)) & 0xFF;
      offset += sizeof(this->sRightMotCur);
      *(outbuffer + offset + 0) = (this->byUS1 >> (8 * 0)) & 0xFF;
      offset += sizeof(this->byUS1);
      *(outbuffer + offset + 0) = (this->byUS2 >> (8 * 0)) & 0xFF;
      offset += sizeof(this->byUS2);
      *(outbuffer + offset + 0) = (this->byUS3 >> (8 * 0)) & 0xFF;
      offset += sizeof(this->byUS3);
      *(outbuffer + offset + 0) = (this->byUS4 >> (8 * 0)) & 0xFF;
      offset += sizeof(this->byUS4);
      *(outbuffer + offset + 0) = (this->byPlatStatus >> (8 * 0)) & 0xFF;
      offset += sizeof(this->byPlatStatus);
      *(outbuffer + offset + 0) = (this->byDocStatus >> (8 * 0)) & 0xFF;
      offset += sizeof(this->byDocStatus);
      *(outbuffer + offset + 0) = (this->byLeftMotStatus >> (8 * 0)) & 0xFF;
      offset += sizeof(this->byLeftMotStatus);
      *(outbuffer + offset + 0) = (this->byRightMotStatus >> (8 * 0)) & 0xFF;
      offset += sizeof(this->byRightMotStatus);
      union {
        int32_t real;
        uint32_t base;
      } u_lLeftMotPosi;
      u_lLeftMotPosi.real = this->lLeftMotPosi;
      *(outbuffer + offset + 0) = (u_lLeftMotPosi.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_lLeftMotPosi.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_lLeftMotPosi.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_lLeftMotPosi.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->lLeftMotPosi);
      union {
        int32_t real;
        uint32_t base;
      } u_lRightMotPosi;
      u_lRightMotPosi.real = this->lRightMotPosi;
      *(outbuffer + offset + 0) = (u_lRightMotPosi.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_lRightMotPosi.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_lRightMotPosi.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_lRightMotPosi.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->lRightMotPosi);
      *(outbuffer + offset + 0) = (this->byLeftIOMonitor >> (8 * 0)) & 0xFF;
      offset += sizeof(this->byLeftIOMonitor);
      *(outbuffer + offset + 0) = (this->byRightIOMonitor >> (8 * 0)) & 0xFF;
      offset += sizeof(this->byRightIOMonitor);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
      union {
        int32_t real;
        uint32_t base;
      } u_lPosiX;
      u_lPosiX.base = 0;
      u_lPosiX.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_lPosiX.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_lPosiX.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_lPosiX.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->lPosiX = u_lPosiX.real;
      offset += sizeof(this->lPosiX);
      union {
        int32_t real;
        uint32_t base;
      } u_lPosiY;
      u_lPosiY.base = 0;
      u_lPosiY.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_lPosiY.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_lPosiY.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_lPosiY.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->lPosiY = u_lPosiY.real;
      offset += sizeof(this->lPosiY);
      union {
        int16_t real;
        uint16_t base;
      } u_sTheta;
      u_sTheta.base = 0;
      u_sTheta.base |= ((uint16_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_sTheta.base |= ((uint16_t) (*(inbuffer + offset + 1))) << (8 * 1);
      this->sTheta = u_sTheta.real;
      offset += sizeof(this->sTheta);
      union {
        int16_t real;
        uint16_t base;
      } u_sRealLinearVel;
      u_sRealLinearVel.base = 0;
      u_sRealLinearVel.base |= ((uint16_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_sRealLinearVel.base |= ((uint16_t) (*(inbuffer + offset + 1))) << (8 * 1);
      this->sRealLinearVel = u_sRealLinearVel.real;
      offset += sizeof(this->sRealLinearVel);
      union {
        int16_t real;
        uint16_t base;
      } u_sRealAngularVel;
      u_sRealAngularVel.base = 0;
      u_sRealAngularVel.base |= ((uint16_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_sRealAngularVel.base |= ((uint16_t) (*(inbuffer + offset + 1))) << (8 * 1);
      this->sRealAngularVel = u_sRealAngularVel.real;
      offset += sizeof(this->sRealAngularVel);
      union {
        int16_t real;
        uint16_t base;
      } u_sVoltIn;
      u_sVoltIn.base = 0;
      u_sVoltIn.base |= ((uint16_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_sVoltIn.base |= ((uint16_t) (*(inbuffer + offset + 1))) << (8 * 1);
      this->sVoltIn = u_sVoltIn.real;
      offset += sizeof(this->sVoltIn);
      union {
        int16_t real;
        uint16_t base;
      } u_sLeftMotCur;
      u_sLeftMotCur.base = 0;
      u_sLeftMotCur.base |= ((uint16_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_sLeftMotCur.base |= ((uint16_t) (*(inbuffer + offset + 1))) << (8 * 1);
      this->sLeftMotCur = u_sLeftMotCur.real;
      offset += sizeof(this->sLeftMotCur);
      union {
        int16_t real;
        uint16_t base;
      } u_sRightMotCur;
      u_sRightMotCur.base = 0;
      u_sRightMotCur.base |= ((uint16_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_sRightMotCur.base |= ((uint16_t) (*(inbuffer + offset + 1))) << (8 * 1);
      this->sRightMotCur = u_sRightMotCur.real;
      offset += sizeof(this->sRightMotCur);
      this->byUS1 =  ((uint8_t) (*(inbuffer + offset)));
      offset += sizeof(this->byUS1);
      this->byUS2 =  ((uint8_t) (*(inbuffer + offset)));
      offset += sizeof(this->byUS2);
      this->byUS3 =  ((uint8_t) (*(inbuffer + offset)));
      offset += sizeof(this->byUS3);
      this->byUS4 =  ((uint8_t) (*(inbuffer + offset)));
      offset += sizeof(this->byUS4);
      this->byPlatStatus =  ((uint8_t) (*(inbuffer + offset)));
      offset += sizeof(this->byPlatStatus);
      this->byDocStatus =  ((uint8_t) (*(inbuffer + offset)));
      offset += sizeof(this->byDocStatus);
      this->byLeftMotStatus =  ((uint8_t) (*(inbuffer + offset)));
      offset += sizeof(this->byLeftMotStatus);
      this->byRightMotStatus =  ((uint8_t) (*(inbuffer + offset)));
      offset += sizeof(this->byRightMotStatus);
      union {
        int32_t real;
        uint32_t base;
      } u_lLeftMotPosi;
      u_lLeftMotPosi.base = 0;
      u_lLeftMotPosi.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_lLeftMotPosi.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_lLeftMotPosi.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_lLeftMotPosi.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->lLeftMotPosi = u_lLeftMotPosi.real;
      offset += sizeof(this->lLeftMotPosi);
      union {
        int32_t real;
        uint32_t base;
      } u_lRightMotPosi;
      u_lRightMotPosi.base = 0;
      u_lRightMotPosi.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_lRightMotPosi.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_lRightMotPosi.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_lRightMotPosi.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->lRightMotPosi = u_lRightMotPosi.real;
      offset += sizeof(this->lRightMotPosi);
      this->byLeftIOMonitor =  ((uint8_t) (*(inbuffer + offset)));
      offset += sizeof(this->byLeftIOMonitor);
      this->byRightIOMonitor =  ((uint8_t) (*(inbuffer + offset)));
      offset += sizeof(this->byRightIOMonitor);
     return offset;
    }

    const char * getType(){ return "md/monitor_msg"; };
    const char * getMD5(){ return "265bab62db87a50551604683361197df"; };

  };

}
#endif
