#ifndef ODBDATA_H
#define ODBDATA_H

#include <odb/core.hxx>
#include <odb/lazy-ptr.hxx>
#include <odb/section.hxx>

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "ODBFile.h"

#ifdef ODB_COMPILER
#pragma db model version(1, 1, open)
#endif

#pragma db object pointer(std::shared_ptr) \
                  table("data")
class CODBData
{
public:
    CODBData()
  {
    m_title = "";
    m_runtime = 0;
  };
  
#pragma db id auto
  unsigned long m_idData;
  odb::lazy_shared_ptr<CODBFile> m_file;
#pragma db type("VARCHAR(255)")
  std::string m_title;
#pragma db section(section_one)
  std::vector< odb::lazy_shared_ptr<CODBFile> > m_files;
#pragma db section(section_two)
  int m_runtime;

  
#pragma db load(eager) update(change)
  odb::section section_one;
  
#pragma db load(lazy) update(change)
  odb::section section_two;

#pragma db transient
  bool m_synced;
  
private:
  friend class odb::access;
  
#pragma db index member(m_file)
#pragma db index member(m_title)
#pragma db index member(m_runtime)
};

#pragma db view \
  object(CODBData) \
  object(CODBFile = file: CODBData::m_files) \
  query(distinct)
struct ODBView_Data
{
  std::shared_ptr<CODBData> data;
};

#endif /* ODBDATA_H */
