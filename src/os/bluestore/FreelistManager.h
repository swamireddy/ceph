// -*- mode:C++; tab-width:8; c-basic-offset:2; indent-tabs-mode:t -*-
// vim: ts=8 sw=2 smarttab

#ifndef CEPH_OS_BLUESTORE_FREELISTMANAGER_H
#define CEPH_OS_BLUESTORE_FREELISTMANAGER_H

#include <string>
#include <map>
#include <ostream>
#include "common/Mutex.h"
#include "kv/KeyValueDB.h"

class FreelistManager {
  std::string prefix;
  Mutex lock;
  uint64_t total_free;

  std::map<uint64_t, uint64_t> kv_free;    ///< mirrors our kv values in the db

  void _audit();

public:
  FreelistManager() :
    lock("FreelistManager::lock"),
    total_free(0) {
  }

  int init(KeyValueDB *kvdb, std::string prefix);
  void shutdown();

  void dump();

  uint64_t get_total_free() {
    Mutex::Locker l(lock);
    return total_free;
  }

  const std::map<uint64_t,uint64_t>& get_freelist() {
    return kv_free;
  }

  int allocate(
    uint64_t offset, uint64_t length,
    KeyValueDB::Transaction txn);
  int release(
    uint64_t offset, uint64_t length,
    KeyValueDB::Transaction txn);
};


#endif
