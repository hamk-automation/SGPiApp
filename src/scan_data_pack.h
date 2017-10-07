#ifndef _SCAN_DATA_PACK_H_
#define _SCAN_DATA_PACK_H_

#include "libs.h"
#include "scan_data_frame.h"

struct TimeStamp
{
  uint32_t hour;
  uint8_t minute;
  uint8_t second;
  uint8_t millisecond;
};

struct YawPitchRoll
{
  float yaw;
  float pitch;
  float roll;
};

class ScanDataPack
{
private:
  uint32_t m_num_points;
  TimeStamp m_time_stamp;
  float m_velocity;
  YawPitchRoll m_angle;
  uint16_t* m_ranges;

public:
  ScanDataPack();
  ScanDataPack(const ScanDataPack& pack);
  ~ScanDataPack();

  ScanDataPack& operator=(const ScanDataPack pack);

  void ExtractTimFrame(ScanDataFrame* frame,
      const std::chrono::high_resolution_clock::time_point start_time);
  int SaveToNewBuffer(char** buffer);

  void SetNumPoints(uint32_t n) { m_num_points = n; }
  void SetTimeStamp(TimeStamp ts) { m_time_stamp = ts; }
  void SetVelocity(float new_velocity) { m_velocity = new_velocity; }
  void SetRotationAngle(YawPitchRoll angle) { m_angle = angle; }
  void SetRangeList(uint16_t* new_range) { m_ranges = new_range; }

  uint32_t GetNumPoints() { return m_num_points; }
  TimeStamp GetTimeStamp() { return m_time_stamp; }
  uint16_t* GetRangeList() { return m_ranges; }
};

#endif // _SCAN_DATA_PACK_H_
