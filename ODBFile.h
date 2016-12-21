#ifndef ODBFILE_H
#define ODBFILE_H

#include <odb/core.hxx>
#include <odb/lazy-ptr.hxx>

#include <string>

#ifdef ODB_COMPILER
#pragma db model version(1, 1, open)
#endif

#pragma db object pointer(std::shared_ptr) \
                  table("file")
class CODBFile
{
public:
  CODBFile()
  {
    m_filename = "";
    m_synced = false;
  };
  
#pragma db id auto
  unsigned long m_idFile;
#pragma db type("VARCHAR(255)")
  std::string m_filename;
#pragma db transient
  bool m_synced;
private:
    friend class odb::access;

#pragma db index member(m_filename)
};


#endif /* ODBFile_h */
