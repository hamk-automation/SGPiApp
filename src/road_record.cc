#include "libs.h"
#include "road_record.h"

RoadRecord::RoadRecord()
  : m_data(NULL)
{
}

RoadRecord::~RoadRecord()
{
  delete[] m_data;
}

bool RoadRecord::SaveToFile(const char* filename)
{
  std::ofstream file_out;
  file_out.open(filename, std::ios_base::binary);

  if (!file_out.is_open()) {
    return false;
  }

  // Write the header
  file_out.write((const char*)this, sizeof(RoadRecord) - sizeof(ScanDataPack*));

  // Write the data
  for (uint32_t i=0; i<m_num_packs; ++i) {
    char* data_buffer;
    int data_size = m_data[i].SaveToNewBuffer(&data_buffer);
    file_out.write(data_buffer, data_size);
    delete[] data_buffer;
  }

  file_out.close();

  return true;
}

bool RoadRecord::ReadFromFile(const char* filename)
{
  std::ifstream fin;

  fin.open(filename, std::ios_base::binary);
  if (!fin.is_open()) {
    return false;
  }

  // Read the header
  fin.read((char*)this, sizeof(RoadRecord) - sizeof(ScanDataPack*));

  // Read the data
  m_data = new ScanDataPack[m_num_packs];
  fin.seekg(sizeof(RoadRecord) - sizeof(ScanDataPack*), std::ios::beg); // Incompatibility between OSs

  for (uint32_t i=0; i<m_num_packs; ++i) {
    uint32_t num_points;
    fin.read((char*)&num_points, sizeof(uint32_t));

    TimeStamp time_stamp;
    fin.read((char*)&time_stamp, sizeof(TimeStamp));

    float vel;
    fin.read((char*)&vel, sizeof(float));

    YawPitchRoll angle;
    fin.read((char*)&angle, sizeof(YawPitchRoll));

    uint16_t* ranges = new uint16_t[num_points];
    fin.read((char*)ranges, sizeof(uint16_t)*num_points);

    m_data[i].SetNumPoints(num_points);
    m_data[i].SetTimeStamp(time_stamp);
    m_data[i].SetVelocity(vel);
    m_data[i].SetRotationAngle(angle);
    m_data[i].SetRangeList(ranges);
  }

  fin.close();

  return true;
}
