#include "libs.h"
#include "scan_data_pack.h"

ScanDataPack::ScanDataPack()
  : m_num_points(0),
    m_velocity(0.0f),
    m_ranges(NULL)
{
  m_time_stamp.hour = m_time_stamp.minute = m_time_stamp.second = 0;
  m_angle.roll = m_angle.pitch = m_angle.yaw = 0.0f;
}

ScanDataPack::ScanDataPack(const ScanDataPack& pack)
{
  m_num_points = pack.m_num_points;
  m_time_stamp = pack.m_time_stamp;
  m_velocity = pack.m_velocity;
  m_angle = pack.m_angle;
  m_ranges = new uint16_t[m_num_points];
  memcpy(m_ranges, pack.m_ranges, m_num_points*sizeof(uint16_t));
}

ScanDataPack::~ScanDataPack()
{
  delete[] m_ranges;
}

ScanDataPack& ScanDataPack::operator=(const ScanDataPack pack)
{
  m_num_points = pack.m_num_points;
  m_time_stamp = pack.m_time_stamp;
  m_velocity = pack.m_velocity;
  m_angle = pack.m_angle;
  m_ranges = new uint16_t[m_num_points];
  memcpy(m_ranges, pack.m_ranges, m_num_points*sizeof(uint16_t));
  return *this;
}

void ScanDataPack::ExtractTimFrame(ScanDataFrame* frame,
      const std::chrono::high_resolution_clock::time_point start_time)
{
  // Get the number of points
  m_num_points = frame->GetNumPoints();

  // Find the time stamp offset from the starting time stamp
  std::chrono::high_resolution_clock::time_point frame_time = frame->GetTimeStamp();
  std::chrono::hours hour_span =
    std::chrono::duration_cast<std::chrono::hours>(frame_time - start_time);
  std::chrono::minutes minute_span =
    std::chrono::duration_cast<std::chrono::minutes>(frame_time - start_time);
  std::chrono::seconds second_span =
    std::chrono::duration_cast<std::chrono::seconds>(frame_time - start_time);
  std::chrono::milliseconds millisecond_span =
    std::chrono::duration_cast<std::chrono::milliseconds>(frame_time - start_time);
  m_time_stamp.hour = (uint32_t)hour_span.count();
  m_time_stamp.minute = (uint8_t)minute_span.count();
  m_time_stamp.second = (uint8_t)second_span.count();
  m_time_stamp.millisecond = (uint8_t)millisecond_span.count();

  // Take the ranges data from the frame
  DataPoint* point_data = frame->GetPointList();
  m_ranges = new uint16_t[m_num_points];
  for (uint32_t i=0; i<m_num_points; ++i) {
    m_ranges[i] = point_data[i].range;
  }
}

int ScanDataPack::SaveToNewBuffer(char** buffer)
{
  int data_size = sizeof(uint32_t) +
                  sizeof(TimeStamp) +
                  sizeof(float) +
                  sizeof(YawPitchRoll) +
                  sizeof(uint16_t)*m_num_points;

  char* data_buffer = new char[data_size];

  memcpy(data_buffer, &m_num_points, sizeof(uint32_t));
  memcpy(data_buffer + sizeof(uint32_t), &m_time_stamp, sizeof(TimeStamp));
  memcpy(data_buffer + sizeof(uint32_t) + sizeof(TimeStamp),
          &m_velocity, sizeof(float));
  memcpy(data_buffer + sizeof(uint32_t) + sizeof(TimeStamp) + sizeof(float),
          &m_angle, sizeof(YawPitchRoll));
  memcpy(data_buffer + sizeof(uint32_t) + sizeof(TimeStamp) +
      sizeof(float) + sizeof(YawPitchRoll),
      m_ranges,
      sizeof(uint16_t)*m_num_points);

  *buffer = data_buffer;

  return data_size;
}
