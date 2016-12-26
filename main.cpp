#include <iostream>

//#include <ODBData.h>
#include "ODBData-odb.hxx"
//#include <odb_gen/ODBFile.h>
//#include <odb_gen/ODBFile_odb.h>

//#include <odb/mysql/database.hxx>
#include <odb/sqlite/database.hxx>

#include <odb/transaction.hxx>
#include <odb/session.hxx>
#include <odb/schema-catalog.hxx>

#include <memory>

class CDBWrapper {
public:
  CDBWrapper() {
    /*std::string type = "mysql";
    if (type == "mysql") {
      m_db = std::shared_ptr<odb::core::database>(new odb::mysql::database("odb_test", "odb_test", "odb_test", "127.0.0.1", 3306));
    }
    else*/
    {
      m_db = std::shared_ptr<odb::core::database>(new odb::sqlite::database("common.db",
                                                                            SQLITE_OPEN_READWRITE |
                                                                            SQLITE_OPEN_CREATE));
    }

    if (!odb::session::has_current())
      m_odb_session = std::shared_ptr<odb::session>(new odb::session);
  }

  void init() {
    odb::core::transaction t(m_db->begin());
    odb::core::schema_catalog::migrate(*m_db);
    //odb::core::schema_catalog::create_schema(*m_db);
    t.commit();
  }

  std::shared_ptr<odb::database> getDB() {
    return m_db;
  }

  std::shared_ptr<odb::transaction> getTransaction() {
    if (!odb::transaction::has_current())
      return std::shared_ptr<odb::transaction>(new odb::transaction(m_db->begin()));
    return nullptr;
  }

private:
  std::shared_ptr<odb::database> m_db;
  std::shared_ptr<odb::session> m_odb_session;
};

int main() {
  CDBWrapper db;
  db.init();

  {
    std::shared_ptr<odb::transaction> odb_transaction (db.getTransaction());

    //Create some test data
    for (int i = 0; i < 10; i++)
    {
      std::string i_str = std::to_string(i);

      std::shared_ptr<CODBFile> file = std::shared_ptr<CODBFile>(new CODBFile);
      file->m_filename = i_str+"_123.txt";
      file->m_synced = false;

      db.getDB()->persist(*file);

      std::shared_ptr<CODBData> data = std::shared_ptr<CODBData>(new CODBData);
      data->m_title = "DataTitle_"+i_str;
      data->m_file = file;

      //Generate some more files
      for (int j = 0; j < 10; j++)
      {
        std::shared_ptr<CODBFile> new_file = std::shared_ptr<CODBFile>(new CODBFile);
        new_file->m_filename = i_str + "_" + std::to_string(j) + ".txt";
        db.getDB()->persist(*new_file);
        data->m_files.emplace_back(new_file);
      }

      data->m_runtime = 123;

      db.getDB()->persist(*data);
      db.getDB()->update(*data, data->section_one);
      db.getDB()->update(*data, data->section_two);

      std::cout << "Created Data: " << data->m_title << std::endl;
    }

    if(odb_transaction)
      odb_transaction->commit();
  }

  {
    std::shared_ptr<odb::transaction> odb_transaction (db.getTransaction());

    typedef odb::query<ODBView_Data> query;

    odb::result<ODBView_Data> res(db.getDB()->query<ODBView_Data>(query::file::filename.like("%.txt")));
    for (odb::result<ODBView_Data>::iterator iter = res.begin(); iter != res.end(); iter++)
    {
      std::cout << "Query Result: " << iter->data->m_idData << std::endl;
    }

    if(odb_transaction)
      odb_transaction->commit();
  }


  return 0;
}